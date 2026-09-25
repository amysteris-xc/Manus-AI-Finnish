# C 159 — Metamorfinen tavumuunnostestaus

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–158
**Seuraava esimerkkiohjelma:** [`examples/c/c-159-metamorphic-byte-transform-tests.c`](../../examples/c/c-159-metamorphic-byte-transform-tests.c)
**Deterministinen testi:** [`tests/c-159-metamorphic-byte-transform-tests.sh`](../../tests/c-159-metamorphic-byte-transform-tests.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat testata suhdetta syötteen ja muunnetun syötteen välillä ilman, että jokaista tapausta varten tarvitaan erillistä totuusarvo-”orakkelia”.

## Muistiharjoitus

1. Miksi raakana oleva tavubufferi pitää kantaa mukana eksplisiittisen `size_t` pituuden kanssa?
2. Mikä silmukan ehto pitää eteenpäin etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, varaus- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastus­arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Metamorfinen relaatio pitää dokumentoida ja sille pitää asettaa rajat. Se täydentää, muttei korvaa, tunnettuja vastaustestejä.

Tämä oppitunti käyttää kannatettavaa ISO C17 -kieltä. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautusarvon johon esimerkki nojaa, ja säilyttää ulostulotilan validointiin asti. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin ajamista, prosessi-injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one(const uint8_t *data, size_t count, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (count != 1 || data[0] == 0) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(159)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 159 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-159-metamorphic-byte-transform-tests \
  examples/c/c-159-metamorphic-byte-transform-tests.c
./c-159-metamorphic-byte-transform-tests
```

Odotettu tuloste:

```text
C 159 validated byte: 159
```

## Ohjattu harjoitus

1. Tunnista tavujen alue (byte span) tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele, milloin ulostuloarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskentaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritin­lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-159-metamorphic-byte-transform-tests.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-159-sanitized \
  examples/c/c-159-metamorphic-byte-transform-tests.c
./c-159-sanitized
```

## Lopullinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausspoileri: vastaukset ja malli­vastaus

1. Laskettu tavu-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametrin, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavassa aikataulutetussa oppitunnossa palataan venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
