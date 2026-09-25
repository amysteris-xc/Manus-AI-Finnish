# C 184 — Usean tietueen tiedostokuori

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–183
**Seuraava ohjelma:** [`examples/c/c-184-multi-record-file-envelope.c`](../../examples/c/c-184-multi-record-file-envelope.c)
**Deterministinen testi:** [`tests/c-184-multi-record-file-envelope.sh`](../../tests/c-184-multi-record-file-envelope.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata validoida yksinkertainen tiedostokuori, joka sisältää ennalta rajatun määrän kiinteän levyisiä tietueita.

## Palautusharjoitus

1. Miksi raakabittipuskurilla täytyy olla selkeä `size_t` pituus?
2. Mikä silmukkuehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tiedostokuoret tarvitsevat otsikon, lukumäärärajoituksen, tietueen leveyden, katkeamisen havaitsemisen ja tarkistetut objektimäärät jokaista lukua varten.

Tämä oppitunti käyttää kannateltavaa ISO C17 -kieltä. Se käsittelee tavut lasketuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessin injektiota, itseään muuttavaa koodia tai oikeita kryptografisia toteutuksia.

## Esimerkkiaskel

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
    if (!write_read_one("lesson.bin", UINT8_C(184), &value)) return EXIT_FAILURE;
    if (printf("C 184 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-184-multi-record-file-envelope \
  examples/c/c-184-multi-record-file-envelope.c
./c-184-multi-record-file-envelope
```

Odotettu tuloste:

```text
C 184 file byte: 184
```

## Ohjattu harjoittelu

1. Tunnista tavujakso tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaatiokokolaskelman tekemistä. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-184-multi-record-file-envelope.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-184-sanitized \
  examples/c/c-184-multi-record-file-envelope.c
./c-184-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavujen API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
