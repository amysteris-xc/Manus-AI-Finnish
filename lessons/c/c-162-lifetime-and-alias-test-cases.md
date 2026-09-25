# C 162 — Lifetime and Alias Test Cases

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–161
**Seuraava ohjelma:** [`examples/c/c-162-lifetime-and-alias-test-cases.c`](../../examples/c/c-162-lifetime-and-alias-test-cases.c)
**Deterministinen testi:** [`tests/c-162-lifetime-and-alias-test-cases.sh`](../../tests/c-162-lifetime-and-alias-test-cases.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen sinun pitäisi pystyä tunnistamaan kelvollinen laina (borrow), kopioitu puskurialue ja virheellinen käyttö vapautuksen jälkeen ilman määrittelemätöntä käyttäytymistä.

## Muistin virkistys

1. Miksi raakabytetason puskurilla on oltava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsennys-API:en paluuarvot on tarkistettava ennen niiden tulosten luottamista?
4. Perustele, miksi ei-kryptografinen tarkastus-arvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Lifetime-oppitunnit dokumentoivat sääntöjä ja testaavat turvallisia sopimuksia; niiden ei tule demonstroida todellista use-after-free-tilannetta tai määrittelemätöntä käyttäytymistä.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavut lasketuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautusarvon johon esimerkki luottaa, ja säilyttää lähtötilan validointiin asti. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asennuskoodin suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(162)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 162 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-162-lifetime-and-alias-test-cases \
  examples/c/c-162-lifetime-and-alias-test-cases.c
./c-162-lifetime-and-alias-test-cases
```

Odotettu tuloste:

```text
C 162 validated byte: 162
```

## Ohjattu harjoitus

1. Tunnista tavuväli tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -paikka voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksellä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä tee tästä opetus-esimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-162-lifetime-and-alias-test-cases.sh
```

Käännä ja suorita sanitisaattoreilla, joissa tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-162-sanitized \
  examples/c/c-162-lifetime-and-alias-test-cases.c
./c-162-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavua-API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; yksikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
