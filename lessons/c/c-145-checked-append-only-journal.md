# C 145 — Tarkastettu lisäysvain päiväkirja

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–144
**Seurantaohjelma:** [`examples/c/c-145-checked-append-only-journal.c`](../../examples/c/c-145-checked-append-only-journal.c)
**Deterministinen testi:** [`tests/c-145-checked-append-only-journal.sh`](../../tests/c-145-checked-append-only-journal.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi pystyä liittämään pieni binäärinen tietue testipäiväkirjaan samalla kun tarkistat jokaisen kirjoitusoperaation tuloksen.

## Muistiharjoitus

1. Miksi raaka tavutaulukko (byte buffer) tarvitsee eksplisiittisen `size_t` pituuden?
2. Mikä silmukkaehto pitää eteenpäin etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuotoksia luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Lisäysvain tallennustilakin tarvitsee tarkistetun avauksen, kirjoituksen, flushin tai sulkemisen, kokorajat ja selkeän palautuspolitiikan.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavujoukkoja laskettuna tietona, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki luottaa, ja säilyttää kirjoitustilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessiin injektointia, itseä muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    if (!write_read_one("lesson.bin", UINT8_C(145), &value)) return EXIT_FAILURE;
    if (printf("C 145 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-145-checked-append-only-journal \
  examples/c/c-145-checked-append-only-journal.c
./c-145-checked-append-only-journal
```

Odotettu tuloste:

```text
C 145 file byte: 145
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, pidä alkuperäinen tuloste muuttumattomana hylkäyksen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen minkään muuttuneen allokaation koon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritettavan lataajaa tai mielivaltaisen koodin suorittamisen mekanismia.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-145-checked-append-only-journal.sh
```

Käännä ja aja sanitisaattoreiden kanssa, missä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-145-sanitized \
  examples/c/c-145-checked-append-only-journal.c
./c-145-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos pystyt määrittelemään syötteen sopimuksen, tunnistamaan hylätyn rajatapauksen, selittämään tulosteen ja erottamaan puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausavain ja mallivastaus (spoiler)

1. Laskettu tavua-API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
