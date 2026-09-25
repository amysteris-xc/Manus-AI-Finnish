# C 107 — Rajoitetun kasvun politiikka

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–106
**Seurantaohjelma:** [`examples/c/c-107-bounded-growth-policy.c`](../../examples/c/c-107-bounded-growth-policy.c)
**Deterministinen testi:** [`tests/c-107-bounded-growth-policy.sh`](../../tests/c-107-bounded-growth-policy.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata soveltaa maksimikapasiteettia ja tarkistaa aritmetiikka ennen tavubufferin kasvattamista.

## Kertaus

1. Miksi raakalla tavubufferilla pitää olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkausehto pitää etenevän indeksin sisällä `count`-alkioisessa bufferissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuottamia arvoja luotetaan?
4. Perustele, miksi ei-kriptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Kasvatussääntöjen täytyy tarkistaa sekä aritmetiikan ylivuoto että tahallinen resurssikatto ennen allokaatiota.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavallisia tavuja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkissä käytetyn palautusarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessin injektointia, itse-modifioituvaa koodia eikä oikeita kryptografisia toteutuksia.

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
    if (!checked_count_bytes((size_t)107, sizeof(uint8_t), &bytes)) return EXIT_FAILURE;
    if (printf("C 107 checked bytes: %zu\n", bytes) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-107-bounded-growth-policy \
  examples/c/c-107-bounded-growth-policy.c
./c-107-bounded-growth-policy
```

Odotettu tuloste:

```text
C 107 checked bytes: 107
```

## Ohjattu harjoittelu

1. Tunnista tavujono tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi aikaisin mahdollinen ehto, joka hylkää epäkelvon osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Käytä eksplisiittisiä tavumääriä, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen mitä tahansa muutosta allokoinnin kokolaskelmaan. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suorituskoodin lataajaksi tai mielivaltaisen koodin suorittamiseksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-107-bounded-growth-policy.sh
```

Käännä ja suorita sanitisaattoreiden kanssa missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-107-sanitized \
  examples/c/c-107-bounded-growth-policy.c
./c-107-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat kuvailla sisääntulosopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähdön ja erottaa puolustusmuotoisen tavukäsittelyn oikean maailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
