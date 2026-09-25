# C 144 — Streaming-dekooderin tila

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–143
**Seuraava ohjelma:** [`examples/c/c-144-streaming-decoder-state.c`](../../examples/c/c-144-streaming-decoder-state.c)
**Deterministinen testi:** [`tests/c-144-streaming-decoder-state.sh`](../../tests/c-144-streaming-decoder-state.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata kantaa pieni dekooderitila lohkojen yli ja hylätä virheellinen lopullinen osittainen yksikkö.

## Muistiharjoitus

1. Miksi raakabufferin mukana on kannettava eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää eteenpäin liikkuvan indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämis-API:sta täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Virran lopun validointi on erillinen jokaisen lohkon validoinnista. Älä hyväksy jääneitä osittaisia tietoja täydellisenä arvona.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavujoukkoa laskettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja palautusarvon, mihin esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assembly-suoritusta, prosessiin injektiota, itseään muokkaavaa koodia tai aitoja kryptografisia toteutuksia — nämä ovat turvallisuusvaroituksia, jotka on säilytettävä täsmällisesti.

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
    if (!accept_limited(&state, UINT8_C(144))) return EXIT_FAILURE;
    if (printf("C 144 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-144-streaming-decoder-state \
  examples/c/c-144-streaming-decoder-state.c
./c-144-streaming-decoder-state
```

Odotettu tuloste:

```text
C 144 state byte: 144
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Löydä varhaisin ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Määrittele milloin lähtöarvo tai -positio voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokaatiosumman laskemista. Älä tee tästä opetus-esimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suoritusta mahdollistavaa mekanismia.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-144-streaming-decoder-state.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-144-sanitized \
  examples/c/c-144-streaming-decoder-state.c
./c-144-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määrittää syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa defensiivisen tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausvinkki: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
