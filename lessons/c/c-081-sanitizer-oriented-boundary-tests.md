# C 081 — Sanitizer-ohjatut reunatestit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–080
**Seurantaohjelma:** [`examples/c/c-081-sanitizer-oriented-boundary-tests.c`](../../examples/c/c-081-sanitizer-oriented-boundary-tests.c)
**Deterministinen testi:** [`tests/c-081-sanitizer-oriented-boundary-tests.sh`](../../tests/c-081-sanitizer-oriented-boundary-tests.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin loppuun mennessä osaat harjoittaa nollapituisia, täyteen kapasiteettiin meneviä ja hylättyjä kapasiteettirajoja sisältäviä tavusyötteitä tiukkojen käännösajan diagnostiikkatyökalujen alle.

## Muistiharjoitus

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luottaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen sen arvon käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Sanitisaattorit täydentävät testejä havaitsemalla määrittelemättömän käyttäytymisen, jonka tulosvertailut saattavat jättää huomaamatta.

Seurantaohjelma käsittelee raakatavuja laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan päivittämistä. Se on kannettava ISO C17 -ohjelma ja jättää tarkoituksellisesti pois shellcode-, natiivikoodin lataajiin liittyvät, mielivaltaisen assembly-koodin suorittamisen, prosessin injektion, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_bounded(const uint8_t *src, size_t len, uint8_t *dst, size_t cap)
{
    if ((src == NULL && len != 0) || dst == NULL || len > cap) return 0;
    if (len != 0) memmove(dst, src, len);
    return 1;
}

int main(void)
{
    const uint8_t source[] = {UINT8_C(0), UINT8_C(81), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 081 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-081-sanitizer-oriented-boundary-tests \
  examples/c/c-081-sanitizer-oriented-boundary-tests.c
./c-081-sanitizer-oriented-boundary-tests
```

Odotettu tuloste:

```text
C 081 copied bytes: 0 81 255
```

## Ohjattu harjoittelu

1. Tunnista kaikki ohjelmassa esiintyvät osoitin-plus-pituus- tai puskuri-plus-kapasiteetti -sopimukset.
2. Etsi aikaisin tapahtuva hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuna.
4. Määrittele, mikä tuloste kirjoitetaan vasta vahvistuksen onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aiheena ovat hajautus tai eheys — ero opetusesimerkin/leikkinäytteen ja oikean maailman kryptografisen suojauksen välillä.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty reunatapaus. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteiden palautustarkistukset. Jos muutat muistiallokointilaskelmia, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-081-sanitizer-oriented-boundary-tests.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-081-sanitized \
  examples/c/c-081-sanitizer-oriented-boundary-tests.c
./c-081-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat esittää tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetukselliset eheys- tai leikkinäytteet soveltuvasta oikean maailman kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellihaku ei ole pätevä pituuden laskentatapa.
2. Etenevän läpikäynnin ehto on `index < count`.
3. I/O-, allokointi-, jäsennys- ja muotoilusubrutien palautusarvot on tarkistettava ennen niiden tulosten käyttämistä.
4. Vahvistettu ohjelma muuttaa ulostulotilaa vain kaikkien edellytysten onnistuttua.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
