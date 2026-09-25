# C 136 — Erotinetsintä ilman C-merkkijono-oletusta

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–135
**Seuralaisohjelma:** [`examples/c/c-136-delimiter-without-c-string-assumption.c`](../../examples/c/c-136-delimiter-without-c-string-assumption.c)
**Deterministinen testi:** [`tests/c-136-delimiter-without-c-string-assumption.sh`](../../tests/c-136-delimiter-without-c-string-assumption.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi osaat etsiä laskennallisesta tavutaulukosta erotinta siten, että sisäkkäiset nollatavut säilyvät.

## Muistiharjoitus

1. Miksi raakalla tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsennysrajapintojen paluuarvot on tarkistettava ennen niiden tulosten luottamista?
4. Miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Erotinetsintä rajoittuu eksplisiittiseen laskuriin. Se ei saa kutsua merkkijonofunktioita mielivaltaiselle tavudatalle.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, pituuden, kapasiteetin, muunnoksen ja esimerkin varaan laskettavan palautusarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assembly-suoritusta, prosessin injektointia, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_span(const uint8_t *source, size_t count, uint8_t *destination, size_t capacity)
{
    if ((source == NULL && count != 0) || destination == NULL || count > capacity) return 0;
    if (count != 0) memmove(destination, source, count);
    return 1;
}

int main(void)
{
    const uint8_t source[] = {UINT8_C(136), UINT8_C(0)};
    uint8_t destination[2] = {0};
    if (!copy_span(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 136 copied byte: %u\n", (unsigned)destination[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-136-delimiter-without-c-string-assumption \
  examples/c/c-136-delimiter-without-c-string-assumption.c
./c-136-delimiter-without-c-string-assumption
```

Odotettu tuloste:

```text
C 136 copied byte: 136
```

## Ohjattu harjoittelu

1. Tunnista tavujakso tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, pituuden, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele, milloin lähtöarvo tai -sijainti saa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, pidä alkuperäinen lähtö muuttumattomana hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaatiokoon laskemista. Älä tee tästä opetus­tarkoituksen esimerkistä kryptografista, suoritettavan lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-136-delimiter-without-c-string-assumption.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-136-sanitized \
  examples/c/c-136-delimiter-without-c-string-assumption.c
./c-136-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausvinkit ja malli­vastaus

1. Laskettu tavurajapinta käyttää osoitinta ja eksplisiittistä pituutta, koska sisäkkäiset nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
