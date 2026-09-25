# C 064 — Tavujärjestyksen valinta sanakentille

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–063
**Kumppaniohjelma:** [`examples/c/c-064-endianness-dispatch-for-word-fields.c`](../../examples/c/c-064-endianness-dispatch-for-word-fields.c)
**Deterministinen testi:** [`tests/c-064-endianness-dispatch-for-word-fields.sh`](../../tests/c-064-endianness-dispatch-for-word-fields.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Oppitunnin lopussa osaat purkaa 16-bittisen kentän käyttäen eksplisiittisesti valittua tavujärjestystä sen sijaan, että luottaisit isäntäjärjestelmän muistiesitykseen.

## Muistiharjoitus

1. Miksi tavupuskurin on kannettava eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Tavujärjestys kuuluu formaattisopimukseen, ei tyyppimuunnokseen tai isäntäriippuvaiseen muistiasetteluun.

Kumppaniohjelma käsittelee raakabyttejä laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan päivittämistä. Se on kannettava ISO C17 -ohjelma ja jättää tahallisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembly-suorituksen, prosessin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografian toteutuksen.

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
    const uint8_t field[] = {UINT8_C(0), UINT8_C(64)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 064 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-064-endianness-dispatch-for-word-fields \
  examples/c/c-064-endianness-dispatch-for-word-fields.c
./c-064-endianness-dispatch-for-word-fields
```

Odotettu tuloste:

```text
C 064 decoded field: 64
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki pointer-plus-length- tai buffer-plus-capacity-sopimukset.
2. Etsi aikaisin hylätty väärinmuotoisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` tavupuskurin loppuna.
4. Ilmoita, mikä tuloste kirjoitetaan vasta kun validointi on onnistunut.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotasoista kryptografiaa, vaikka aiheena olisi hajautus tai eheyden tarkistus.

## Itsenäinen tehtävä

Lisää yksi lisädeterministinen hyväksytty syöte ja yksi hylätty reunatapaussyöte. Säilytä olemassa olevat argumenttien tarkistukset, eksplisiittiset pituudet ja tuloste-paluuarvon tarkistukset. Jos muutat mitään allokointilaskentaa, lisää ennen laskentaa tarkistettu ylivuotoehto.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-064-endianness-dispatch-for-word-fields.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-064-sanitized \
  examples/c/c-064-endianness-dispatch-for-word-fields.c
./c-064-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetusesimerkit tai leikkihajautukset todellisesta tuotantokäyttöön sopivasta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakabyteissä voi olla `0x00`, joten sentinellin etsintä ei ole pätevä pituuden laskentatapa.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O-, allokointi-, jäsennys- ja muotoilutulokset on tarkistettava ennen kuin niiden tuotos käytetään.
4. Vahvistettu ohjelma muuttaa tulostustilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti jatkaa venäjänkielistä sarjaa.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C:n tiedostojen I/O — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
