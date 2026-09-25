# C 152 — Simuloitu aikakatkaisubudjetti

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esitiedot:** C 001–151
**Seuralainen ohjelma:** [`examples/c/c-152-simulated-timeout-budget.c`](../../examples/c/c-152-simulated-timeout-budget.c)
**Deterministinen testi:** [`tests/c-152-simulated-timeout-budget.sh`](../../tests/c-152-simulated-timeout-budget.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat vähentää rajattua operaatiobudjettia deterministisesti ja hylätä työn, kun budjetti on kulutettu loppuun.

## Muistiharjoitus

1. Miksi raakabufferrin on kannettava nimenomainen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin liikkuvan indeksin `count`-alkioisen bufferin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:iden paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Paikallinen työbudjetti on opetusväline rajoille. Se ei ole verkkoyhteyden aikakatkaisu eikä korvaa järjestelmäresurssien hallintaa.

Tämä oppitunti käyttää kannatettavaa ISO C17 -kieltä. Se käsittelee tavujoukkoa laskettavana datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluu­tuloksen, johon esimerkki nojaa, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suoritusta, prosessien injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(152))) return EXIT_FAILURE;
    if (printf("C 152 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-152-simulated-timeout-budget \
  examples/c/c-152-simulated-timeout-budget.c
./c-152-simulated-timeout-budget
```

Odotettu tulostus:

```text
C 152 state byte: 152
```

## Ohjattu harjoitus

1. Tunnista tavujono tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä lopetusmerkki.
4. Määrittele, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettuun allokoinnin kokoon liittyvää laskentaa. Älä tee tästä opetus­esimerkistä kryptografista, suoritettavien lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-152-simulated-timeout-budget.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-152-sanitized \
  examples/c/c-152-simulated-timeout-budget.c
./c-152-sanitized
```

## Lopullinen itse­tarkastus

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavun API käyttää osoitinta ja nimenomaista pituutta, koska upotetut nollatavut voivat olla kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostulo­parametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palautuu venäjänkieliseen sarjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
