# C 062 — Lajiteltujen tavuerätietueiden haku

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–061
**Companion program:** [`examples/c/c-062-sorted-byte-record-search.c`](../../examples/c/c-062-sorted-byte-record-search.c)
**Deterministic test:** [`tests/c-062-sorted-byte-record-search.sh`](../../tests/c-062-sorted-byte-record-search.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun pitäisi osata hakea lajitellusta, kiinteän levyisestä tavutietue­taulukosta käyttäen validoitua vertailusopimusta.

## Muistiharjoitus

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston paluuarvo, joka täytyy tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Hakulogiikka tarvitsee vakaana pysyvän tietueleveyden ja eksplisiittisen tietueiden lukumäärän.

Kumppaniohjelma käsittelee raakadataa tavujonoina, joiden pituus on eksplisiittinen. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan muuttamista. Ohjelma on siirrettävä ISO C17 -ohjelma ja tarkoituksellisesti poissulkee shellcoden, natiivikoodin lataajat, mielivaltaisen assemblerin suorituksen, prosessin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_bounded(const uint8_t *src, size_t len, uint8_t *dst, size_t cap)
{
    if ((src == NULL && len != 0) || dst == NULL || len > cap) return 0;
    if (len != 0) memmove(dst, src, len);
    return 1;
}

int main(void)
{
    const uint8_t source[] = {UINT8_C(0), UINT8_C(62), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 062 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-062-sorted-byte-record-search \
  examples/c/c-062-sorted-byte-record-search.c
./c-062-sorted-byte-record-search
```

Odotettu tulostus:

```text
C 062 copied bytes: 0 62 255
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- ja puskurin-plus-kapasiteettisopimukset.
2. Etsi aikaisin tapahtuva virheellinen syöte -tarkistus.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabyte-puskurin loppuna.
4. Mainitse, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelisi hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty rajatapaus. Säilytä olemassa olevat argumenttien tarkistukset, eksplisiittiset pituudet ja tulostusten paluuarvon tarkistukset. Jos muutat mitään muistinvaraustai laskennallista aritmetiikkaa, lisää tarkistettu ylivuotoehto ennen laskutoimitusta.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-062-sorted-byte-record-search.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-062-sanitized \
  examples/c/c-062-sorted-byte-record-search.c
./c-062-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt määrittelemään tavupuskurin sopimuksen, tunnistamaan hylätyn syötteen, selittämään tulosteen ja erottamaan opettavaisen eheys- tai lelu-hajautusesimerkin sopivasta tuotantokäytön kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinelliskannaus ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O:n, varauksen, jäsentämisen ja muotoilun tulokset on tarkistettava ennen kuin niiden tuloksia käytetään.
4. Vahvistettu ohjelma muuttaa ulostulotilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C:n tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "Ohje: C-pohjaisen työkaluketjun koventaminen — OWASP"
