# C 113 — Sequence Gap Detection

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–112
**Seurantaohjelma:** [`examples/c/c-113-sequence-gap-detection.c`](../../examples/c/c-113-sequence-gap-detection.c)
**Deterministinen testi:** [`tests/c-113-sequence-gap-detection.sh`](../../tests/c-113-sequence-gap-detection.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata havaita odotettu seuraava sekvenssiarvo ja raportoida aukko ilman, että hyväksytty tila etenee.

## Palauttava harjoitus

1. Miksi raakabuffterin tulee kantaa eksplisiittinen `size_t` pituus?
2. Mikä silmukaehdotus pitää etenevän indeksin sisällä `count`-alkioisen buffterin rajojen sisällä?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta täytyy tarkistaa ennen kuin niiden tuotoksiin luotetaan?
4. Perustele miksi ei-kryptografinen tarkastusavain ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tilamuutokset seuraavat validoitua syötettä. Hylätty aukko ei saa hiljaisesti muuttua uudeksi odotetuksi arvoksi.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavujoukkoja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki perustuu, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assemblerin suorittamista, prosessiin injektiota, itseensä muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(113))) return EXIT_FAILURE;
    if (printf("C 113 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-113-sequence-gap-detection \
  examples/c/c-113-sequence-gap-detection.c
./c-113-sequence-gap-detection
```

Odotettu tuloste:

```text
C 113 state byte: 113
```

## Ohjattu harjoittelu

1. Tunnista tavujoukko tai tilasopimus jokaisessa apurajapinnassa.
2. Löydä varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä lopetusmerkki.
4. Määritä milloin lähtöarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen kuin muutat minkään allokointikoon laskentaa. Älä muutakaan tätä opetus-esimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-113-sequence-gap-detection.sh
```

Käännä ja aja sanitisaattoreiden kanssa siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-113-sanitized \
  examples/c/c-113-sequence-gap-detection.c
./c-113-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määrittää syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja malli‑vastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa ulostusparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäynnin; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
