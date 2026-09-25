# C 181 — Tagged Union Regression Tests

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–180
**Seuraava ohjelma:** [`examples/c/c-181-tagged-union-regression-tests.c`](../../examples/c/c-181-tagged-union-regression-tests.c)
**Deterministinen testi:** [`tests/c-181-tagged-union-regression-tests.sh`](../../tests/c-181-tagged-union-regression-tests.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa osaat koota deterministisiä hyväksyttäviä ja hylättyjä tapauksia pienelle tagged-union -validaattorille.

## Hakukertaus

1. Miksi raakalle tavutaulukolle on kannettava mukana eksplisiittinen `size_t` pituus?
2. Mikä silmukkiehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämisrajapintojen paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Regressiotapausten tulisi kattaa jokainen tagi, väärä pituus, tuntematon tagi ja vakaus hylkäyksen jälkeen.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautusarvon, joihin esimerkki luottaa, ja säilyttää ulostulosään tilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessi-injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(181)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 181 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-181-tagged-union-regression-tests \
  examples/c/c-181-tagged-union-regression-tests.c
./c-181-tagged-union-regression-tests
```

Odotettu tuloste:

```text
C 181 validated byte: 181
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilaopimus kunkin apurajapinnan kohdalla.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin ulostuloarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokoinnin kokolaskelman tekemistä. Älä muutakaan tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-181-tagged-union-regression-tests.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-181-sanitized \
  examples/c/c-181-tagged-union-regression-tests.c
./c-181-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit ilmoittaa syöttösopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Ratkaisut ja mallivastaus (spoiler)

1. Lasketun tavun rajapinta käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
