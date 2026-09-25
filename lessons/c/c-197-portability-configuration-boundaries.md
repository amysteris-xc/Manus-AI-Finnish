# C 197 — Siirrettävyyden konfiguraatiorajat

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–196
**Seuralaisohjelma:** [`examples/c/c-197-portability-configuration-boundaries.c`](../../examples/c/c-197-portability-configuration-boundaries.c)
**Deterministinen testi:** [`tests/c-197-portability-configuration-boundaries.sh`](../../tests/c-197-portability-configuration-boundaries.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat erottaa siirrettävän ISO C17 -koodin selkeästi dokumentoidusta alustakohtaisesta konfiguraatiosta.

## Muistitesti

1. Miksi raakabuffereilla täytyy olla selkeä `size_t` pituus?
2. Mikä silmukkakäsite pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot pitää tarkistaa ennen kuin niiden tuottamia arvoja luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Siirrettävän ydinlogiikan ei tule hiljaisesti nojata isännän tavujärjestykseen, tiedostojen järjestelyyn, kääntäjän laajennuksiin tai määrittelemättömään käytökseen.

Tässä oppitunnissa käytetään siirrettävää ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkastaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautetuloksen, johon esimerkki luottaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suoritusta, prosessin injektiota, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

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
    const uint8_t input[] = {UINT8_C(197)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 197 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-197-portability-configuration-boundaries \
  examples/c/c-197-portability-configuration-boundaries.c
./c-197-portability-configuration-boundaries
```

Odotettu tuloste:

```text
C 197 validated byte: 197
```

## Ohjattu harjoitus

1. Tunnista tavujen alue (byte span) tai tilasopimus kunkin apurajapinnan kohdalla.
2. Löydä aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele, milloin ulostuloarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaatiokoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavien lataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-197-portability-configuration-boundaries.sh
```

Käännä ja aja sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-197-sanitized \
  examples/c/c-197-portability-configuration-boundaries.c
./c-197-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkastaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
