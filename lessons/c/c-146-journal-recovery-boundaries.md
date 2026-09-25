# C 146 — Journal Recovery Boundaries

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–145
**Seuraava ohjelma:** [`examples/c/c-146-journal-recovery-boundaries.c`](../../examples/c/c-146-journal-recovery-boundaries.c)
**Deterministinen testi:** [`tests/c-146-journal-recovery-boundaries.sh`](../../tests/c-146-journal-recovery-boundaries.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat lukea yksinkertaisen lehden (journal) tallennustietueiden sarjan ja pysähtyä turvallisesti, kun viimeinen tietue on katkennut.

## Palautusharjoitus

1. Miksi raakatavujoukon yhteydessä on ilmoitettava eksplisiittinen `size_t` pituus?
2. Mikä silmukkuehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, varaus- tai jäsentämisrajapintojen paluuarvot on tarkistettava ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole todennus aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Palautus erottaa siistin tiedoston lopun (end-of-file) I/O-virheestä sekä virheellisestä tai osittaisesta tietueaineistosta. Älä keksi puuttuvia tavuja.

Tämä oppitunti käyttää kannettavaa ISO C17 -kieltä. Se käsittelee tavuja laskettavana datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkin luottamuksen arvoiksi olevat paluuarvot, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessiin injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Esimerkki askel askeleelta

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
    if (!write_read_one("lesson.bin", UINT8_C(146), &value)) return EXIT_FAILURE;
    if (printf("C 146 file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-146-journal-recovery-boundaries \
  examples/c/c-146-journal-recovery-boundaries.c
./c-146-journal-recovery-boundaries
```

Odotettu tuloste:

```text
C 146 file byte: 146
```

## Ohjattu harjoitus

1. Tunnista tavujen väli (byte span) tai tilasopimus (state contract) kunkin apurajapinnan kohdalla.
2. Etsi aikaisimpiin ehtoihin pohjautuva tarkistus, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin ulostuloarvo tai -asento saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Oma harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajaustapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauksen suuruuden laskemista. Älä muuta tätä opetusmallia kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-146-journal-recovery-boundaries.sh
```

Käännä ja suorita sanitisaattoreilla, jos ne ovat saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-146-sanitized \
  examples/c/c-146-journal-recovery-boundaries.c
./c-146-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat kertoa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettua tavurajapintaa käytettäessä tarvitaan osoitin ja eksplisiittinen pituus, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus edellyttää sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
