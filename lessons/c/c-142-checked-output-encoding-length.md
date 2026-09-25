# C 142 — Tarkistettu lähtöenkoodauksen pituus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–141
**Seurantaohjelma:** [`examples/c/c-142-checked-output-encoding-length.c`](../../examples/c/c-142-checked-output-encoding-length.c)
**Deterministinen testi:** [`tests/c-142-checked-output-encoding-length.sh`](../../tests/c-142-checked-output-encoding-length.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat laskea rajatun enkoodatun lähtöarvon pituuden käyttäen tarkistettua yhteen- ja kertolaskua.

## Palautusharjoitus

1. Miksi raakabittipuskurilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O:n, alokoinnin tai jäsennys-API:en paluuarvot täytyy tarkistaa ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Ydinajatus

Enkoodaus laajentaa dataa. Todista, että tulos mahtuu sekä `size_t` että konfiguroidun lähtöenimmäisvaatimuksen sisään ennen alokointia tai kirjoitusta.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee bittejä laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkin riippuvuuden paluu-arvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessiin injektiota, itseään muuttavaa koodia tai oikeita kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int checked_count_bytes(size_t count, size_t item_size, size_t *out)
{
    if (out == NULL || item_size == 0 || count > SIZE_MAX / item_size) return 0;
    *out = count * item_size;
    return 1;
}

int main(void)
{
    size_t bytes = 0;
    if (!checked_count_bytes((size_t)142, sizeof(uint8_t), &bytes)) return EXIT_FAILURE;
    if (printf("C 142 checked bytes: %zu\n", bytes) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-142-checked-output-encoding-length \
  examples/c/c-142-checked-output-encoding-length.c
./c-142-checked-output-encoding-length
```

Odotettu tuloste:

```text
C 142 checked bytes: 142
```

## Ohjattu harjoittelu

1. Tunnista tavumäärä tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä lopetusmerkki.
4. Ilmoita, milloin lähtöarvo tai -paikka voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun alokointikoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-142-checked-output-encoding-length.sh
```

Käännä ja suorita sanitisaattoreilla missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-142-sanitized \
  examples/c/c-142-checked-output-encoding-length.c
./c-142-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn oikean maailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavuliitäntä käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
