# C 140 — Base64-rajan tarkistus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–139
**Seuraava ohjelma:** [`examples/c/c-140-base64-boundary-review.c`](../../examples/c/c-140-base64-boundary-review.c)
**Deterministinen testi:** [`tests/c-140-base64-boundary-review.sh`](../../tests/c-140-base64-boundary-review.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat tarkistaa Base64-tulosteen ja -syötteen rajatapaukset siten, että Base64 esitetään koodauksena, ei salaustapana.

## Muistiharjoitus

1. Miksi raakabuffermin täytyy kantaa eksplisiittistä `size_t` pituutta?
2. Mikä silmukkaehto pitää eteenpäin kasvavan indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kriptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Base64 on koodaus. Tarkista pituus, aakkosto, täytepolitiikka ja lähtökapasiteetti; se ei tarjoa salaisuutta eikä autentikointia.

Tämä opetus käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki perustuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessi-injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(140)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 140 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-140-base64-boundary-review \
  examples/c/c-140-base64-boundary-review.c
./c-140-base64-boundary-review
```

Odotettu tuloste:

```text
C 140 validated byte: 140
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus jokaisen apurajapinnan kohdalla.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Määritä milloin lähtöarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhden tarkan lauseen avulla.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Käytä selkeitä tavumääriä, säilytä alkuperäinen lähtö hylkäyksessä ja lisää tarkistettu aritmetiikkaehto ennen minkään muutetun allokaatiokoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi tai suoritettavan koodin lataus- tai mielivaltaisen suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-140-base64-boundary-review.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-140-sanitized \
  examples/c/c-140-base64-boundary-review.c
./c-140-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli

1. Laskettu tavua käyttävä API tarvitsee osoittimen lisäksi eksplisiittisen pituuden, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi oleva oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
