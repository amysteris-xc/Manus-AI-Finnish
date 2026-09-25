# C 158 — Property Test Invariants

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–157
**Seuralaisohjelma:** [`examples/c/c-158-property-test-invariants.c`](../../examples/c/c-158-property-test-invariants.c)
**Deterministinen testi:** [`tests/c-158-property-test-invariants.sh`](../../tests/c-158-property-test-invariants.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata määritellä ja testata pieni invariantti kiinteällä, laskettujen tavujen alueella.

## Muistiharjoitus

1. Miksi raaka tavutaulukko on varustettava eksplisiittisellä `size_t` pituudella?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkion kokoisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämisrajapinnoista on tarkistettava ennen kuin niiden tuottamiin ulostuloihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistus-arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Invariantti on tarkka ominaisuus, esimerkiksi koodaa sitten dekoodaa säilyttäen arvon. Raja domain niin, että virheet ovat toistettavissa.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessiin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Käyty läpi esimerkki

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
    const uint8_t input[] = {UINT8_C(158)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 158 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-158-property-test-invariants \
  examples/c/c-158-property-test-invariants.c
./c-158-property-test-invariants
```

Odotettu tulos:

```text
C 158 validated byte: 158
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus kunkin apufunktion rajapinnassa.
2. Löydä varhaisin ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data, ei loppumerkki.
4. Sano, milloin ulostuloarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokointikoon laskemista. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suorituksen lataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-158-property-test-invariants.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-158-sanitized \
  examples/c/c-158-property-test-invariants.c
./c-158-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustuksellisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja malli-vastaus

1. Laskettu tavurajapinta käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi ajoitettu oppitunti palaa venäjänkieliseen linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
