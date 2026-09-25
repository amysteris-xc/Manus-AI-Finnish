# C 186 — Metatietojen tarkistusarvon rajat

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Edellytykset:** C 001–185
**Seuraava esimerkkiohjelma:** [`examples/c/c-186-metadata-checksum-limits.c`](../../examples/c/c-186-metadata-checksum-limits.c)
**Deterministinen testi:** [`tests/c-186-metadata-checksum-limits.sh`](../../tests/c-186-metadata-checksum-limits.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi osata erottaa ei-kryptografinen metatietojen tarkistusarvo (checksum) ja autentikoitu eheysmekanismi.

## Muistiharjoitus

1. Miksi raakana tavualueena annettava puskuriosoite tarvitsee aina eksplisiittisen `size_t` pituuden?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, varaus- tai jäsentämis-API:sta pitää tarkistaa ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei toimi autentikointina aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tarkistusarvot (checksums) havaitsevat satunnaisen korruption, mutta eivät suojaa tahalliselta muokkaukselta. Vihamielisille syötteille käytä tarkastettuja, autentikoituja mekanismeja.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavujen määrääntyvyyttä, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja esimerkin riippuvuuden palautusarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivilataajia, mielivaltaista assemblerin suorittamista, prosessi-injektiota, itseään muuttavaa koodia tai oikeita kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational accidental-corruption check only; not authentication or cryptography. */
static uint8_t educational_xor_check(const uint8_t *data, size_t count)
{
    uint8_t value = 0;
    if (data == NULL && count != 0) return 0;
    for (size_t i = 0; i < count; ++i) value ^= data[i];
    return value;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(186), UINT8_C(1), UINT8_C(2)};
    uint8_t value = educational_xor_check(data, sizeof data);
    if (printf("C 186 educational check: %02" PRIX8 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-186-metadata-checksum-limits \
  examples/c/c-186-metadata-checksum-limits.c
./c-186-metadata-checksum-limits
```

Odotettu tuloste:

```text
C 186 educational check: B9
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi varhaisin ehto, joka hylkää kelvottoman osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro, milloin lähtöarvo tai -sijainti saa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapauksen esimerkki. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokointikoon laskentaa. Älä tee tästä opetus­tapauksesta kryptografista, suoritin­lataajaa tai mielivaltaista koodin­a­jo­tusta tekevä mekanismi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-186-metadata-checksum-limits.sh
```

Käännä ja suorita sanitointityökalujen kanssa, missä niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-186-sanitized \
  examples/c/c-186-metadata-checksum-limits.c
./c-186-sanitized
```

## Lopullinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; yksittäinen pieni esimerkki ei todista tuotannon turvallisuutta.

## Seuraava oppitunti

Seuraavaksi on aikataulutettu oppitunti, joka palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
