# C 118 — Duplicate Field Rejection

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–117
**Seuraava ohjelma:** [`examples/c/c-118-duplicate-field-rejection.c`](../../examples/c/c-118-duplicate-field-rejection.c)
**Deterministinen testi:** [`tests/c-118-duplicate-field-rejection.sh`](../../tests/c-118-duplicate-field-rejection.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata havaita vaaditun kentän toistuminen pienessä tagatussa tietueessa ja hylätä toinen esiintymä.

## Muistin virkistys

1. Miksi raaka tavutaulukko täytyy kuljettaa mukana eksplisiittisen `size_t` pituuden kanssa?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Kun formaatti sallii kentän vain kerran, merkitse kun se havaitaan ja hylkää toinen esiintymä ennen aiemman tilan ylikirjoittamista.

Tämä oppitunti käyttää kannatettavaa ISO C17 -kieltä. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki perustuu, ja säilyttää ulostilaa kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessiin injisointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(118))) return EXIT_FAILURE;
    if (printf("C 118 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-118-duplicate-field-rejection \
  examples/c/c-118-duplicate-field-rejection.c
./c-118-duplicate-field-rejection
```

Odotettu tuloste:

```text
C 118 state byte: 118
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilaehdotus kunkin apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa, ei loppumerkkiä.
4. Ilmoita milloin ulostusarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdessä täsmällisessä lauseessa.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty reunatapaus. Pidä eksplisiittiset tavulukumäärät, säilytä alkuperäinen ulostulo hylkäystilanteessa ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokointikoon laskemista. Älä muuta tätä opetus-esimerkkiä kryptografiseksi, suoritettavan koodin lataajaksi tai mielivaltaisen koodin suoritustavaksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-118-duplicate-field-rejection.sh
```

Käännä ja aja sanitisaattoreilla aina kun se on tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-118-sanitized \
  examples/c/c-118-duplicate-field-rejection.c
./c-118-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syöterajapinnan, tunnistaa hylätyn reunatapauksen, selittää ulostulon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostoparametria, kursoria tai tila-kenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
