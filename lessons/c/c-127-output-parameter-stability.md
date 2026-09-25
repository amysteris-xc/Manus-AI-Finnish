# C 127 — Output Parameter Stability

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–126
**Seurantoprogrammi:** [`examples/c/c-127-output-parameter-stability.c`](../../examples/c/c-127-output-parameter-stability.c)
**Deterministinen testi:** [`tests/c-127-output-parameter-stability.sh`](../../tests/c-127-output-parameter-stability.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat jättää lähtöparametrin muuttumattomaksi, kun tulos edellyttää, että syötesopimus epäonnistuu.

## Muistiharjoitus

1. Miksi raakabittipuskurilla pitää olla eksplisiittinen `size_t` pituus?
2. Mikä silmukka­ehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsentämis-API:sta palautuvat arvot on tarkistettava ennen kuin niiden lähtöjä luotetaan?
4. Miksi ei-kryptografinen tarkistus­arvo ei ole autentikointia aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Vakaa lähtötila tekee virhetilanteista turvallisempia kutsua. Vahvista syötteet ennen kuin kirjoitat uuden tuloksen lähtöpaikkaan.

Tämä oppitunti käyttää kannettavaa ISO C17 -koodia. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, konversion ja palautusarvon, johon esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai aitoja kryptografisia toteutuksia.

## Käytännön esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one(const uint8_t *data, size_t count, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (count != 1 || data[0] == 0) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(127)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 127 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-127-output-parameter-stability \
  examples/c/c-127-output-parameter-stability.c
./c-127-output-parameter-stability
```

Odotettu tuloste:

```text
C 127 validated byte: 127
```

## Ohjattu harjoitus

1. Tunnista tavumääreen tai tilasopimuksen laajuus kunkin apurin rajapinnassa.
2. Löydä aikaisin mahdollinen ehto, joka hylkää kelvottoman osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun varauksen koon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suorituksen mekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-127-output-parameter-stability.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-127-sanitized \
  examples/c/c-127-output-parameter-stability.c
./c-127-sanitized
```

## Lopullinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja malli­vastaus

1. Laskettu tavu-API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen lähtöparametrin, kursorin tai tilakentän muuttamista.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi opetuksessa palataan venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
