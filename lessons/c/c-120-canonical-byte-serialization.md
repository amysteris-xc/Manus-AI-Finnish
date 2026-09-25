# C 120 — Canonical Byte Serialization

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–119
**Seuralaisohjelma:** [`examples/c/c-120-canonical-byte-serialization.c`](../../examples/c/c-120-canonical-byte-serialization.c)
**Deterministinen testi:** [`tests/c-120-canonical-byte-serialization.sh`](../../tests/c-120-canonical-byte-serialization.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata sarjoittaa pieni arvo yhteen dokumentoituun, kanoniseen tavujärjestykseen.

## Hakukertausharjoitus

1. Miksi raakabufferin täytyy kantaa nimenomaista `size_t` pituutta?
2. Mikä silmukan ehto pitää etenevän indeksin turvallisesti `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Kanoninen ulostulo poistaa epäselvyydet testeissä ja vertailuissa. Se on formaatin ominaisuus, ei salausta tai autentikointia.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki nojaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivilataajia, mielivaltaista asennuskoodin suorittamista, prosessiin injektointia, itseä muuttavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

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
    const uint8_t encoded[] = {UINT8_C(0), UINT8_C(120)};
    uint16_t value = 0;
    if (!decode_u16_be(encoded, sizeof encoded, &value)) return EXIT_FAILURE;
    if (printf("C 120 canonical value: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-120-canonical-byte-serialization \
  examples/c/c-120-canonical-byte-serialization.c
./c-120-canonical-byte-serialization
```

Odotettu tuloste:

```text
C 120 canonical value: 120
```

## Ohjattu harjoittelu

1. Tunnista tavujen alue tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele, milloin ulostuloarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavulaskelmat, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen mitä tahansa muutettua allokointikoon laskentaa. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-120-canonical-byte-serialization.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-120-sanitized \
  examples/c/c-120-canonical-byte-serialization.c
./c-120-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavujen API käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
