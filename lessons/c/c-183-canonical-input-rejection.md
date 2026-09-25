# C 183 — Canonical Input Rejection

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–182
**Seurantaohjelma:** [`examples/c/c-183-canonical-input-rejection.c`](../../examples/c/c-183-canonical-input-rejection.c)
**Deterministinen testi:** [`tests/c-183-canonical-input-rejection.sh`](../../tests/c-183-canonical-input-rejection.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopussa osaat hylätä ei-kanonisen esitysmuodon, kun formaatti edellyttää yhtä yksikäsitteistä muotoa.

## Palautusharjoitus

1. Miksi raaka tavutaulukko (raw byte buffer) pitää kantaa mukanaan eksplisiittistä `size_t` pituutta?
2. Mikä silmukan ehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsennys-API:sta on tarkistettava ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Kanonaalinen validointi estää saman arvon useita tavuesityksiä. Se on formaattisääntö, ei korvaus autentikoinnille.

Tämä oppitunti käyttää siirrettävää ISO C17 -kieltä. Se käsittelee tavujoukkoa laskettavana datana, tarkistaa kaikki osoittimet, lukumäärät, kapasiteetit, muunnokset ja esimerkin riippuvuudet palautusarvoissa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(183)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 183 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-183-canonical-input-rejection \
  examples/c/c-183-canonical-input-rejection.c
./c-183-canonical-input-rejection
```

Odotettu tuloste:

```text
C 183 validated byte: 183
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi aikaisin toteutuva ehto, joka hylkää virheellisen osoitteen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` tässä esimerkissä on tavallinen data eikä loppumerkki.
4. Mainitse milloin lähtöarvo tai -sijainti saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä lukujen tavamäärät eksplisiittisinä, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaation kokolaskennan suorittamista. Älä tee tästä opetusesimerkistä kryptografista, suoritettavaa lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-183-canonical-input-rejection.sh
```

Käännä ja aja sanitisaattoreilla, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-183-sanitized \
  examples/c/c-183-canonical-input-rejection.c
./c-183-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määrittää syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Juonipaljastus: vastaukset ja mallivastaus

1. Laskettu tavua-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen lukemista `data[index]`.
3. Apuri validoi ennen kuin se muuttaa lähtöparametria, kursoria tai tila-kenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
