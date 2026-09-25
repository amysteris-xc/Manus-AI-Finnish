# C 176 — Taulukonohjattu jäsentäjätilakone

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–175
**Seuraava ohjelma:** [`examples/c/c-176-table-driven-parser-state-machine.c`](../../examples/c/c-176-table-driven-parser-state-machine.c)
**Deterministinen testi:** [`tests/c-176-table-driven-parser-state-machine.sh`](../../tests/c-176-table-driven-parser-state-machine.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopuksi sinun pitäisi pystyä kuvaamaan pieni jäsentäjän siirtymätaulukko ja validoidut tila- ja tavuluokat.

## Muistiharjoitus

1. Miksi raakabuffereilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämis-API:sta on tarkistettava ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Taulukko tekee siirtymistä tarkasteltavia, mutta jokainen taulun indeksi ja seuraava tila on tarkistettava sallitulla alueella.

Tämä oppitunti käyttää siirrettävää ISO C17 -koodia. Se käsittelee tavujoukkoa laskettuna datana, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja paluu­tuloksen, joihin esimerkki nojautuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessiin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(176))) return EXIT_FAILURE;
    if (printf("C 176 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-176-table-driven-parser-state-machine \
  examples/c/c-176-table-driven-parser-state-machine.c
./c-176-table-driven-parser-state-machine
```

Odotettu tulostus:

```text
C 176 state byte: 176
```

## Ohjattu harjoitus

1. Tunnista jokaisellä apurajapinnalla tavujen väli tai tilasopimus.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -paikka voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapauksen esimerkki. Käytä eksplisiittisiä tavumääriä, säilytä alkuperäinen lähtö hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokoinnin koon laskemista. Älä muunna tätä opetus­esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-176-table-driven-parser-state-machine.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-176-sanitized \
  examples/c/c-176-table-driven-parser-state-machine.c
./c-176-sanitized
```

## Lopullinen itse­tarkastus

Olet valmis seuraavaan oppituntiin, jos osaat kuvata syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitteiden tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
