# C 063 — Versioitu binääriotsikon validointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–062
**Kumppaniohjelma:** [`examples/c/c-063-versioned-binary-header-validation.c`](../../examples/c/c-063-versioned-binary-header-validation.c)
**Deterministinen testi:** [`tests/c-063-versioned-binary-header-validation.sh`](../../tests/c-063-versioned-binary-header-validation.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat validoida tiiviin otsikon magic‑arvon, version ja hyötykuorman pituuskentän ennen hyötykuorman käsittelyä.

## Kertauskysymykset

1. Miksi tavupuskurin on kannettava eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C‑kirjaston paluuarvo, joka on tarkistettava ennen tuloksen käyttöä.
4. Muista yksi täsmällinen lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Versioitu jäsentäjä tarkistaa jokaisen kentän ennen kuin laskee mitään siirtymä- tai varauskokoa.

Kumppaniohjelma käsittelee raakabyttejä eksplisiittisesti määrällisenä datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan etenemistä. Ohjelma on kannettava ISO C17 ‑yhteensopiva, ja se nimenomaisesti sulkee pois shellcoden, native‑code loaderit, mielivaltaisen assembly‑koodin suorittamisen, prosessiin injektoinnin, itseään muuttavan koodin ja todellisen kryptografisen toteutuksen — tämä on opetusesimerkki, ei tuotantotason kryptografiaa.

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
    const uint8_t field[] = {UINT8_C(0), UINT8_C(63)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 063 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-063-versioned-binary-header-validation \
  examples/c/c-063-versioned-binary-header-validation.c
./c-063-versioned-binary-header-validation
```

Odotettu tuloste:

```text
C 063 decoded field: 63
```

## Ohjattu harjoitus

1. Tunnista jokainen ohjelmassa esiintyvä osoitin‑plus‑pituus tai puskurin‑plus‑kapasiteetti ‑sopimus.
2. Etsi aikaisin tapahtuva hylätty virhetilanne, jossa syöte todetaan vääräksi.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabufferin loppuna.
4. Määritä, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aihe koskettaisi hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen hyväksytty syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuspaluuarvojen tarkistukset. Jos muutat mitään varauslaskentaa, lisää ennen laskentaa tarkistettu ylivuotoehto.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-063-versioned-binary-header-validation.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-063-sanitized \
  examples/c/c-063-versioned-binary-header-validation.c
./c-063-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat luetella tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetustarkoitukseen tarkoitetut yksinkertaiset eheys‑ tai hajautusesimerkit todellisesta tuotantokelpoisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakadatat voivat sisältää `0x00`, joten päätemerkkiskannaus ei kelpaa pituuden laskemiseen.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O‑, allokointi‑, jäsennys‑ ja formaattitulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validioitu ohjelma muuttaa tulostustilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C‑tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C‑pohjainen työkaluketjun koventamisen pikatarkistuslista — OWASP"
