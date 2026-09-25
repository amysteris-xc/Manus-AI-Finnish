# C 080 — Round-Trip Property Checks

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–079
**Seuralaisohjelma:** [`examples/c/c-080-round-trip-property-checks.c`](../../examples/c/c-080-round-trip-property-checks.c)
**Deterministinen testi:** [`tests/c-080-round-trip-property-checks.sh`](../../tests/c-080-round-trip-property-checks.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat testata kooderin ja dekooderin round-trip-kierroksen käyttäen kiinteää joukkoa turvallisia testiarvoja.

## Muistinvarainen harjoitus

1. Miksi tavutaulukon (byte buffer) mukana on oltava eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Mainitse yksi C-kirjaston paluuarvo, joka pitää tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä Russian-opetuksesta.

## Keskeinen käsite

Round-trip-testit vahvistavat ilmoitetun invariantin, mutta eivät todista kaikkien bugien poissaoloa.

Seuralaisohjelma käsittelee raakabittejä laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan etenemistä. Se on kannettava ISO C17 -ohjelma ja jättää tietoisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen asm:n ajon, prosessiin injektoinnin, itseään muuttavan koodin ja todellisen kryptografian toteutuksen.

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
    const uint8_t field[] = {UINT8_C(0), UINT8_C(80)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 080 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-080-round-trip-property-checks \
  examples/c/c-080-round-trip-property-checks.c
./c-080-round-trip-property-checks
```

Odotettu tuloste:

```text
C 080 decoded field: 80
```

## Ohjattu harjoitus

1. Tunnista kaikki ohjelmassa esiintyvät pointer-plus-length- tai buffer-plus-capacity-sopimukset.
2. Etsi aikaisin hylätty viallinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakapuskurin lopetuksena.
4. Kerro, mikä tuloste kirjoitetaan vain validaation onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä — erottele opetusesimerkit (tai lelu-esimerkit) ja todelliset kryptografiset suojaukset.

## Itsenäinen tehtävä

Lisää yksi lisädeterministinen kelvollinen syöte ja yksi hylätty rajatapaus. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostus–paluuarvon tarkistukset. Jos muutat muistiallokoinnin laskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-080-round-trip-property-checks.sh
```

Käännä ja suorita sanitizereiden kanssa, jos järjestelmä tukee:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-080-sanitized \
  examples/c/c-080-round-trip-property-checks.c
./c-080-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos pystyt kertomaan tavataulun sopimuksen, tunnistamaan hylätyn syötteen, selittämään tulosteen ja erottamaan opetuskäyttöiset eheys- tai lelu-hajautusesimerkit todellisista tuotantokäyttöisistä kryptografisista suojauksista.

## Vastausvihjeet ja mallivastaukset

1. Raakabytet voivat sisältää `0x00`, joten sentinelli-skannaus ei ole pätevä pituuden laskeminen.
2. Eteenpäin kulkevan läpikäynnin ehto on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset pitää tarkistaa ennen kuin niiden tuloksia käytetään.
4. Validoitu ohjelma muuttaa tulostetilaa vasta kaikkien edellytysten onnistuttua.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto (File I/O) — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaohje — OWASP"
