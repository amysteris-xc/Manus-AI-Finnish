# C 149 — Tiedoston metatiedon koon rajoitukset

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–148
**Seurantoprogrammi:** [`examples/c/c-149-file-metadata-size-limits.c`](../../examples/c/c-149-file-metadata-size-limits.c)
**Deterministinen testi:** [`tests/c-149-file-metadata-size-limits.sh`](../../tests/c-149-file-metadata-size-limits.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat käsitellä tiedoston koko‑metatietoa epäluotettavana, kunnes se on tarkistettu muunnettuna ja konfiguroidun koon rajoilla.

## Muistiharjoitus

1. Miksi raakabuffereilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot täytyy tarkistaa ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei‑kryptografinen tarkistusarvo ei todista autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tiedoston ilmoitettu koko voi olla sopimaton allokointiin. Tarkista API‑virheet, allekirjoitetun/allekirjoittamattoman tulkinnan ongelmat, `size_t`‑muunnos ja harkittu maksimiraja.

Tämä opetusmateriaali käyttää kannalta riippumatonta ISO C17:ää. Se käsittelee tavua laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkin tarvitseman paluuarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin ajoa, prosessien injektiota, itse‑muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_read_one(const char *path, uint8_t input, uint8_t *out)
{
    FILE *stream;
    if (path == NULL || out == NULL) return 0;
    stream = fopen(path, "wb");
    if (stream == NULL) return 0;
    if (fwrite(&input, sizeof input, 1, stream) != 1 || fclose(stream) != 0) return 0;
    stream = fopen(path, "rb");
    if (stream == NULL) return 0;
    if (fread(out, sizeof *out, 1, stream) != 1 || fclose(stream) != 0) return 0;
    return 1;
}

int main(void)
{
    uint8_t value = 0;
    if (!write_read_one("lesson.bin", UINT8_C(149), &value)) return EXIT_FAILURE;
    if (printf("C 149 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-149-file-metadata-size-limits \
  examples/c/c-149-file-metadata-size-limits.c
./c-149-file-metadata-size-limits
```

Odotettu tuloste:

```text
C 149 file byte: 149
```

## Ohjattu harjoitus

1. Määritä tavujakso tai tilasopimus jokaisessa apufunktiorajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi tässä esimerkissä `0x00` on tavallinen data eikä lopetusmerkki.
4. Kerro, milloin lähtöarvo tai -sijainti saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen mitään muuttunutta allokointikokolaskelmaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin ajo‑mekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-149-file-metadata-size-limits.sh
```

Käännä ja aja sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-149-sanitized \
  examples/c/c-149-file-metadata-size-limits.c
./c-149-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketulla tavupohjaisella API:lla on osoitin ja eksplisiittinen pituus, koska upotetut nollabytet ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian‑raidelle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
