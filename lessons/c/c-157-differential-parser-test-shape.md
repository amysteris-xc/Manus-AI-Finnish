# C 157 — Differentiaalinen jäsentäjätestin muoto

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–156
**Seurantiohjelma:** [`examples/c/c-157-differential-parser-test-shape.c`](../../examples/c/c-157-differential-parser-test-shape.c)
**Deterministinen testi:** [`tests/c-157-differential-parser-test-shape.sh`](../../tests/c-157-differential-parser-test-shape.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Oppitunnin lopussa osaat verrata kahta turvallista opettavaista jäsentäjäpolkua samoilla kiinteillä tavutuloilla.

## Muistin virkistys

1. Miksi raakabuffertin tulee kantaa eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsennys-APIen paluuarvot on tarkistettava ennen kuin niiden tuottamaan tietoon luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusluku ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Differentiaalinen testaus vertaa ilmoitettua käyttäytymistä; se ei todista kumpaakaan toteutusta turvalliseksi tai täydelliseksi.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, konversion ja paluuarvon, joihin esimerkki luottaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asennuskoodin ajoa, prosessi-injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one(const uint8_t *data, size_t count, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (count != 1 || data[0] == 0) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(157)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 157 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-157-differential-parser-test-shape \
  examples/c/c-157-differential-parser-test-shape.c
./c-157-differential-parser-test-shape
```

Odotettu tulos:

```text
C 157 validated byte: 157
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele, milloin ulostuloarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, pidä alkuperäinen ulostulo ennallaan hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua allokointikoko-laskelmaa. Älä muuta tätä opettavaista esimerkkiä kryptografiseksi, suorituksen latausmekanismiksi tai mielivaltaisen koodin suorittamisen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-157-differential-parser-test-shape.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-157-sanitized \
  examples/c/c-157-differential-parser-test-shape.c
./c-157-sanitized
```

## Lopullinen itsediagnostiikka

Olet valmis seuraavaan oppituntiin, jos osaat kertoa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa suojatut tavukäsittelyt todellisesta maailmasta tulevasta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli-vastaus

1. Laskettu tavu-API käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nollatavua sisältävät arvot ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus edellyttää sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Venäjän kielelle suunnattuun linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
