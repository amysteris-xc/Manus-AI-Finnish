# C 037 — Tarkistettu muuttuvapituisen tavun kokonaislukukoodaus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–036 ja välittömästi edeltävät oppitunnit
**Kumppaniohjelma:** [`examples/c/c-037-checked-variable-byte-integer-encoding.c`](../../examples/c/c-037-checked-variable-byte-integer-encoding.c)
**Deterministinen testi:** [`tests/c-037-checked-variable-byte-integer-encoding.sh`](../../tests/c-037-checked-variable-byte-integer-encoding.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat koodata 32-bittisen etumerkittömän kokonaisluvun 7-bittisillä jatkotavuilla (LEB128/Varint).

## Kertaus

1. Miksi tavupuskurit on ilmoitettava eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin sentinellipäättymää?
2. Muodosta turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosten paluuarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään lasketuna raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessien injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int encode_varint(uint32_t val, uint8_t *out, size_t cap, size_t *used)
{
    if (out == NULL || used == NULL || cap == 0) return 0;
    size_t i = 0;
    while (val >= 0x80) {
        if (i >= cap) return 0;
        out[i++] = (uint8_t)((val & 0x7F) | 0x80);
        val >>= 7;
    }
    if (i >= cap) return 0;
    out[i++] = (uint8_t)(val & 0x7F);
    *used = i;
    return 1;
}

int main(void)
{
    uint8_t buf[5];
    size_t len = 0;
    if (!encode_varint(300, buf, sizeof buf, &len)) return EXIT_FAILURE;
    if (printf("Encoded bytes: %zu\n", len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-037-checked-variable-byte-integer-encoding \
  examples/c/c-037-checked-variable-byte-integer-encoding.c
./c-037-checked-variable-byte-integer-encoding
```

Odotettu tulostus:

```text
Encoded bytes: 2
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen paluuarvon ja kapasiteetin tarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellinen syöte, joka aiheuttaa varhaisen virhepaluun.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-037-checked-variable-byte-integer-encoding.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-037-sanitized \
  examples/c/c-037-checked-variable-byte-integer-encoding.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen turvamekanismi.

## Paljastus: Vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoja ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava suunniteltu oppitunti palaa venäjänkieliseen linjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
