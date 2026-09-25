# C 057 — Kattava tavujonon tilakone

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–056 ja välittömästi edeltävät oppitunnit
**Liitännäinen ohjelma:** [`examples/c/c-057-comprehensive-byte-stream-state-machine.c`](../../examples/c/c-057-comprehensive-byte-stream-state-machine.c)
**Deterministinen testi:** [`tests/c-057-comprehensive-byte-stream-state-machine.sh`](../../tests/c-057-comprehensive-byte-stream-state-machine.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopussa osaat jäsentää kehysprotokollan (otsikko, hyötykuorma, tarkistussumma) eksplisiittistä tilakonetta ja virhesiirtymiä käyttäen.

## Kertausharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että ne perustuisivat sentinellipäättelyyn?
2. Ilmoita turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja konversiofunktioiden palautusarvot on tarkistettava ennen tulosten käyttämistä.
4. Muista yksi oikea lause edelliseltä venäjän oppitunnilta.

## Keskeinen käsite

Esimerkki käsittelee tavuja lasketussa raakatietona. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATE_MAGIC, STATE_LEN, STATE_PAYLOAD, STATE_DONE, STATE_ERROR } ParserState;

typedef struct {
    ParserState state;
    size_t expected_len;
    size_t read_len;
    uint8_t payload[8];
} ProtocolParser;

static void parser_feed(ProtocolParser *p, uint8_t byte)
{
    switch (p->state) {
    case STATE_MAGIC:
        p->state = (byte == 0xAA) ? STATE_LEN : STATE_ERROR;
        break;
    case STATE_LEN:
        if (byte > sizeof p->payload) p->state = STATE_ERROR;
        else {
            p->expected_len = byte;
            p->read_len = 0;
            p->state = (byte == 0) ? STATE_DONE : STATE_PAYLOAD;
        }
        break;
    case STATE_PAYLOAD:
        p->payload[p->read_len++] = byte;
        if (p->read_len == p->expected_len) p->state = STATE_DONE;
        break;
    default:
        break;
    }
}

int main(void)
{
    ProtocolParser p = {STATE_MAGIC, 0, 0, {0}};
    const uint8_t stream[] = {0xAA, 0x02, 0x10, 0x20};
    for (size_t i = 0; i < sizeof stream; ++i) parser_feed(&p, stream[i]);
    if (p.state != STATE_DONE) return EXIT_FAILURE;
    if (printf("Parser finished successfully with %zu payload bytes\n", p.read_len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-057-comprehensive-byte-stream-state-machine \
  examples/c/c-057-comprehensive-byte-stream-state-machine.c
./c-057-comprehensive-byte-stream-state-machine
```

Odotettu tuloste:

```text
Parser finished successfully with 2 payload bytes
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen palautusarvon ja kapasiteetin tarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisina datoina raakapuskureissa.
4. Mainitse yksi väärinmuodostunut syöte, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edeltävältä venäjän oppitunnilta.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan yksi lisä-äärintapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautusarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-057-comprehensive-byte-stream-state-machine.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-057-sanitized \
  examples/c/c-057-comprehensive-byte-stream-state-machine.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautukset ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen turvamekanismi.

## Juonipaljastus: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Väärinmuodostuneet syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjän opintopolkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
