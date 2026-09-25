# C 201 — Final Byte Systems Review

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–200
**Seuranto-ohjelma:** [`examples/c/c-201-final-byte-systems-review.c`](../../examples/c/c-201-final-byte-systems-review.c)
**Deterministinen testi:** [`tests/c-201-final-byte-systems-review.sh`](../../tests/c-201-final-byte-systems-review.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat tarkastella täydellistä tavupohjaista C-työnkulkua sopimuksesta validointiin, testivektoriin, virheenkäsittelyyn ja kirjastorajapintaan asti.

## Muistin harjoitus

1. Miksi raakalla tavupuskurilla on oltava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, varaus- tai jäsennys-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastus- eli tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Lopputyö korostaa eksplisiittisiä pituuksia, tarkistettua aritmetiikkaa, siirrettävää serialisointia, rajattua työtä, turvallisia diagnostiikkaviestejä ja vakiintuneiden kryptografisten kirjastojen käyttöä todellisiin turvallisuustarpeisiin.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodilataajia, mielivaltaista käännöskiellon täyttämistä assemblya, prosessi-injektiota, itseä muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(201)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 201 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-201-final-byte-systems-review \
  examples/c/c-201-final-byte-systems-review.c
./c-201-final-byte-systems-review
```

Odotettu tulostus:

```text
C 201 validated byte: 201
```

## Ohjattu harjoitus

1. Tunnista tavujono tai tilasopimus jokaisella apurajalla.
2. Etsi aikaisin se ehto, joka hylkää virheellisen osoitteen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro, milloin lähtöarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty raja-arvotapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua varauskoon laskentaa. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin ajomekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-201-final-byte-systems-review.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-201-sanitized \
  examples/c/c-201-final-byte-systems-review.c
./c-201-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit ilmoittaa syötesopimuksen, tunnistaa hylätyn raja-arvotapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministiset testit että ilmoitettujen rajojen tarkistuksen; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjän kieleen.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
