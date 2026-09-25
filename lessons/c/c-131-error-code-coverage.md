# C 131 — Virhekoodikattavuus

**Arvioitu opiskelu‑aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–130
**Seurana oleva ohjelma:** [`examples/c/c-131-error-code-coverage.c`](../../examples/c/c-131-error-code-coverage.c)
**Deterministinen testi:** [`tests/c-131-error-code-coverage.sh`](../../tests/c-131-error-code-coverage.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata ajaa jokainen nimetty virhetulos pienestä jäsentimestä deterministisessä tapauksessa.

## Muistiharjoitus

1. Miksi raakabufferin (tavutaulukon) täytyy kantaa mukana eksplisiittinen `size_t` pituus?
2. Mikä silmusääntö pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O‑, allokaatio‑ tai jäsentämisrajapinnoista täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei‑kryptografinen tarkistus‑arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Virhepolut ovat osa julkista sopimusta. Testien tulisi erottaa virheelliset argumentit, katkennut syöte ja virheelliset kentät.

Tässä oppitunnissa käytetään kannettavaa ISO C17:ää. Se käsittelee tavut laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(131)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 131 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-131-error-code-coverage \
  examples/c/c-131-error-code-coverage.c
./c-131-error-code-coverage
```

Odotettu tuloste:

```text
C 131 validated byte: 131
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus jokaisella apurajapinnan rajalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita milloin ulostuloarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen kuin muutetaan mitään allokaatiokoon laskentaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-131-error-code-coverage.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-131-sanitized \
  examples/c/c-131-error-code-coverage.c
./c-131-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit kuvata syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn oikean maailman kryptografisesta suojauksesta.

## Spoiler: vastaukset ja mallivastaus

1. Lasketun tavurajapinnan muoto on osoitin plus eksplisiittinen pituus, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
