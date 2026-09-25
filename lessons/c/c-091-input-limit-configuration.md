# C 091 — Syötteen enimmäisrajojen konfigurointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edeltävät tiedot:** C 001–090
**Companion program:** [`examples/c/c-091-input-limit-configuration.c`](../../examples/c/c-091-input-limit-configuration.c)
**Deterministic test:** [`tests/c-091-input-limit-configuration.sh`](../../tests/c-091-input-limit-configuration.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopussa osaat keskittää parserin maksimirajat nimettyihin vakioihin ja hylätä syötteet, jotka ylittävät ne.

## Muistiharjoitus

1. Miksi tavupuskurin täytyy sisältää nimenomainen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto osoittaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen tuloksen käyttöä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Ydinajatus

Nimetyt rajat tekevät formaatin resurssibudjetista näkyvän tarkastajille ja testeille.

Companion-ohjelma käsittelee raakabyttejä laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan muuttamista. Ohjelma on kannettava ISO C17 -mukainen ja tarkoituksellisesti poissulkee shellcodea, natiivikoodin lataajat, mielivaltaisen assembly-koodin suorituksen, prosessiin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Esimerkki käytännössä

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
    const uint8_t field[] = {UINT8_C(0), UINT8_C(91)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 091 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-091-input-limit-configuration \
  examples/c/c-091-input-limit-configuration.c
./c-091-input-limit-configuration
```

Odotettu tuloste:

```text
C 091 decoded field: 91
```

## Ohjatut harjoitukset

1. Tunnista ohjelmasta kaikki pointer-plus-length- eli puskurin-ja-kapasiteetin sopimukset.
2. Etsi aikaisin tapahtuva hylätty viallinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabytteen lopetussentinellinä.
4. Määrittele, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aiheessa käsitellään hajautusta tai eheystarkistusta.

## Itsenäinen harjoitus

Lisää yksi uusi deterministinen hyväksytty syöte ja yksi hylätty raja-arvosyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteiden paluuarvojen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-091-input-limit-configuration.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-091-sanitized \
  examples/c/c-091-input-limit-configuration.c
./c-091-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää ohjelman tuottaman tulosteen ja erottaa opetukselliset eheys- tai leikkihajautusesimerkit todellisesta käyttökelpoisesta kryptografisesta suojauksesta.

## Vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellihaku ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulun ehto on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutoimintojen tulokset on tarkistettava ennen niiden tuottamien arvojen käyttöä.
4. Validointia läpikäynyt ohjelma muuttaa tulostetilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava suunniteltu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikamuistilista — OWASP"
