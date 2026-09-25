# C 168 — Sisäkkäisyyden maksimisyvyys

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–167
**Seuralaisohjelma:** [`examples/c/c-168-nesting-depth-caps.c`](../../examples/c/c-168-nesting-depth-caps.c)
**Deterministinen testi:** [`tests/c-168-nesting-depth-caps.sh`](../../tests/c-168-nesting-depth-caps.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Oppitunnin lopussa sinun pitäisi pystyä pakottamaan pieni maksimisäätö sisäkkäisyystasolle demonstraatioanalyysaattorin tilassa.

## Muistinvirkistys

1. Miksi raakabittipuskuriin on liitettävä eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin indeksoivan osoittimen `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokaatio- tai jäsentämis-API:sta palautuvat arvot on tarkistettava ennen kuin niiden tuottamiin ulostuloihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastus-arvo ei ole todistus aitoudesta aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Syvyys on resurssirajoite. Inkrementoi vain validoiduissa avaustapauksissa, dekrementoi vain vastaavissa suljetapauksissa ja hylkää ylitysrajat.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, laskelman, kapasiteetin, muunnoksen ja palautusarvon, mihin esimerkki perustuu, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataimia, mielivaltaista assembler-suoritusta, prosessiin injektointia, itseä muokkaavaa koodia eikä todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(168))) return EXIT_FAILURE;
    if (printf("C 168 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-168-nesting-depth-caps \
  examples/c/c-168-nesting-depth-caps.c
./c-168-nesting-depth-caps
```

Odotettu tuloste:

```text
C 168 state byte: 168
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi aikaisin toteutuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Määrittele milloin ulostuloarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataus- tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-168-nesting-depth-caps.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-168-sanitized \
  examples/c/c-168-nesting-depth-caps.c
./c-168-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat kuvata syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta maailmasta tulevasta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Russian-kielelle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
