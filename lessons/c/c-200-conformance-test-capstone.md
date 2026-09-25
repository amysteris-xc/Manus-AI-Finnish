# C 200 — Conformance Test Capstone

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–199
**Seuraava ohjelma:** [`examples/c/c-200-conformance-test-capstone.c`](../../examples/c/c-200-conformance-test-capstone.c)
**Deterministinen testi:** [`tests/c-200-conformance-test-capstone.sh`](../../tests/c-200-conformance-test-capstone.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopuksi sinun pitäisi osata ajaa kompakti joukko tunnettuja kelvollisia ja tunnettuja virheellisiä tavujoukkoja läpi pareittain toimivien lukija- ja kirjoittaja-apureiden.

## Palautusharjoitus

1. Miksi raakabuffern tulee kantaa mukana eksplisiittistä `size_t` pituutta?
2. Mikä silmallinen ehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan — perustele.

## Keskeinen käsite

Conformance-testit yhdistävät dokumentoidut formaattisäännöt deterministisiin tapauksiin. Ne täydentävät koodikatselmuksia ja sanitisaattorikäyttöjä sen sijaan, että korvaisivat niitä.

Tässä oppitunnissa käytetään kannateltavaa ISO C17:ää. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki nojaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivia koodilataajaa, mielivaltaista assemblerin suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Ratkaistu esimerkki

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
    const uint8_t input[] = {UINT8_C(200)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 200 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-200-conformance-test-capstone \
  examples/c/c-200-conformance-test-capstone.c
./c-200-conformance-test-capstone
```

Odotettu tuloste:

```text
C 200 validated byte: 200
```

## Ohjattu harjoittelu

1. Tunnista tavujänne tai tilasopimus kunkin apurin rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin ulostuloarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavulaskelmat, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokointikoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritettavaa lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-200-conformance-test-capstone.sh
```

Käännä ja aja sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-200-sanitized \
  examples/c/c-200-conformance-test-capstone.c
./c-200-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat ilmoittaa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavuohjauksen todellisesta tuotantokäytön kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tava-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Aputoiminto tarkistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotannon turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
