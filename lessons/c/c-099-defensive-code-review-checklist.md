# C 099 — Puolustava koodikatselmointi – tarkistuslista

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitaidot:** C 001–098
**Companion program:** [`examples/c/c-099-defensive-code-review-checklist.c`](../../examples/c/c-099-defensive-code-review-checklist.c)
**Deterministic test:** [`tests/c-099-defensive-code-review-checklist.sh`](../../tests/c-099-defensive-code-review-checklist.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat soveltaa tiivistä tarkistuslistaa pieneen tavujen parseriin ennen sen tuloksen hyväksymistä.

## Muistiharjoitus

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka täytyy tarkistaa ennen kuin sen tulosta käytetään.
4. Muista yksi täsmällinen lause edeltävältä venäjänkieliseltä oppitunnilta.

## Ydinajatus

Katselmoinnissa kysytään, onko jokaisella syötepituudella, muunnoksella, osoittimella ja palautusarvolla eksplisiittinen sopimus.

Seurantana oleva ohjelma käsittelee raakabyttejä lasketun datan tavoin. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan etenemistä. Ohjelma on siirrettävä ISO C17:ään ja siihen on tarkoituksellisesti jätetty pois shellcode, natiivikoodin lataimet, mielivaltaisen assembler-koodin suorittaminen, prosessiin injektointi, itseään muokkaava koodi sekä todellinen kryptografinen toteutus.

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
    const uint8_t input[] = {UINT8_C(99)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 099 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-099-defensive-code-review-checklist \
  examples/c/c-099-defensive-code-review-checklist.c
./c-099-defensive-code-review-checklist
```

Odotettu tulostus:

```text
C 099 status-checked byte: 99
```

## Ohjattu harjoitus

1. Tunnista kaikki ohjelman osoitin-plus-pituus- tai puskurin-plus-kapasiteetti-sopimukset.
2. Löydä aikaisin tapahtuva hylätty virheellisen syötteen tarkastus.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabytin lopetusmerkkinä.
4. Ilmoita, mikä tulostus kirjoitetaan vain sen jälkeen kun validointi on onnistunut.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokäyttöön sopivaa kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen tehtävä

Lisää yksi lisädeterministinen hyväksytty syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tuloksen-palautuksen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-099-defensive-code-review-checklist.sh
```

Käännä ja aja sanitisaattoreilla, jos tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-099-sanitized \
  examples/c/c-099-defensive-code-review-checklist.c
./c-099-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opettavaisen eheys- tai lelumaisen hajautusesimerkin oikean maailman kryptografisesta suojauksesta.

## Ratkaisut ja mallivastaus (juonipaljastus)

1. Raakabytit saattavat sisältää `0x00`, joten sentinel-skannaus ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulkeva tarkastus ehto on `index < count`.
3. I/O-, allokointi-, jäsennys- ja muotoilutulokset täytyy tarkistaa ennen kuin niiden lähtöjä käytetään.
4. Vahvistettu ohjelma muuttaa ulostulotilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen sarjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/tulostus — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
