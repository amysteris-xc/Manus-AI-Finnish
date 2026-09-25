# C 172 — Virheraportin sensurointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–171
**Seurantaojelma:** [`examples/c/c-172-error-report-redaction.c`](../../examples/c/c-172-error-report-redaction.c)
**Deterministinen testi:** [`tests/c-172-error-report-redaction.sh`](../../tests/c-172-error-report-redaction.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi osata tuottaa yleinen virhekoodi sen sijaan, että toistaisit näennäisesti arkaluonteisen tavuarvon (byte).

## Kertaus

1. Miksi raaka tavutaulukko (raw byte buffer) täytyy kuljettaa mukana selkeällä `size_t` pituudella?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsentämis-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Virheraportointi auttaa diagnosoinnissa ilman salaisuuksien tai luottamattoman sisällön paljastamista. Tämä on suunnitteluraja, ei kryptografinen takuu.

Tämä oppitunti käyttää kannettavaa ISO C17 -koodia. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei tee shellcodea, natiivia lataajaa, mielivaltaisen assemblerin suorittamista, prosessin injektiota, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(172)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 172 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-172-error-report-redaction \
  examples/c/c-172-error-report-redaction.c
./c-172-error-report-redaction
```

Odotettu tuloste:

```text
C 172 validated byte: 172
```

## Ohjattu harjoitus

1. Tunnista tavujen alue (byte span) tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiaksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritustavaksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-172-error-report-redaction.sh
```

Käännä ja suorita sanitisaattoreilla, jos saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-172-sanitized \
  examples/c/c-172-error-report-redaction.c
./c-172-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa defensiivisen tavujen käsittelyn todellisesta kryptografisesta suojasta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavupääteinen API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin se lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Venäjän rataan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
