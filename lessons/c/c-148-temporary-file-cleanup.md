# C 148 — Väliaikaisten tiedostojen siivous

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edeltävät kurssit:** C 001–147
**Seurantaohjelma:** [`examples/c/c-148-temporary-file-cleanup.c`](../../examples/c/c-148-temporary-file-cleanup.c)
**Deterministinen testi:** [`tests/c-148-temporary-file-cleanup.sh`](../../tests/c-148-temporary-file-cleanup.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa sinun pitäisi osata luoda, käyttää ja poistaa pieni väliaikainen testitiedosto tarkastettujen siivouspolkujen kautta.

## Muistiharjoitus

1. Miksi raaka tavutaulu tulee kantaa eksplisiittisen `size_t` pituuden kanssa?
2. Mikä silmukkakonditio pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsennys-API:sta on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Siivous suoritetaan onnistumisen ja virheen yhteydessä. Tiedostonimet ja sijainnit tulee hallita sen sijaan, että ne johdettaisiin epäluotettavasta syötteestä.

Tämä oppitunti käyttää kannateltavaa ISO C17 -kieltä. Se käsittelee tavujoukkoja laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki tukeutuu, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assemblyn suorittamista, prosessiin injektointia, itseä muuttavaa koodia tai aitoja kryptografisia toteutuksia.

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
    if (!write_read_one("lesson.bin", UINT8_C(148), &value)) return EXIT_FAILURE;
    if (printf("C 148 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-148-temporary-file-cleanup \
  examples/c/c-148-temporary-file-cleanup.c
./c-148-temporary-file-cleanup
```

Odotettu tuloste:

```text
C 148 file byte: 148
```

## Ohjattu harjoitus

1. Tunnista jokaisessä apufunktiorajapinnassa tavujen alue (byte span) tai tilasopimus.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro milloin ulostuloarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainitsema turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavan koodin lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-148-temporary-file-cleanup.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-148-sanitized \
  examples/c/c-148-temporary-file-cleanup.c
./c-148-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn oikean maailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavujen API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio tarkistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
