# C 192 — Protokolladokumentaation tarkistuslista

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esitiedot:** C 001–191
**Seuraava ohjelma:** [`examples/c/c-192-protocol-documentation-checklist.c`](../../examples/c/c-192-protocol-documentation-checklist.c)
**Deterministinen testi:** [`tests/c-192-protocol-documentation-checklist.sh`](../../tests/c-192-protocol-documentation-checklist.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin jälkeen osaat dokumentoida tavujärjestyksen, rajoitukset, versiot, virheet ja testivektorit pienelle formaatil­le.

## Muistiharjoitus

1. Miksi raakadataa sisältävän tavutaulun tulee kantaa mukana eksplisiittistä `size_t` pituutta?
2. Mikä silmukkaehto pitää eteenpäin etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, varauksen- tai jäsentämis-API:en paluuarvot tulee tarkistaa ennen kuin niiden tuottamia arvoja uskotaan?
4. Perustele, miksi ei-kryptografinen tarkastus­tarkkuus ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Protokollakuvaus on osa korrektiutta. Epäselvät kentät ja määrittelemättömät rajoitukset luovat yhteentoimivuus- ja turvallisuusriskejä.

Tämä oppitunti käyttää siirrettävää ISO C17 -koodia. Se käsittelee tavut laskettuina datoina, tarkistaa kaikki osoittimet, määrät, kapasiteetit, muunnokset ja palautusarvot, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodilataimia, mielivaltaista assemblerin suoritusta, prosessi-injektiota, itseänimuuttavaa koodia tai aitoja kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(192)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 192 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-192-protocol-documentation-checklist \
  examples/c/c-192-protocol-documentation-checklist.c
./c-192-protocol-documentation-checklist
```

Odotettu tuloste:

```text
C 192 validated byte: 192
```

## Ohjattu harjoitus

1. Tunnista tavujänneke tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Määrittele milloin lähtöarvo tai -sijainti saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä ekspisiittiset tavujen määrät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavaan lataimeen tai mielivaltaisen koodin suoritukseen kykeneväksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-192-protocol-documentation-checklist.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-192-sanitized \
  examples/c/c-192-protocol-documentation-checklist.c
./c-192-sanitized
```

## Lopullinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn tosimaailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketulla tavua API:lla on osoitin plus eksplisiittinen pituus, koska myös upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Korrektius vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliselle uralle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
