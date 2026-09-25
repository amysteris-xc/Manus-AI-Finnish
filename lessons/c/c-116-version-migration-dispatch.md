# C 116 — Version Migration Dispatch

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–115
**Seurantaohjelma:** [`examples/c/c-116-version-migration-dispatch.c`](../../examples/c/c-116-version-migration-dispatch.c)
**Deterministinen testi:** [`tests/c-116-version-migration-dispatch.sh`](../../tests/c-116-version-migration-dispatch.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata ohjata rajattu tietue sallitulle versio-käsittelijälle ja hylätä tuetut versiot.

## Palauttava harjoitus

1. Miksi raakabufferin tulee kantaa eksplisiittistä `size_t` pituutta?
2. Mikä silmukaehdotus pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot on tarkistettava ennen kuin niiden antamiin tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole todennus aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Versiokenttä valitsee dokumentoidun jäsentäjäpolun. Tuntemattomia versioita ei pidä tulkita lähellä olevana tunnetuksi versioksi.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavut luettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluurinteen, joihin esimerkki nojaa, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista asm-suoritusta, prosessiin injektointia, itseään muuttavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(116)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 116 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-116-version-migration-dispatch \
  examples/c/c-116-version-migration-dispatch.c
./c-116-version-migration-dispatch
```

Odotettu tuloste:

```text
C 116 validated byte: 116
```

## Ohjattu harjoittelu

1. Tunnista tavuspan ja tilasopimus jokaisella apurajapinnalla.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä lopetusmerkki.
4. Ilmoita, milloin tulosarvo tai asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokointikoon laskemista. Älä muunna tätä opetus-esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-116-version-migration-dispatch.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-116-sanitized \
  examples/c/c-116-version-migration-dispatch.c
./c-116-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat ilmoittaa syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Vuorokoodin paljastus: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulosteparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; yksittäinen pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
