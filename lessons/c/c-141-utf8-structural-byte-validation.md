# C 141 — UTF-8-rakenteellinen tavutikkujen validointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–140
**Seurantaohjelma:** [`examples/c/c-141-utf8-structural-byte-validation.c`](../../examples/c/c-141-utf8-structural-byte-validation.c)
**Deterministinen testi:** [`tests/c-141-utf8-structural-byte-validation.sh`](../../tests/c-141-utf8-structural-byte-validation.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Tämän tunnin lopuksi sinun pitäisi pystyä tarkistamaan pieni tavujono rajoitetun, rakenteellisen UTF-8-kuvion osalta ennen näyttöä.

## Hakukerta harjoitus

1. Miksi raakabuffereilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkuehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä return-arvot I/O-, allokointi- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi kryptografisesti suojaamaton tarkistus-arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tekstikoodaus on edelleen tavudataa. Rakenteellinen tarkistus ei ole kieli- tai tietoturvapolitiikka eikä saa korvata luotettua dekooderia, kun tällaista tarvitaan.

Tässä oppitunnissa käytetään kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa kaikki osoittimet, laskurit, kapasiteetit, muunnokset ja palautusarvot, joihin esimerkki perustuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai aitoja kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(141), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 141 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-141-utf8-structural-byte-validation \
  examples/c/c-141-utf8-structural-byte-validation.c
./c-141-utf8-structural-byte-validation
```

Odotettu tuloste:

```text
C 141 reader byte: 141
```

## Ohjattu harjoittelu

1. Tunnista tavuväli tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema saa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokointikoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suorituksenlataajaan tai mielivaltaisen koodin suorittamisen mekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-141-utf8-structural-byte-validation.sh
```

Käännä ja suorita sanitisaattoreilla, kun tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-141-sanitized \
  examples/c/c-141-utf8-structural-byte-validation.c
./c-141-sanitized
```

## Loppukontrolli

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja malli—vastaus

1. Laskettu tavu-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
