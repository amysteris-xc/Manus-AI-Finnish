# C 067 — Kirjoita ja tarkista binääritietueet

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–066
**Kumppaniohjelma:** [`examples/c/c-067-write-then-verify-binary-records.c`](../../examples/c/c-067-write-then-verify-binary-records.c)
**Determinismin testi:** [`tests/c-067-write-then-verify-binary-records.sh`](../../tests/c-067-write-then-verify-binary-records.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata kirjoittaa kiinteän pituisia binääritietueita ja lukea ne takaisin siten, että objektien määrät tarkistetaan.

## Kertauskysymykset

1. Miksi tavupuskurin kohdalla on kannettava nimenomainen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on voimassa `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen sen tuloksen käyttämistä.
4. Muista yksi tarkka ilmaisu edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Kirjoituksen tulos ja myöhempi lukutulos ovat erillisiä sopimuksia, ja molemmat on varmistettava.

Kumppaniohjelma käsittelee raakabittejä lasketun pituisina datoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan muuttamista. Ohjelma on siirrettävä ISO C17 -koodi ja jättää tahallisesti pois shellcoden, natiivikoodin lataimet, mielivaltaisen kokoelmakoodin suorittamisen, prosessin injisoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_and_read_demo(const char *path, uint8_t *out)
{
    const uint8_t value = UINT8_C(68);
    FILE *stream = fopen(path, "wb");
    if (stream == NULL) return 0;
    if (fwrite(&value, sizeof value, 1, stream) != 1 || fclose(stream) != 0) return 0;
    stream = fopen(path, "rb");
    if (stream == NULL) return 0;
    if (fread(out, sizeof *out, 1, stream) != 1 || fclose(stream) != 0) return 0;
    return 1;
}

int main(void)
{
    uint8_t value = 0;
    if (!write_and_read_demo("lesson.bin", &value)) return EXIT_FAILURE;
    if (printf("C 067 verified file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-067-write-then-verify-binary-records \
  examples/c/c-067-write-then-verify-binary-records.c
./c-067-write-then-verify-binary-records
```

Odotettu tulostus:

```text
C 067 verified file byte: 68
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskuri-plus-kapasiteetti‑sopimukset.
2. Etsi aikaisin tapahtuva hylätty väärinmuotoisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabit­tapuskurin lopetuksena.
4. Ilmoita mikä tuloste kirjoitetaan vasta validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty syöte ja yksi hylätty rajatapaus. Säilytä olemassa olevat argumenttitarkistukset, nimenomaiset pituudet ja tuloksenpalautusten tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-067-write-then-verify-binary-records.sh
```

Käännä ja aja sanitisaattoreiden kanssa, missä saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-067-sanitized \
  examples/c/c-067-write-then-verify-binary-records.c
./c-067-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat kertoa tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tuotoksen ja erottaa opetusesimerkit tai "lelu"-hajautukset todellisesta, tuotantokelpoisesta kryptografiasta.

## Vihje: vastaukset ja mallivastaus

1. Raw bytes may include `0x00`, so a sentinel scan is not a valid length calculation.
   Raakabittiavaruus voi sisältää `0x00`, joten sentinelliskannaus ei ole pätevä pituuden laskentatapa.
2. The forward traversal condition is `index < count`.
   Eteenpäin kulkemisen ehto on `index < count`.
3. I/O, allocation, parsing, and formatting results must be checked before their outputs are used.
   I/O:n, allokaation, jäsentämisen ja muotoilun palautusarvot on tarkistettava ennen niiden tulosten käyttämistä.
4. A validated program changes output state only after every prerequisite succeeds.
   Vahvistettu ohjelma muuttaa ulostulotilaa vasta kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava suunniteltu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/ulostulo — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
