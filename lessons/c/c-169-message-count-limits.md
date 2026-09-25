# C 169 — Viestimäärän rajoitukset

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–168
**Seuraava ohjelma:** [`examples/c/c-169-message-count-limits.c`](../../examples/c/c-169-message-count-limits.c)
**Deterministinen testi:** [`tests/c-169-message-count-limits.sh`](../../tests/c-169-message-count-limits.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin jälkeen osaat hylätä sekvenssin, joka ylittää dokumentoidun maksimimäärän tietueita.

## Muistin virkistys

1. Miksi raakabuffeilla täytyy olla eksplisiittinen `size_t`‑pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisen buffern sisällä?
3. Mitkä I/O-, allokointi- tai jäsennysrajapintojen paluuarvot täytyy tarkistaa, ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei‑kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Viestimäärän rajoitukset sitovat kokonaistyön ja tallennustilan, vaikka kukin yksittäinen viesti mahtuisi paikallisen kokorajoituksen sisään.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuita laskettuna datana, tarkistaa jokaisen osoittimen, laskelman, kapasiteetin, konversion ja paluuarvon, joihin esimerkki luottaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assemblyn suorittamista, prosessin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Läpikäyty esimerkki

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
    if (!accept_limited(&state, UINT8_C(169))) return EXIT_FAILURE;
    if (printf("C 169 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-169-message-count-limits \
  examples/c/c-169-message-count-limits.c
./c-169-message-count-limits
```

Odotettu tuloste:

```text
C 169 state byte: 169
```

## Ohjattu harjoitus

1. Tunnista jokaisen apurajapinnan kohdalla tavujen alue (byte span) tai tilasopimus.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, laskelman, kapasiteetin tai arvon.
3. Selitä, miksi tässä esimerkissä `0x00` on tavallinen data eikä lopetusmerkki.
4. Mainitse koska ulostuloarvo tai -positio saattaa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokointikoon laskemista. Älä muunna tätä opettavaista esimerkkiä kryptografiseksi, suoritettavien lataimeksi tai mielivaltaisen koodin ajomekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-169-message-count-limits.sh
```

Käännä ja aja sanitisaattoreilla, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-169-sanitized \
  examples/c/c-169-message-count-limits.c
./c-169-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit kertoa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavu‑API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri varmistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmaistujen rajojen läpikäyntiä; pieni esimerkki ei todista tuotantoturvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen sarjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
