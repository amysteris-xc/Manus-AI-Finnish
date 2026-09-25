# C 103 — Explisiittiset endianness-testivektorit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–102
**Seuraava ohjelma:** [`examples/c/c-103-explicit-endianness-test-vectors.c`](../../examples/c/c-103-explicit-endianness-test-vectors.c)
**Deterministinen testi:** [`tests/c-103-explicit-endianness-test-vectors.sh`](../../tests/c-103-explicit-endianness-test-vectors.sh)
**Tekijä:** Manus AI

## Mitä osaat lopussa

Tämän oppitunnin lopuksi osaat dekoodata ja koodata kiinteäleveyskentän käyttämällä eksplisiittistä tavujärjestystä ja dokumentoituja testivektoreita.

## Hae palautusta (retrieval practice)

1. Miksi raaka tavupuskuri täytyy kantaa eksplisiittisellä `size_t` pituudella?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsentämis-API:den paluuarvot pitää tarkistaa ennen kuin niiden lähtöihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Testivektori määrittelee tavujonon ja odotetun arvon ilman riippuvuutta isäntämuistin asettelusta tai endiannessistä.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkin tarvitsemien funktioiden paluuarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Tarkasteltu esimerkki

```c
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int decode_u16_be(const uint8_t *bytes, size_t count, uint16_t *out)
{
    if (bytes == NULL || out == NULL || count != 2) return 0;
    *out = (uint16_t)(((uint16_t)bytes[0] << 8) | (uint16_t)bytes[1]);
    return 1;
}

int main(void)
{
    const uint8_t encoded[] = {UINT8_C(0), UINT8_C(103)};
    uint16_t value = 0;
    if (!decode_u16_be(encoded, sizeof encoded, &value)) return EXIT_FAILURE;
    if (printf("C 103 canonical value: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-103-explicit-endianness-test-vectors \
  examples/c/c-103-explicit-endianness-test-vectors.c
./c-103-explicit-endianness-test-vectors
```

Odotettu tuloste:

```text
C 103 canonical value: 103
```

## Ohjattu harjoitus

1. Tunnista tavujono tai tilasopimus jokaisen apurajapinnan kohdalla.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -asema saattaa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajaustapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen kuin teet mitään muuttuvaa varauskoon laskentaa. Älä muuta tätä opetus-esimerkkiä kryptografiseksi, suoritinan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-103-explicit-endianness-test-vectors.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-103-sanitized \
  examples/c/c-103-explicit-endianness-test-vectors.c
./c-103-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötteen sopimuksen, tunnistaa hylätyn rajaustapauksen, selittää lähdön ja erottaa defensiivisen tavujen käsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli-vastaus

1. Laskettu tavurajapinta käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Russian-polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
