# C 179 — Laajennettavat TLV-kenttäsäännöt

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edellytykset:** C 001–178
**Seurantaoheisohjelma:** [`examples/c/c-179-extensible-tlv-field-rules.c`](../../examples/c/c-179-extensible-tlv-field-rules.c)
**Deterministinen testi:** [`tests/c-179-extensible-tlv-field-rules.sh`](../../tests/c-179-extensible-tlv-field-rules.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat jäsentää rajatun type-length-value -kentän säilyttäen säännöt tuntemattomille ja toistuville kentille.

## Muistinvarainen harjoittelu

1. Miksi raaka tavutaulukko täytyy kantaa mukana eksplisiittisen `size_t` pituisena?
2. Mikä silmukkaehto pitää eteenpäin kulkevan indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, varaus- tai jäsennys-API:sta täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

TLV:n laajennettavuus perustuu tarkistettuihin pituuksiin, ilmoitettuun duplikaattipolitiikkaan ja turvalliseen ohitusääntöön tuetuissa valinnaisissa kentissä.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se pitää tavut laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivilataajia, mielivaltaista assemblyn suoritusta, prosessin injektiota, itseään muuttavaa koodia tai oikeita kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { const uint8_t *data; size_t count; size_t position; } Reader;

static int reader_take(Reader *reader, uint8_t *out)
{
    if (reader == NULL || out == NULL || reader->data == NULL || reader->position >= reader->count) return 0;
    *out = reader->data[reader->position];
    reader->position++;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(179), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 179 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-179-extensible-tlv-field-rules \
  examples/c/c-179-extensible-tlv-field-rules.c
./c-179-extensible-tlv-field-rules
```

Odotettu tuloste:

```text
C 179 reader byte: 179
```

## Ohjattu harjoittelu

1. Tunnista tavujono tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi varmin varhainen ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä lopetusmerkki.
4. Ilmoita milloin lähtöarvo tai sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauksen kokolaskelmaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavaa lataavaksi tai mielivaltaisen koodin suoritusta mahdollistavaksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-179-extensible-tlv-field-rules.sh
```

Käännä ja suorita sanitisaattoreilla missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-179-sanitized \
  examples/c/c-179-extensible-tlv-field-rules.c
./c-179-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat kertoa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavukäsittelyn todellisen maailman kryptografisesta suojauksesta.

## Vastausvinkit ja mallivastaukset

1. Laskettu tavurajapinta käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kävely tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
