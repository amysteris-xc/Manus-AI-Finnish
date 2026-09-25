# C 119 — Pakollisten kenttien täydellisyys

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–118
**Seuralainen ohjelma:** [`examples/c/c-119-required-field-completeness.c`](../../examples/c/c-119-required-field-completeness.c)
**Deterministinen testi:** [`tests/c-119-required-field-completeness.sh`](../../tests/c-119-required-field-completeness.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi pystyä varmistamaan, että kaikki pakolliset kentät olivat läsnä ennen kuin hyväksyt jäsennellyn tietueen.

## Kertauskysymykset

1. Miksi raaka tavumuisti (raw byte buffer) pitää kantaa mukana eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, varaus- tai jäsentämis-API:sta pitää tarkistaa ennen kuin niiden tuottamiin tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistus-arvo ei ole todennus aktiivista hyökkääjää vastaan.

## Keskeinen ajatus

Onnistunut syötteen loppu ei riitä: tietue on kelvollinen vain sen jälkeen, kun jokainen pakollinen kenttä on havaittu ja validoitu.

Tämä oppitunti käyttää kannatettavaa ISO C17 -koodia. Se käsittelee tavujoukkoa laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suoritusta, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(119))) return EXIT_FAILURE;
    if (printf("C 119 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-119-required-field-completeness \
  examples/c/c-119-required-field-completeness.c
./c-119-required-field-completeness
```

Odotettu tuloste:

```text
C 119 state byte: 119
```

## Ohjattu harjoittelu

1. Tunnista tavujakso tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavien lataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-119-required-field-completeness.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-119-sanitized \
  examples/c/c-119-required-field-completeness.c
./c-119-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavuja (zero bytes) ovat kelvollista dataa.
2. Etenemissilmukka tarkistaa `index < count` ennen kuin se lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
