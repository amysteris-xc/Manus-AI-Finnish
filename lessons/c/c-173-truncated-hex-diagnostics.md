# C 173 — Truncated Hex Diagnostics

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–172
**Seuralainen ohjelma:** [`examples/c/c-173-truncated-hex-diagnostics.c`](../../examples/c/c-173-truncated-hex-diagnostics.c)
**Deterministinen testi:** [`tests/c-173-truncated-hex-diagnostics.sh`](../../tests/c-173-truncated-hex-diagnostics.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin lopussa osaat renderöidä diagnostista viestiä varten vain rajoitetun etuliitteen tavutaulukosta.

## Hakukertaus

1. Miksi raakabytetaulukolla pitää olla nimenomainen `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisessa taulukossa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta pitää tarkistaa, ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Kerro miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Diagnostiikkaulosteen pitää sisältää nimenomainen maksimietuliite ja merkintä siitä, että dataa on jätetty pois. Älä koskaan renderöi mielivaltaista muistia.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavujoukkoja laskettavana datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkin vaatiman paluuarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessiin injektiota, itsensä muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Työstetty esimerkki

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
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(173), UINT8_C(255)};
    if (printf("C 173 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-173-truncated-hex-diagnostics \
  examples/c/c-173-truncated-hex-diagnostics.c
./c-173-truncated-hex-diagnostics
```

Odotettu tuloste:

```text
C 173 hex: 00ADFF
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä miksi `0x00` on tässä esimerkissä tavallinen data eikä lopetusmerkki.
4. Ilmoita koska lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä selkeät tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua allokointikokolaskelmaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavien lataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-173-truncated-hex-diagnostics.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-173-sanitized \
  examples/c/c-173-truncated-hex-diagnostics.c
./c-173-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit ilmoittaa syöttösopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli

1. Laskettu tavua-API käyttää osoitinta plus nimenomaista pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen lukemista `data[index]`.
3. Apuri validoi ennen kuin se muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
