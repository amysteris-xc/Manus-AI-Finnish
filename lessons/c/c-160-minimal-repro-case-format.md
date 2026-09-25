# C 160 — Minimal Reproduction Case Format

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–159
**Seuraava ohjelma:** [`examples/c/c-160-minimal-repro-case-format.c`](../../examples/c/c-160-minimal-repro-case-format.c)
**Deterministinen testi:** [`tests/c-160-minimal-repro-case-format.sh`](../../tests/c-160-minimal-repro-case-format.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun pitäisi pystyä tallentamaan minimaalinen tavutulo ja odotettu tilatila regressiotapausta varten.

## Muistin virkistys

1. Miksi raakanavutun puskin on kantettava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi kryptografiaa ei tallentava tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Tiivis reproduktio sisältää syötebitit, pituuden, odotetun tuloksen ja aiemman vian tilan ilman, että arkaluonteisia oikeita tietoja paljastetaan.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluuarvon johon esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessin injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Käytännön esimerkki

```c
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_hex_prefix(const uint8_t *data, size_t count, size_t maximum)
{
    size_t shown = count < maximum ? count : maximum;
    if (data == NULL && count != 0) return 0;
    for (size_t i = 0; i < shown; ++i)
        if (printf("%02" PRIX8, data[i]) < 0) return 0;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(160), UINT8_C(255)};
    if (printf("C 160 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-160-minimal-repro-case-format \
  examples/c/c-160-minimal-repro-case-format.c
./c-160-minimal-repro-case-format
```

Odotettu tuloste:

```text
C 160 hex: 00A0FF
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoitteen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoite

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavupituudet, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen mitään muutettua allokointikoko-laskelmaa. Älä tee tästä opetusesimerkistä kryptografista, suoritettavaa lataajaa tai mekanismia mielivaltaisen koodin suorittamiseen.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-160-minimal-repro-case-format.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-160-sanitized \
  examples/c/c-160-minimal-repro-case-format.c
./c-160-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määrittää syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähdön ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojasta.

## Vastausvinkki: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenkulkua tarkistava silmukka varmistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliseen linjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
