# C 153 — Deterministinen testisyötteen generaattori

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–152
**Seuralaisohjelma:** [`examples/c/c-153-deterministic-test-input-generator.c`](../../examples/c/c-153-deterministic-test-input-generator.c)
**Deterministinen testi:** [`tests/c-153-deterministic-test-input-generator.sh`](../../tests/c-153-deterministic-test-input-generator.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi pystyä generoimaan toistettava pieni tavujono testikäyttöä varten ilman riippuvuutta järjestelmän satunnaisuudesta.

## Palautusharjoitus

1. Miksi raakatavupuskurilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin etenevän indeksin [`count`]-alkioisessa puskurissa?
3. Mitkä I/O-, allokaatio- tai jäsentämis-API:den paluuarvot täytyy tarkistaa ennen kuin niiden tuloksiin luotetaan?
4. Selitä, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Testidatan tulee olla toistettavaa. Deterministinen generaattori ei sovellu salaisuuksiin, kryptografiaan tai turvatunnuksiin.

Tämä oppitunti käyttää siirrettävää ISO C17 -koodia. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki tukeutuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Käytännön esimerkki

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
    const uint8_t input[] = {UINT8_C(153)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 153 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-153-deterministic-test-input-generator \
  examples/c/c-153-deterministic-test-input-generator.c
./c-153-deterministic-test-input-generator
```

Odotettu tulos:

```text
C 153 validated byte: 153
```

## Ohjattu harjoittelu

1. Tunnista tavujen väli tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -positio voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen minkään muuttuneen allokaatiokoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suorituskoodin lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-153-deterministic-test-input-generator.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-153-sanitized \
  examples/c/c-153-deterministic-test-input-generator.c
./c-153-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa defensiivisen tavukäsittelyn oikean maailman kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavujakin sisältävät olivat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
