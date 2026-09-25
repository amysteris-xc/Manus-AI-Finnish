# C 073 — Streaming CRC-32 Integrity Check

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edellytykset:** C 001–072
**Komppaniohjelma:** [`examples/c/c-073-streaming-crc32-integrity-check.c`](../../examples/c/c-073-streaming-crc32-integrity-check.c)
**Deterministinen testi:** [`tests/c-073-streaming-crc32-integrity-check.sh`](../../tests/c-073-streaming-crc32-integrity-check.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Oppitunnin lopuksi sinun pitäisi osata laskea nimenomaan ei-kryptografinen CRC-32-tyylinen eheysarvo tavujonolle.

## Kertaus harjoituksena

1. Miksi tavupuskurilla täytyy olla nimenomainen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka täytyy tarkistaa ennen sen tuloksen käyttämistä.
4. Muista yksi täsmällinen ilmaus edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

CRC havaitsee monia tahattomia muutoksia, mutta se ei ole todennus, salaus tai kryptografinen tiiviste.

Komppaniohjelma käsittelee raakatavuja laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan muuttamista. Koodi on siirrettävää ISO C17 -tasoa ja jättää tahallaan pois shellcoden, natiivikoodin lataimet, mielivaltaisen assembly-suorituksen, prosessin injisoinnin, itseään muokkaavan koodin ja todellisen kryptografian toteutukset.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational integrity check only: not a MAC or cryptographic hash. */
static uint32_t educational_crc(const uint8_t *data, size_t len)
{
    uint32_t crc = UINT32_C(0xFFFFFFFF);
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ ((crc & 1U) ? UINT32_C(0xEDB88320) : 0U);
    }
    return ~crc;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(1), UINT8_C(2), UINT8_C(3)};
    uint32_t value = educational_crc(data, sizeof data);
    if (printf("C 073 educational check: %08" PRIX32 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-073-streaming-crc32-integrity-check \
  examples/c/c-073-streaming-crc32-integrity-check.c
./c-073-streaming-crc32-integrity-check
```

Odotettu tuloste:

```text
C 073 educational check: 55BC801D
```

## Ohjattu harjoittelu

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskuriplus-kapasiteetti -sopimukset.
2. Etsi aikaisin hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` tavupuskurin lopetuksena.
4. Kerro, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelisi hajautusta tai eheyttä — erottele opetusesimerkit ja aidot kryptografiset suojaukset.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty rajatapaussyöte. Säilytä olemassa olevat argumenttitarkistukset, nimenomaiset pituudet ja tulostuksen paluuarvotarkistukset. Jos muutat mitään allokointiaritmetiikkaa, lisää laskettua ylivuodon tarkistus ennen laskutoimitusta.

## Varmennus

Aja deterministinen testi:

```sh
./tests/c-073-streaming-crc32-integrity-check.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-073-sanitized \
  examples/c/c-073-streaming-crc32-integrity-check.c
./c-073-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat esittää tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetusesimerkin eheydestä tai leikkimielisen hajautuksen asianmukaisesta todellisen maailman kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut saattavat sisältää `0x00`, joten sentinelin etsimiseen perustuva tarkistus ei ole kelvollinen pituuden laskenta.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden arvojen käyttöä.
4. Validoitu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
