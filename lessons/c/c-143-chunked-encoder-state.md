# C 143 — Chunked Encoder State

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–142
**Seuralaisohjelma:** [`examples/c/c-143-chunked-encoder-state.c`](../../examples/c/c-143-chunked-encoder-state.c)
**Deterministinen testi:** [`tests/c-143-chunked-encoder-state.sh`](../../tests/c-143-chunked-encoder-state.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat siirtää pienen enkooderistatuksen rajoitettujen syötekappaleiden läpi menettämättä osittaista yksikköä.

## Muistin virkistys

1. Miksi raakabuffereilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkeeseen perustuva ehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-APIen paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Striimaustila säilyttää vain dokumentoidun rajallisen jäännöksen. Jokainen kappale on käsiteltävä ilman oletusta siitä, että se päättyy loogiseen rajaan.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavut laskentatietona, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki perustuu, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, satunnaista assemblyn suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(143))) return EXIT_FAILURE;
    if (printf("C 143 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-143-chunked-encoder-state \
  examples/c/c-143-chunked-encoder-state.c
./c-143-chunked-encoder-state
```

Odotettu tuloste:

```text
C 143 state byte: 143
```

## Ohjattu harjoitus

1. Tunnista tavujoukko tai tilasopimus kunkin apufunktion rajalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty reunatapaus. Pidä määrät eksplisiittisinä tavumäärinä, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokoinnin koon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavan koodin lataimeksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-143-chunked-encoder-state.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-143-sanitized \
  examples/c/c-143-chunked-encoder-state.c
./c-143-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat ilmaista syötteen sopimuksen, tunnistaa hylätyn reunatapauksen, selittää ulostulon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministiset testit että julistettujen rajoitusten tarkastelun; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
