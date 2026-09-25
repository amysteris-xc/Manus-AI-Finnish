# C 132 — Component Contract Documentation

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–131
**Seuralaisohjelma:** [`examples/c/c-132-component-contract-documentation.c`](../../examples/c/c-132-component-contract-documentation.c)
**Deterministinen testi:** [`tests/c-132-component-contract-documentation.sh`](../../tests/c-132-component-contract-documentation.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi pystyä kirjoittamaan tiivis sopimus tavua käsittelevälle apurutiinille ja osoittamaan se koodissa.

## Muistiharjoitus

1. Miksi raakabuffereilla on oltava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin käyvän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kriptografinen tarkastus-arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Hyödyllinen sopimus nimeää tulot, tulosteet, omistajuuden, rajat, onnistumistuloksen ja virhekäyttäytymisen ennen toteutuksen yksityiskohtia.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautusarvon, joihin esimerkki luottaa, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asennuskoodin suorittamista, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(132)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 132 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-132-component-contract-documentation \
  examples/c/c-132-component-contract-documentation.c
./c-132-component-contract-documentation
```

Odotettu tulostus:

```text
C 132 validated byte: 132
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apurin rajapinnalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin tulosarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen minkään muokatun allokointikoon laskentaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suorittamisen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-132-component-contract-documentation.sh
```

Käännä ja suorita sanitisaattoreilla, missä niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-132-sanitized \
  examples/c/c-132-component-contract-documentation.c
./c-132-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määrittää syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen opintolinjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
