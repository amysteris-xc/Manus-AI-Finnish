# C 065 — Tarkistetut `calloc` lukumäärä- ja kokolaskelmat

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–064
**Seuralaisohjelma:** [`examples/c/c-065-calloc-count-size-overflow-checks.c`](../../examples/c/c-065-calloc-count-size-overflow-checks.c)
**Deterministinen testi:** [`tests/c-065-calloc-count-size-overflow-checks.sh`](../../tests/c-065-calloc-count-size-overflow-checks.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopussa osaat suojata laskelmia, joissa kerrotaan määrä kertaa koko, ennen nollatun taulukon varaamista.

## Muistiharjoitus

1. Miksi tavutaulukolla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka täytyy tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Varauslaskenta on turvallisuusraja: validoi kertolasku ennen sen suorittamista.

Seuralaisohjelma käsittelee raakabyteja laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan eteenpäin viemistä. Ohjelma on siirrettävä ISO C17 -tasoinen ja sulkee tahallisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assemblyn suorittamisen, prosessin injektion, itseään muokkaavan koodin ja oikean kryptografisen toteutuksen.

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
    const uint8_t source[] = {UINT8_C(0), UINT8_C(65), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 065 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-065-calloc-count-size-overflow-checks \
  examples/c/c-065-calloc-count-size-overflow-checks.c
./c-065-calloc-count-size-overflow-checks
```

Odotettu tuloste:

```text
C 065 copied bytes: 0 65 255
```

## Ohjattu harjoitus

1. Tunnista kaikki ohjelman pointer-plus-pituus- tai puskurin-plus-kapasiteetti -sopimukset.
2. Etsi aikaisin hylätty väärinmuodostettu syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavutaulukon lopetuksena.
4. Kerro, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe koskisi hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja paluuarvojen tarkistukset. Jos muutat mitään varauslaskentaa, lisää ennen laskua ylivuodon tarkistus.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-065-calloc-count-size-overflow-checks.sh
```

Käännä ja aja sanitisaattoreilla siellä, missä niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-065-sanitized \
  examples/c/c-065-calloc-count-size-overflow-checks.c
./c-065-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat ilmaista tavataulukon sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetuksellisen eheyys- tai leikkimielisen hajautusesimerkin todellisesta tuotantokelpoisesta kryptografisesta suojauksesta.

## Ratkaisut ja mallivastaus

1. Raakabyteissa voi olla `0x00`, joten sentinellin skannaus ei ole pätevä pituuslaskelma.
2. Eteenpäin kulkeva läpikäyntiehtoa on `index < count`.
3. I/O:n, varauksen, jäsentämisen ja muotoilun tulokset on tarkistettava ennen niiden käyttämistä.
4. Vahvistettu ohjelma muuttaa tulostetilaa vasta kaikkien edellytysten onnistuttua.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän levyisen kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
