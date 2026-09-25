# C 182 — Yhteensopivuusmatriisin testit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–181
**Seuraava ohjelma:** [`examples/c/c-182-compatibility-matrix-tests.c`](../../examples/c/c-182-compatibility-matrix-tests.c)
**Deterministinen testi:** [`tests/c-182-compatibility-matrix-tests.sh`](../../tests/c-182-compatibility-matrix-tests.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopuksi sinun pitäisi osata testata valittuja tuottaja- ja kuluttajaversioiden yhdistelmiä selkein hyväksytyin ja hylätyin tuloksin.

## Muistin virkistys

1. Miksi raakabufferin tulee kantaa mukanaan eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää eteenpäin etenevän indeksiä sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O:n, allokaation tai jäsentämisen API:en palautusarvot täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Yhteensopivuus on dokumentoitu matriisi. Tukemattomien yhdistelmien tulee epäonnistua tarkoituksella sen sijaan, että ne heikentäisivät toimintaa hiljaisesti.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavut laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautetuloksen, johon esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataimia, mielivaltaista assemblyn suorittamista, prosessiin injektointia, itseään muuttavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(182)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 182 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-182-compatibility-matrix-tests \
  examples/c/c-182-compatibility-matrix-tests.c
./c-182-compatibility-matrix-tests
```

Odotettu tulos:

```text
C 182 validated byte: 182
```

## Ohjattu harjoitus

1. Tunnista tavujen span tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Mainitse, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty reunatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-182-compatibility-matrix-tests.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-182-sanitized \
  examples/c/c-182-compatibility-matrix-tests.c
./c-182-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat sanoa syötesopimuksen, tunnistaa hylätyn reunatapauksen, selittää lähtötuloksen ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Varoitus: vastaukset ja malli

1. Laskettu tavun API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuohjelma validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa Russian-polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
