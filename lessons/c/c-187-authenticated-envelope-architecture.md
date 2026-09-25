# C 187 — Authenticated Envelope Architecture

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–186
**Seurantoprogrammi:** [`examples/c/c-187-authenticated-envelope-architecture.c`](../../examples/c/c-187-authenticated-envelope-architecture.c)
**Deterministinen testi:** [`tests/c-187-authenticated-envelope-architecture.sh`](../../tests/c-187-authenticated-envelope-architecture.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi pystyä hahmottamaan kapea adapterirajapinta autentikoidulle kirjekuorelle ilman kryptografian toteuttamista.

## Palautusharjoitus

1. Miksi raakabitintäyte (raw byte buffer) täytyy kantaa mukana eksplisiittisellä `size_t` pituudella?
2. Mikä silmukkaehto pitää etenevän indeksen `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsennys-API:sta on tarkistettava ennen kuin niiden tuotoksia luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastusluku ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tuotantoon tarkoitettu salaus ja autentikointi kuuluvat vakiintuneille kirjastoille ja korkeantason API:ille. Tämä oppitunti ei luo mitään kryptografista rakennetta tai avaintyönkulkua.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, lukeman, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suoritusta, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

## Käytännön esimerkki

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
    const uint8_t input[] = {UINT8_C(187)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 187 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-187-authenticated-envelope-architecture \
  examples/c/c-187-authenticated-envelope-architecture.c
./c-187-authenticated-envelope-architecture
```

Odotettu tuloste:

```text
C 187 validated byte: 187
```

## Ohjattu harjoittelu

1. Tunnista tavuille ulottuma tai tilasopimus kussakin apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukeman, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty reunatapaus. Pidä eksplisiittiset tavulaskelmat, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen kuin muutat mitään allokoinnin kokolaskelmaa. Älä tee tästä opetusesimerkistä kryptografista, suoritettavaa lataajaa tai mielivaltaisen koodin suoritussysteemiä.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-187-authenticated-envelope-architecture.sh
```

Käännä ja suorita sanitizerien kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-187-sanitized \
  examples/c/c-187-authenticated-envelope-architecture.c
./c-187-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tulosopimuksen, tunnistaa hylätyn reunatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta tuotantotason kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavujen API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa ulostulotason parametriä, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotannon turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
