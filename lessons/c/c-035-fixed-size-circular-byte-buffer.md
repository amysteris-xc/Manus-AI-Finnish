# C 035 — Kiinteänkokoinen tavujen ympyräpuskuri

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–034 ja sitä välittömästi edeltävät oppitunnit
**Esimerkkiohjelma:** [`examples/c/c-035-fixed-size-circular-byte-buffer.c`](../../examples/c/c-035-fixed-size-circular-byte-buffer.c)
**Deterministinen testi:** [`tests/c-035-fixed-size-circular-byte-buffer.sh`](../../tests/c-035-fixed-size-circular-byte-buffer.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun pitäisi osata toteuttaa yksinkertainen tavujen ympyräpuskuri, joka ilmoittaa, kun se on täynnä, ja tyhjenee siististi.

## Kertaus

1. Miksi tavupuskurit käyttävät selkeitä `size_t` pituuksia sen sijaan, että käyttäisivät päättymismerkkiä?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuna raakadatana. Se pakottaa käytettäväksi selkeät `size_t` kapasiteetit, tarkistaa kaikki ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessien injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t data[4]; size_t head; size_t tail; size_t count; } Ring;

static int ring_push(Ring *r, uint8_t b)
{
    if (r == NULL || r->count >= 4) return 0;
    r->data[r->head] = b;
    r->head = (r->head + 1) % 4;
    r->count++;
    return 1;
}

static int ring_pop(Ring *r, uint8_t *b)
{
    if (r == NULL || b == NULL || r->count == 0) return 0;
    *b = r->data[r->tail];
    r->tail = (r->tail + 1) % 4;
    r->count--;
    return 1;
}

int main(void)
{
    Ring r = {{0}, 0, 0, 0};
    uint8_t out = 0;
    if (!ring_push(&r, 42) || !ring_push(&r, 99)) return EXIT_FAILURE;
    if (!ring_pop(&r, &out) || out != 42) return EXIT_FAILURE;
    if (printf("Popped: %u\nRemaining: %zu\n", out, r.count) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-035-fixed-size-circular-byte-buffer \
  examples/c/c-035-fixed-size-circular-byte-buffer.c
./c-035-fixed-size-circular-byte-buffer
```

Odotettu tuloste:

```text
Popped: 42
Remaining: 1
```

## Ohjattu harjoitus

1. Tunnista sisääntulo- ja ulostulopuskurien rajat.
2. Etsi jokainen paluuarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisina datoina raakapuskureissa.
4. Nimeä yksi virheellinen syöte-ehdoista, joka aiheuttaa ennenaikaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa niin, että se validoi jonkin lisärajatapauksen tai testisyötteen. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-035-fixed-size-circular-byte-buffer.sh
```

Käännä ja suorita sanitizerien kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-035-sanitized \
  examples/c/c-035-fixed-size-circular-byte-buffer.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen suojausmekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Raakavaatupuskurit voivat sisältää `0x00` kelvollisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Etenevän silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveysalueen kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
