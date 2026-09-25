# C 166 — Token Budget Parser Limit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–165
**Seuraava ohjelma:** [`examples/c/c-166-token-budget-parser-limit.c`](../../examples/c/c-166-token-budget-parser-limit.c)
**Deterministinen testi:** [`tests/c-166-token-budget-parser-limit.sh`](../../tests/c-166-token-budget-parser-limit.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi osaat pakottaa pieneen maksimimäärään perustuvan token- tai kenttien määrän rajoituksen parser-tyyppisessä silmukassa.

## Muistiharjoitus

1. Miksi raakabufferin tulee kantaa mukana eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsentämis-API:en paluuarvot täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Lukumääräraja rajaa tehtävää myös silloin, kun jokainen kenttä on lyhyt. Hylkää uudet kentät sen jälkeen, kun määritetty budjetti on käytetty.

Tämä oppitunti käyttää kannettavaa ISO C17 -koodia. Se käsittelee tavuja laskettavana datana, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki nojautuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaisen assemblerin ajamista, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(166))) return EXIT_FAILURE;
    if (printf("C 166 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-166-token-budget-parser-limit \
  examples/c/c-166-token-budget-parser-limit.c
./c-166-token-budget-parser-limit
```

Odotettu tuloste:

```text
C 166 state byte: 166
```

## Ohjattu harjoitus

1. Tunnista tavualue tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä miksi `0x00` on tavallista dataa eikä päätteen merkitsevä loppumerkki tässä esimerkissä.
4. Ilmoita milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen validi tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun varauskoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mekanismiksi mielivaltaisen koodin suorittamiseen.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-166-token-budget-parser-limit.sh
```

Käännä ja aja sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-166-sanitized \
  examples/c/c-166-token-budget-parser-limit.c
./c-166-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat kertoa tulosopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministiset testit että ilmoitettujen rajojen tarkastelun; mikään pieni esimerkki ei todista tuotantoturvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle osiolle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
