# C 133 — Span-rajapinnan johdonmukaisuus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–132
**Seurantoprogrammi:** [`examples/c/c-133-span-interface-consistency.c`](../../examples/c/c-133-span-interface-consistency.c)
**Deterministinen testi:** [`tests/c-133-span-interface-consistency.sh`](../../tests/c-133-span-interface-consistency.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopuksi osaat soveltaa yhdenmukaista osoitin-plus-pituus -rajapintaa liittyvien tavujen apufunktioiden läpi.

## Muistiharjoitus

1. Miksi raakabyte-puskurin on kantava eksplisiittistä `size_t` pituutta?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsennys-API:en palautusarvot on tarkastettava ennen kuin niiden tuottamiin ulostuloihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastusluku ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Johdonmukainen span-API käsittelee null-arvon vain nollapituuden yhteydessä, säilyttää eksplisiittiset määrät ja välttää sentinellikartoituksia.

Tämä oppitunti käyttää kannettavaa ISO C17 -kieltä. Se käsittelee tavuja laskettuina datoina, tarkastaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautetun tuloksen, joihin esimerkki nojautuu, ja säilyttää ulostulon tilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivia koodilataajaa, mielivaltaista assemblyn suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t source[] = {UINT8_C(133), UINT8_C(0)};
    uint8_t destination[2] = {0};
    if (!copy_span(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 133 copied byte: %u\n", (unsigned)destination[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-133-span-interface-consistency \
  examples/c/c-133-span-interface-consistency.c
./c-133-span-interface-consistency
```

Odottu tulostus:

```text
C 133 copied byte: 133
```

## Ohjattu harjoitus

1. Tunnista tavuspan ja tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin se ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita milloin ulostuloarvo tai -asema voi muuttua.
5. Selitä harjoituksen mainittu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkastettu aritmeettinen ehto ennen minkään muutetun allokaatiokoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-133-span-interface-consistency.sh
```

Käännä ja suorita sanitisaattoreilla, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-133-sanitized \
  examples/c/c-133-span-interface-consistency.c
./c-133-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat ilmoittaa syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Juonipaljastus: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkastaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen ulostuloparametrin, kursori- tai tilakentän muuttamista.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantoturvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
