# C 095 — Alustariippumattomat serialisointisopimukset

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–094
**Mukana oleva ohjelma:** [`examples/c/c-095-cross-platform-serialization-contracts.c`](../../examples/c/c-095-cross-platform-serialization-contracts.c)
**Deterministinen testi:** [`tests/c-095-cross-platform-serialization-contracts.sh`](../../tests/c-095-cross-platform-serialization-contracts.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat määritellä kiinteäleveysisen tavujen serialisoinnin siten, että se ei riipu osoimen koosta tai isäntäjärjestelmän endianness-asetuksesta.

## Muistin palautusharjoitus

1. Miksi tavubufferin tulee kantaa eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen bufferin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka täytyy tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Formaattien tulee määrittää leveydet, tavujärjestys ja sallitut arvovälit riippumatta paikallisesta koneesta.

Mukana oleva ohjelma käsittelee raakadataa tavuin laskettuna. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan etenemistä. Ohjelma on kannettava ISO C17 -yhteensopiva esimerkki, ja se jättää tahallaan pois shellcoden, natiivikoodin lataimet, mielivaltaisen assemblerin suorittamisen, prosessiin injisoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int read_u16_be(const uint8_t *data, size_t len, uint16_t *out)
{
    if (data == NULL || out == NULL || len < 2) return 0;
    *out = (uint16_t)(((uint16_t)data[0] << 8) | data[1]);
    return 1;
}

int main(void)
{
    const uint8_t field[] = {UINT8_C(0), UINT8_C(95)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 095 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-095-cross-platform-serialization-contracts \
  examples/c/c-095-cross-platform-serialization-contracts.c
./c-095-cross-platform-serialization-contracts
```

Odotettu tulostus:

```text
C 095 decoded field: 95
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki pointer-plus-length- tai buffer-plus-capacity-sopimukset.
2. Etsi aikaisin hylätty virheellistä syötettä kuvaava ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabufferin lopetussenttinelä.
4. Mainitse, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyden tarkistusta — korosta ero kouluesimerkin ja todellisen kryptografisen suojauksen välillä.

## Itsenäinen harjoitus

Lisää yksi ylimääräinen deterministinen hyväksytty syöte ja yksi hylätty rajatapauksesta tuleva syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteiden takaisinpalautusten tarkistukset. Jos muutat mitään allokointilaskentaa, lisää ennen laskua ylivuodon tarkistusehto.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-095-cross-platform-serialization-contracts.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-095-sanitized \
  examples/c/c-095-cross-platform-serialization-contracts.c
./c-095-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavubufferin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa kouluesimerkin eheyden tai leikkimielisen hajautuksen todellisesta maailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinel-haku ei ole kelvollinen pituuslaskelma.
2. Eteenpäin kulkeva läpikäyntiehton muoto on `index < count`.
3. I/O:n, allokaation, jäsentämisen ja muotoilun tulokset täytyy tarkistaa ennen kuin niiden tuotoksia käytetään.
4. Vahvistettu ohjelma muuttaa ulostulotilaa vain sen jälkeen, kun kaikki ennakkoedellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaohje — OWASP"
