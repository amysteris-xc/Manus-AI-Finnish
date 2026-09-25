# C 112 — Lajitellun indeksin validointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–111
**Seurantaohjelma:** [`examples/c/c-112-sorted-index-validation.c`](../../examples/c/c-112-sorted-index-validation.c)
**Deterministinen testi:** [`tests/c-112-sorted-index-validation.sh`](../../tests/c-112-sorted-index-validation.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi pystyä validoimaan pieni lajiteltu indeksi ennen kuin luotat siihen hakutoiminnoissa.

## Muistiharjoitus

1. Miksi raakabaittimuistin mukana täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamia arvoja pidetään luotettavina?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Indeksisopimus sisältää lajitellun järjestyksen, määrän ja alueen kelvollisuuden. Epäonnistunut validointi ei saa muuttaa lähtöarvoa.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee bittejä lasketuilla datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assemblin suoritusta, prosessiin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_byte(const uint8_t *data, size_t count, uint8_t target, size_t *out_index)
{
    if (data == NULL || out_index == NULL) return 0;
    for (size_t i = 0; i < count; ++i) {
        if (data[i] == target) { *out_index = i; return 1; }
    }
    return 0;
}

int main(void)
{
    const uint8_t table[] = {UINT8_C(1), UINT8_C(112), UINT8_C(250)};
    size_t index = 0;
    if (!find_byte(table, sizeof table, UINT8_C(112), &index)) return EXIT_FAILURE;
    if (printf("C 112 table index: %zu\n", index) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-112-sorted-index-validation \
  examples/c/c-112-sorted-index-validation.c
./c-112-sorted-index-validation
```

Odotettu tuloste:

```text
C 112 table index: 1
```

## Ohjattu harjoitus

1. Tunnista tavun alue tai tilasopimus kussakin apurajapinnassa.
2. Etsi aikaisin ilmenevä ehto, joka hylkää kelvottoman osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele, milloin lähtöarvo tai -paikka voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä muunna tätä opettavaista esimerkkiä kryptografiseksi, suoritettavia lataavaksi tai mielivaltaisen koodin suorittamista mahdollistavaksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-112-sorted-index-validation.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-112-sanitized \
  examples/c/c-112-sorted-index-validation.c
./c-112-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos pystyt määrittelemään syötteen sopimuksen, tunnistamaan hylätyn rajatapauksen, selittämään lähtötuloksen ja erottamaan puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketuilla tavuilla varustettu API käyttää osoitinta ja eksplisiittistä pituutta, koska kiedotut nollabytet voivat olla kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa lähtöparametria, osoitinta tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että määriteltyjen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
