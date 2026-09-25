# C 188 — Kirjaston käärerajapinta

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–187
**Seurantoprogrammi:** [`examples/c/c-188-library-wrapper-interface.c`](../../examples/c/c-188-library-wrapper-interface.c)
**Deterministinen testi:** [`tests/c-188-library-wrapper-interface.sh`](../../tests/c-188-library-wrapper-interface.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi osaat suunnitella tarkastetun tavutaulukon (byte-buffer) käärerajapinnan hypoteettisen ylläpidettävän kirjastokutsun ympärille.

## Hakuharjoitus

1. Miksi raakalla tavutaulukolla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisen taulukon rajojen sisällä?
3. Mitkä I/O-, varauksen- tai jäsentämis-APIen palautusarvot täytyy tarkistaa ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi ei-kryptografinen tarkastus-arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Kääre validoi pituudet ja mapittaa virheet, mutta sen täytyy säilyttää kirjaston dokumentoidut vaatimukset eikä toteuttaa uudelleen primitiivejä.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa kaikki osoittimet, määrät, kapasiteetit, muunnokset ja palautustulokset, joihin esimerkki perustuu, ja säilyttää lähtötilan siihen saakka, kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assembler-suoritusta, prosessi-injektiota, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(188)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 188 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-188-library-wrapper-interface \
  examples/c/c-188-library-wrapper-interface.c
./c-188-library-wrapper-interface
```

Odotettu tuloste:

```text
C 188 validated byte: 188
```

## Ohjattu harjoittelu

1. Tunnista tavujakso tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa, ei loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asento voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä ja lisää tarkistettu aritmetiikkaehto ennen mitä tahansa muutettua varauskoon laskentaa. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suorittamiseksi tarkoitetuksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-188-library-wrapper-interface.sh
```

Käännä ja suorita sanitointityökaluilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-188-sanitized \
  examples/c/c-188-library-wrapper-interface.c
./c-188-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit ilmoittaa sisääntulosopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähdön ja erottaa defensiivisen tavankäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausvinkki: vastaukset ja mallivastaus

1. Laskettu tavua-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apumoduuli valideeraa ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Russian-ketjuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
