# C 098 — Teksti- ja tavurajojen loppuprojekti

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–097
**Kumppaniohjelma:** [`examples/c/c-098-text-and-byte-boundary-capstone.c`](../../examples/c/c-098-text-and-byte-boundary-capstone.c)
**Deterministinen testi:** [`tests/c-098-text-and-byte-boundary-capstone.sh`](../../tests/c-098-text-and-byte-boundary-capstone.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopuksi osaat yhdistää lasketut tavut, validoidut tekstin pituuskentät ja turvallisen heksadesimaalisen tarkastelun.

## Muistiharjoitus

1. Miksi tavupuskurilla pitää olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Tiedosto- tai viestimuoto voi kantaa tekstiä, mutta sen tavurajat tulee validoida ennen näyttämistä tai käsittelyä.

Kumppaniohjelma käsittelee raakadataa lasketuina tavujoukkoina. Se tarkastaa argumentit, kapasiteetit ja paluuarvot ennen tilan eteenpäin viemistä. Ohjelma on kannettava ISO C17 -tasolla ja se jättää tahallisesti pois shellcoden, natiivisten koodilataajien, mielivaltaisen assemblerin suorittamisen, prosessiin injektoinnin, itseään muuttavan koodin ja todellisen kryptografian toteutuksen.

## Käytännön esimerkki

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
    const uint8_t source[] = {UINT8_C(0), UINT8_C(98), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 098 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-098-text-and-byte-boundary-capstone \
  examples/c/c-098-text-and-byte-boundary-capstone.c
./c-098-text-and-byte-boundary-capstone
```

Odotettu tuloste:

```text
C 098 copied bytes: 0 98 255
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin+pituus- tai puskurin+kapasiteetti-sopimukset.
2. Sijaitse varhaisin hylätty vääränmuotoinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuna.
4. Ilmoita, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee tiivisteitä tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty syöte ja yksi hylätty raja-arvosyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tuloste-paluu -tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-098-text-and-byte-boundary-capstone.sh
```

Käännä ja aja sanitisaattoreiden kanssa, kun tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-098-sanitized \
  examples/c/c-098-text-and-byte-boundary-capstone.c
./c-098-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetuksellisen eheys- tai lelu­tason tiiviste­esimerkin todellisesta tuotantokryptografiasta.

## Vihjeet: vastaukset ja mallivastaus

1. Raw bytes may include `0x00`, so a sentinel scan is not a valid length calculation.
   (Raakatavut voivat sisältää `0x00`, joten sentinellin etsiminen ei ole kelvollinen pituuslaskenta.)
2. The forward traversal condition is `index < count`.
   (Eteenpäin kulkemisen ehto on `index < count`.)
3. I/O, allocation, parsing, and formatting results must be checked before their outputs are used.
   (I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttämistä.)
4. A validated program changes output state only after every prerequisite succeeds.
   (Validoitu ohjelma muuttaa tulostetilaa vasta kaikkien edellytysten onnistuessa.)

## Seuraava oppitunti

Seuraavaksi käsitellään taas venäjänkielistä polkua.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteäleveysiset kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
