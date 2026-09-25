# C 108 — Object Representation versus Wire Format

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–107
**Kumppaniohjelma:** [`examples/c/c-108-object-representation-vs-wire-format.c`](../../examples/c/c-108-object-representation-vs-wire-format.c)
**Deterministinen testi:** [`tests/c-108-object-representation-vs-wire-format.sh`](../../tests/c-108-object-representation-vs-wire-format.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa sinun pitäisi osata erottaa muistissa oleva C-objekti kannettavasta tavumuodosta.

## Muistiharjoitus

1. Miksi raakabyte-puskurilla on oltava selvästi määrätty `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta pitää tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei todista aitoa tunnistautumista aktiivista hyökkääjää vastaan.

## Ydinajatus

Älä koskaan serialisoi C-oliota olettaen sen täytteitä, kohdistusta, osoitinkokoa tai isäntäjärjestelmän tavujärjestystä vakaaksi protokollasopimukseksi.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavujoukkoja laskettuina tietoina, tarkistaa kaikki osoittimet, lukumäärät, kapasiteetit, muunnokset ja palautusarvot joita esimerkki käyttää, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessiin injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t encoded[] = {UINT8_C(0), UINT8_C(108)};
    uint16_t value = 0;
    if (!decode_u16_be(encoded, sizeof encoded, &value)) return EXIT_FAILURE;
    if (printf("C 108 canonical value: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-108-object-representation-vs-wire-format \
  examples/c/c-108-object-representation-vs-wire-format.c
./c-108-object-representation-vs-wire-format
```

Odotettu tuloste:

```text
C 108 canonical value: 108
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus kussakin apurajapisteessä.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä lopetusmerkki.
4. Kerro, milloin ulostuloarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaatiokoon laskentaa. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-108-object-representation-vs-wire-format.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-108-sanitized \
  examples/c/c-108-object-representation-vs-wire-format.c
./c-108-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitinta ja selkeää pituutta, koska upotetut nolla-bytet ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että rajausten tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Venäjän materiaalilinjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
