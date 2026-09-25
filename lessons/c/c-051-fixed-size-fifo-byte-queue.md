# C 051 — Kiinteänkokoinen FIFO-tavujono

**Arvioitu oppimisaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–050 ja sitä välittömästi edeltävät oppitunnit
**Seuralaisohjelma:** [`examples/c/c-051-fixed-size-fifo-byte-queue.c`](../../examples/c/c-051-fixed-size-fifo-byte-queue.c)
**Deterministinen testi:** [`tests/c-051-fixed-size-fifo-byte-queue.sh`](../../tests/c-051-fixed-size-fifo-byte-queue.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat hallita kiinteän 16 tavun FIFO-jonon, mukaan lukien luku-/kirjoituslaskurit ja rajojen tarkistukset.

## Kertaus

1. Miksi tavupuskurit on määritettävä eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin sentineliä päättymisen merkkinä?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnoksen palautusarvot täytyy tarkistaa ennen tulosten käyttämistä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää kannettavuuden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessiin injektointia, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t buf[16]; size_t r; size_t w; size_t n; } FIFO;

static int fifo_write(FIFO *f, const uint8_t *src, size_t len)
{
    if (f == NULL || src == NULL || len > 16 - f->n) return 0;
    for (size_t i = 0; i < len; ++i) {
        f->buf[f->w] = src[i];
        f->w = (f->w + 1) % 16;
        f->n++;
    }
    return 1;
}

int main(void)
{
    FIFO f = {{0}, 0, 0, 0};
    const uint8_t chunk[] = {1, 2, 3};
    if (!fifo_write(&f, chunk, sizeof chunk)) return EXIT_FAILURE;
    if (printf("Queued bytes: %zu\n", f.n) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-051-fixed-size-fifo-byte-queue \
  examples/c/c-051-fixed-size-fifo-byte-queue.c
./c-051-fixed-size-fifo-byte-queue
```

Odotettu tuloste:

```text
Queued bytes: 3
```

## Ohjattu harjoitus

1. Tunnista tulo- ja tulostuspuskureiden rajat.
2. Etsi jokainen paluuarvon ja kapasiteettitarkistuksen kohta.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellisesti muotoiltu syöte-tilanne, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan jokin lisärajatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-051-fixed-size-fifo-byte-queue.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos ympäristö tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-051-sanitized \
  examples/c/c-051-fixed-size-fifo-byte-queue.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole käytännössä toimiva kryptografinen turvamekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Raakapuskurit voivat sisältää `0x00` laillisena datana; `strlen` eivät pysty määrittämään niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheellisesti muotoillut syötteet rikkovat kapasiteetti- tai rakennerajoja ja niiden täytyy palauttaa virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed width integer types — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Arithmetic operators — cppreference.com"
