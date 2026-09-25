# C 102 — Cursor and Span API Separation

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–101
**Seurantoprogrammi:** [`examples/c/c-102-cursor-and-span-api-separation.c`](../../examples/c/c-102-cursor-and-span-api-separation.c)
**Deterministinen testi:** [`tests/c-102-cursor-and-span-api-separation.sh`](../../tests/c-102-cursor-and-span-api-separation.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat erottaa laskettuun tavujaksoon kuuluvan datan ja muuttuvan kursorin siten, että kumpikin sopimus pysyy tarkasteltavissa.

## Muistiharjoitus

1. Miksi raakaa tavupuskuria on kantettava selkeällä `size_t` pituudella?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:iden paluuarvot on tarkistettava ennen kuin niiden tuloksia luotetaan?
4. Miksi ei-kryptografinen tarkastusluku ei ole autentikointi aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Span nimeää datan ja pituuden; kursori lisää nykyisen position. Älä siirrä kursoria eteenpäin ennen kuin luku onnistuu.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja palautusarvon, joihin esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Ratkaisuesimerkki

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
    const uint8_t bytes[] = {UINT8_C(102), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 102 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-102-cursor-and-span-api-separation \
  examples/c/c-102-cursor-and-span-api-separation.c
./c-102-cursor-and-span-api-separation
```

Odotettu tulos:

```text
C 102 reader byte: 102
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus kussakin apurajapinnassa.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele milloin lähtöarvo tai -positio voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaatiokoon laskemista. Älä muunna tätä opetus-esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-102-cursor-and-span-api-separation.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-102-sanitized \
  examples/c/c-102-cursor-and-span-api-separation.c
./c-102-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määrittää syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavujen käsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavujen API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat validia dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin se lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi on paluu venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
