# C 100 — Tuotantokryptokirjaston integrointimuistiinpanot

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–099
**Seurantaohjelma:** [`examples/c/c-100-production-crypto-library-integration-notes.c`](../../examples/c/c-100-production-crypto-library-integration-notes.c)
**Deterministinen testi:** [`tests/c-100-production-crypto-library-integration-notes.sh`](../../tests/c-100-production-crypto-library-integration-notes.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun pitäisi pystyä valmistamaan kapea adaptointirajapinta ylläpidettävälle kryptografiselle kirjastolle ilman kryptografian toteuttamista.

## Muistiharjoitus

1. Miksi tavutaulukon (byte buffer) on kannettava eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muistatko yhden tarkan ilmauksen edelliseltä venäjänkieliseltä oppitunnilta?

## Keskeinen käsite

Todellisille datoille valitse vakiintunut kirjasto, seuraa sen dokumentaatiota, käytä testivektoreita ja hanki arvionti; älä koskaan korvaa tätä leikkiesimerkillä.

Seurantaohjelma käsittelee raakabyttejä laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan etenemistä. Se on kannettava ISO C17 -koodia eikä sisällä tarkoituksella shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessin injektointia, itseään muuttavaa koodia tai todellista kryptografian toteutusta.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one_byte(const uint8_t *data, size_t len, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (len != 1) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(100)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 100 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-100-production-crypto-library-integration-notes \
  examples/c/c-100-production-crypto-library-integration-notes.c
./c-100-production-crypto-library-integration-notes
```

Odotettu tulostus:

```text
C 100 status-checked byte: 100
```

## Ohjattu harjoitus

1. Tunnista jokainen osoitin-plus-pituus- tai puskurin-plus-kapasiteetti-sopimus ohjelmassa.
2. Etsi aikaisin hylätty virheellisesti muodostetun syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabytepuskuriin päättymismerkkinä.
4. Kerro, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaa, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelisi hajautusta tai eheyden tarkistusta.

## Oma harjoitus

Lisää yksi deterministinen hyväksytty syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteen-palautuksen tarkistukset. Jos muutat minkään allokaation aritmetiikkaa, lisää laskelmaa ennen tarkistettu ylivuotoehto.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-100-production-crypto-library-integration-notes.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos ympäristö tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-100-sanitized \
  examples/c/c-100-production-crypto-library-integration-notes.c
./c-100-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavataulukon sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opettavaisen eheystarkistusesimerkin tai leikkimielisen hajautuksen todellisesta maailman kryptografisesta suojasta.

## Spoileri: vastaukset ja mallivastaus

1. Raakabytet voivat sisältää `0x00`, joten päättymissentinellin skannaus ei ole pätevä pituuden laskenta.
2. Eteenpäin kulkeva läpikäyntiehtoa on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttöä.
4. Validoinnin läpäissyt ohjelma muuttaa ulostulotilaa vasta kaikkien edellytysten onnistuttua.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte ja tuloste — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
