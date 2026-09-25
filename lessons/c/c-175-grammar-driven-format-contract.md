# C 175 — Kielen ohjaama formaattisopimus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–174
**Seuraava ohjelma:** [`examples/c/c-175-grammar-driven-format-contract.c`](../../examples/c/c-175-grammar-driven-format-contract.c)
**Deterministinen testi:** [`tests/c-175-grammar-driven-format-contract.sh`](../../tests/c-175-grammar-driven-format-contract.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän tunnin lopussa osaat kuvata tiiviin tietuegrammatiikan ja validoida yhden sen yksinkertaisen tuotannon.

## Muistiharjoitus

1. Miksi raakabuffern tulee kantaa eksplisiittistä `size_t` pituutta?
2. Mikä silmukan ehto pitää eteenpäin etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä palautusarvot I/O-, allokointi- tai jäsennys-API:sta on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Grammatiikka ilmaisee sallitun järjestyksen, kentät ja toistot. Validoinnin täytyy silti tarkistaa todelliset tavumäärät ja rajat.

Tässä oppitunnissa käytetään kannettavaa ISO C17:ää. Se käsittelee tavuja laskettavana datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautustuloksen, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessin injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Esimerkin läpikäynti

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { const uint8_t *data; size_t count; size_t position; } Reader;

static int reader_take(Reader *reader, uint8_t *out)
{
    if (reader == NULL || out == NULL || reader->data == NULL || reader->position >= reader->count) return 0;
    *out = reader->data[reader->position];
    reader->position++;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(175), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 175 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-175-grammar-driven-format-contract \
  examples/c/c-175-grammar-driven-format-contract.c
./c-175-grammar-driven-format-contract
```

Odotettu tuloste:

```text
C 175 reader byte: 175
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi aikaisin sattuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä tunnin ilmoitettu turvallisuusrajoitus yhdessä tarkassa lauseessa.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen mitään muutettavaa allokointikokolaskelmaa. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-175-grammar-driven-format-contract.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-175-sanitized \
  examples/c/c-175-grammar-driven-format-contract.c
./c-175-sanitized
```

## Loppuväitön tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat kertoa syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavukäsittelyn oikean maailman kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavupääte käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoii ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Russian-ketjuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
