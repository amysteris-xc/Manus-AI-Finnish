# C 174 — Diagnostic Code Taxonomy

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–173
**Seurantaohjelma:** [`examples/c/c-174-diagnostic-code-taxonomy.c`](../../examples/c/c-174-diagnostic-code-taxonomy.c)
**Deterministinen testi:** [`tests/c-174-diagnostic-code-taxonomy.sh`](../../tests/c-174-diagnostic-code-taxonomy.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata antaa pysyviä nimettyjä diagnostiikkakategorioita argumentti-, katkaisu- ja formaattivirheille.

## Muistiharjoitus

1. Miksi raakabuffeilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukaehdon pitää etenevän indeksin sisällä `count`-alkioisessa buffereissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistearvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Koodit tekevät virheistä koneellisesti tarkistettavia samalla kun viestit pysyvät ihmisen luettavina. Älä paljasta toteutuksen sisäisiä yksityiskohtia turvallisuusväitteenä.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja lueteltuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivin koodin lataajia, mielivaltaista asm-suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(174)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 174 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-174-diagnostic-code-taxonomy \
  examples/c/c-174-diagnostic-code-taxonomy.c
./c-174-diagnostic-code-taxonomy
```

Odotettu tuloste:

```text
C 174 validated byte: 174
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti saattaa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokointikoon laskemista. Älä muuta tätä opetusmuotoista esimerkkiä kryptografiseksi, suoritettavaan lataajaan tai mielivaltaisen koodin suoritukseen kykeneväksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-174-diagnostic-code-taxonomy.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-174-sanitized \
  examples/c/c-174-diagnostic-code-taxonomy.c
./c-174-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat ilmoittaa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoiler: vastaukset ja malli

1. Luvun sisältävä tavua käyttävä API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Russian-polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
