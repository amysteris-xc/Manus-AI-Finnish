# C 180 — Merkitty yhdiste: validointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–179
**Seurantaohjelma:** [`examples/c/c-180-tagged-union-validation.c`](../../examples/c/c-180-tagged-union-validation.c)
**Deterministinen testi:** [`tests/c-180-tagged-union-validation.sh`](../../tests/c-180-tagged-union-validation.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa osaat validoida tunnuksen ennen pienen merkittyyn yhdisteeseen kuuluvan hyötykuorman tulkintaa.

## Muistin virkistys

1. Miksi raakabuffereissa on kannettava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen bufferin sisällä?
3. Mitkä paluuarvot I/O-, varaus- tai jäsentämisrajapinnoista on tarkastettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi kryptografisesti epäkelpo tarkastus-arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tunnus valitsee yhden sallitun hyötykuorman muodoista. Hylkää epäsopiva tunnus, pituus tai arvo ennen kuin altistat ulostulotilan.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavujoukkoa laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää ulostulotilan siihen asti kun validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(180))) return EXIT_FAILURE;
    if (printf("C 180 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-180-tagged-union-validation \
  examples/c/c-180-tagged-union-validation.c
./c-180-tagged-union-validation
```

Odotettu tuloste:

```text
C 180 state byte: 180
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin hylkäävä ehto, joka torjuu virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro milloin ulostuloarvo tai -sijainti saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkistettu aritmetiikkaehto ennen minkään muutetun varauskoon laskemista. Älä muutakaan tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-180-tagged-union-validation.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-180-sanitized \
  examples/c/c-180-tagged-union-validation.c
./c-180-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat kertoa syötetilan sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojasta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketulla tavurajapinnalla käytetään osoitinta ja eksplisiittistä pituutta, koska upotetut nolla­tavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; yksikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
