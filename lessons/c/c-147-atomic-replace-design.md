# C 147 — Atomic Replace Design

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–146
**Seurantaohjelma:** [`examples/c/c-147-atomic-replace-design.c`](../../examples/c/c-147-atomic-replace-design.c)
**Deterministinen testi:** [`tests/c-147-atomic-replace-design.sh`](../../tests/c-147-atomic-replace-design.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat selittää ja mallintaa varmennusvaiheet turvallisen korvaus­työnkulun ympärillä ilman oletuksia alustan takeista.

## Muistiharjoitus

1. Miksi raakabyte­tason puskurilla pitää olla selkeä `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsennys-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastus­arvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Väliaikainen tiedosto, tarkastettu sulku ja eksplisiittinen uudelleennimeämiskäytäntö vähentävät riskiä, mutta alustan semantiikka ja pysyvyys vaativat silti dokumentoitua järjestelmän tukea.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, laskelman, kapasiteetin, muunnoksen ja paluu­tuloksen, joita esimerkki edellyttää, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblin suorittamista, prosessien injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    if (!write_read_one("lesson.bin", UINT8_C(147), &value)) return EXIT_FAILURE;
    if (printf("C 147 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-147-atomic-replace-design \
  examples/c/c-147-atomic-replace-design.c
./c-147-atomic-replace-design
```

Odotettu tuloste:

```text
C 147 file byte: 147
```

## Ohjattu harjoitus

1. Tunnista tavutusväli tai tilasopimus kunkin apu­toiminnon rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, laskelman, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro, milloin lähtöarvo tai -asema saattaa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset bittimäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen varauksen koon laskemista. Älä muunna tätä opetus­tapausta kryptografiseksi, suoritin­lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-147-atomic-replace-design.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-147-sanitized \
  examples/c/c-147-atomic-replace-design.c
./c-147-sanitized
```

## Lopullinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan bittikäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli­vastaus

1. Laskettu byte-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nolla­bitit ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apu­toiminto validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten tarkastelua; yksittäinen pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa Russian-polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
