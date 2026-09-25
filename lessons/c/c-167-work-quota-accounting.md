# C 167 — Työkiintiön kirjanpito

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–166
**Seuraava ohjelma:** [`examples/c/c-167-work-quota-accounting.c`](../../examples/c/c-167-work-quota-accounting.c)
**Deterministinen testi:** [`tests/c-167-work-quota-accounting.sh`](../../tests/c-167-work-quota-accounting.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat kirjata rajattuja työyksiköitä käsitellessäsi kontrolloitua tavujonoa.

## Hakukertaus

1. Miksi raakaa tavupuskuroa täytyy kantaa mukana eksplisiittisellä `size_t` pituudella?
2. Mikä silmukkapaluu ehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuloksia luotetaan?
4. Perustele, miksi kryptografisesti merkityksetön tarkistusluku ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Resurssien kirjaamisen tulee olla eksplisiittistä ja monotonaista. Hylätty syöte ei saa nollata tai ohittaa käytettyä kiintiötä.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavut laskettuina datoina, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki tukeutuu, ja säilyttää tuotetilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataimia, mielivaltaista assemblin suorittamista, prosessin injektiota, itseä muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!accept_limited(&state, UINT8_C(167))) return EXIT_FAILURE;
    if (printf("C 167 state byte: %u\n", (unsigned)state.accepted) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-167-work-quota-accounting \
  examples/c/c-167-work-quota-accounting.c
./c-167-work-quota-accounting
```

Odotettu tuloste:

```text
C 167 state byte: 167
```

## Ohjattu harjoitus

1. Tunnista tavuspektri tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi aikaisin se ehto, joka hylkää virheellisen osoitteen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua allokoinnin kokolaskelmaa. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-167-work-quota-accounting.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-167-sanitized \
  examples/c/c-167-work-quota-accounting.c
./c-167-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotannon turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
