# C 128 — Cleanup Ladder Pattern

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–127
**Seuraava ohjelma:** [`examples/c/c-128-cleanup-ladder-pattern.c`](../../examples/c/c-128-cleanup-ladder-pattern.c)
**Deterministinen testi:** [`tests/c-128-cleanup-ladder-pattern.sh`](../../tests/c-128-cleanup-ladder-pattern.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat käyttää yhtä helposti luettavaa siivouspolkua pienelle resurssin omistavalle funktiolle.

## Muistiharjoitus

1. Miksi raakabuffterin on kuljetettava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokaatio- tai jäsentämisrajapintojen paluuarvot on tarkistettava ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi kryptografisesti epävirallinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Jokaiselle hankitulle resurssille tarvitaan yksi vapautuspolku. Tarkista jokainen open-, write-, read- ja close-operaatio ennen kuin luotat sen tulokseen.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja luetelluina datoina, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja esimerkin riippuvuuden paluuarvon ja säilyttää ulostulon tilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assemblin suoritusta, prosessiin injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    if (!write_read_one("lesson.bin", UINT8_C(128), &value)) return EXIT_FAILURE;
    if (printf("C 128 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-128-cleanup-ladder-pattern \
  examples/c/c-128-cleanup-ladder-pattern.c
./c-128-cleanup-ladder-pattern
```

Odotettu tuloste:

```text
C 128 file byte: 128
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin ehtolistasta se ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin ulostuloarvo tai -asento voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoite yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkistettu aritmetiikkaehto ennen minkään muuttuvan allokaatiosuuruuden laskemista. Älä tee tästä opetus-esimerkistä kryptografista, suorituskoodin lataus- tai mielivaltaisen koodin suoritustekniikkaa.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-128-cleanup-ladder-pattern.sh
```

Käännä ja suorita sanitisaattoreilla, missä niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-128-sanitized \
  examples/c/c-128-cleanup-ladder-pattern.c
./c-128-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavujen käsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lueteltu tavu-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
