# C 137 — Escaperoitu tekstitavun jäsentäjä

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–136
**Seurantaohjelma:** [`examples/c/c-137-escaped-text-byte-parser.c`](../../examples/c/c-137-escaped-text-byte-parser.c)
**Deterministinen testi:** [`tests/c-137-escaped-text-byte-parser.sh`](../../tests/c-137-escaped-text-byte-parser.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin lopussa osaat jäsentää rajatun escaperoidun tekstimäisen tavujoukon käyttämällä eksplisiittistä tilaa ja rajoja.

## Hakuharjoitus

1. Miksi raaka tavupusku täytyy kantaa eksplisiittisellä `size_t` pituudella?
2. Mikä silmukan ehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämis-API:sta on tarkistettava ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Escape-käsittely on tilakone. Hylkää roikkuva escape ja pysäytä ennen kuin kulutat yli annetun määrän.

Tämä opetus käyttää kannatettavaa ISO C17:ää. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautusarvon, joihin esimerkki luottaa, ja säilyttää tuotosvaltion kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t accepted; size_t count; } ParseState;

static int accept_limited(ParseState *state, uint8_t byte)
{
    if (state == NULL || state->count >= 3 || byte == 0) return 0;
    state->accepted = byte;
    state->count++;
    return 1;
}

int main(void)
{
    ParseState state = {0, 0};
    if (!accept_limited(&state, UINT8_C(137))) return EXIT_FAILURE;
    if (printf("C 137 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-137-escaped-text-byte-parser \
  examples/c/c-137-escaped-text-byte-parser.c
./c-137-escaped-text-byte-parser
```

Odotettu tuloste:

```text
C 137 state byte: 137
```

## Ohjatun harjoittelun tehtävät

1. Tunnista tavuspanne tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin oleva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema saattaa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavulukumäärät, säilytä alkuperäinen tulos hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaation koon laskemista. Älä tee tästä opetus-esimerkistä kryptografista, suoritettavan lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-137-escaped-text-byte-parser.sh
```

Käännä ja suorita sanitisaattoreilla, kun tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-137-sanitized \
  examples/c/c-137-escaped-text-byte-parser.c
./c-137-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat ilmoittaa syötetason sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevässä läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Russian-kielelle (venäjän suunta).

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
