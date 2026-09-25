# C 096 — Tietueen lukija- ja kirjoituskierros

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivalmiudet:** C 001–095
**Mukana oleva ohjelma:** [`examples/c/c-096-record-reader-writer-round-trip.c`](../../examples/c/c-096-record-reader-writer-round-trip.c)
**Deterministinen testi:** [`tests/c-096-record-reader-writer-round-trip.sh`](../../tests/c-096-record-reader-writer-round-trip.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin loppuun mennessä sinun pitäisi osata kirjoittaa ja lukea kiinteän kokoinen tietue paritetun rajatun API:n kautta ja verrata tulosta.

## Kertauskysymykset

1. Miksi tavutaulukon on kuljetettava nimenomainen `size_t` pituus sen sijaan, että luottaisiin `\0`?
2. Mikä ehto todistaa, että indeksi sijoittuu `count`-alkioiseen puskurin sisälle?
3. Nimeä yksi C-kirjaston paluuarvo, joka pitää tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Paritetut API:t tarvitsevat kapasiteettitarkistukset molemmissa päissä ja testivektorin kirjoitetulle muodolle.

Mukana oleva ohjelma käsittelee raakoja tavujoukkoja laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan edistämistä. Ohjelma on kannettava ISO C17 -koodia, ja se jättää tarkoituksella pois shellcoden, native-code-loaderit, mielivaltaisen assembly-suorituksen, prosessiin injisoinnin, itsensä muokkaavan koodin sekä todellisen kryptografisen toteutuksen — käytetyt esimerkit ovat opetuksellisia leluja, eivät tuotantotason kryptografiaa.

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
    const uint8_t field[] = {UINT8_C(0), UINT8_C(96)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 096 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-096-record-reader-writer-round-trip \
  examples/c/c-096-record-reader-writer-round-trip.c
./c-096-record-reader-writer-round-trip
```

Odotettu tuloste:

```text
C 096 decoded field: 96
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki pointer-plus-length- ja buffer-plus-capacity -sopimukset.
2. Löydä aikaisin hylätty virheellisen syötetilan ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin lopetuksena.
4. Kerro, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi lisädeterministinen hyväksytty syöte ja yksi hylätty reunatapaus. Säilytä olemassa olevat argumenttitarkistukset, nimenomaiset pituudet ja tulostuksen paluuarvon tarkistukset. Jos muutat mitään muistiallokointilaskentaa, lisää ennen laskua tarkistettu ylivuotoehto.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-096-record-reader-writer-round-trip.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-096-sanitized \
  examples/c/c-096-record-reader-writer-round-trip.c
./c-096-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat lausua tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulostuksen ja erottaa opetukselliset eheys- tai lelu-hajautusesimerkit todellisesta tuotantotason kryptografisesta suojauksesta.

## Vastausvinkit ja mallivastaus

1. Raakadatassa voi esiintyä `0x00`, joten sentinellikartoitus ei ole pätevä pituuden laskenta.
2. Eteenpäin kulkeva tarkistus on `index < count`.
3. I/O-, allokointi-, jäsennys- ja muotoilutulokset on tarkistettava ennen niiden tuottaman datan käyttämistä.
4. Vahvistettu ohjelma muuttaa ulostilaa vain kaikkien edellytysten onnistuttua.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
