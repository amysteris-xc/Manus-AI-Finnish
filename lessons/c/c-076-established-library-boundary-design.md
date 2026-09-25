# C 076 — Vakiintuneen kirjastorajan suunnittelu

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–075
**Esimerkkiohjelma:** [`examples/c/c-076-established-library-boundary-design.c`](../../examples/c/c-076-established-library-boundary-design.c)
**Deterministinen testi:** [`tests/c-076-established-library-boundary-design.sh`](../../tests/c-076-established-library-boundary-design.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat suunnitella kapean tavataulukon (byte-buffer) rajapinnan, joka voi kutsua vakiintunutta kryptografista kirjastoa ilman primitiivien uudelleenimplementointia.

## Muistiharjoitus

1. Miksi tavataulukolla on oltava eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto osoittaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston palautusarvo, joka täytyy tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edeltävältä venäjänkieliseltä oppitunnilta.

## Ydinajatus

Tuotantokäytön kryptografia kuuluu tarkastellun kirjastorajan taakse; tämä oppitunti tahallisesti ei toteuta kryptografista primitiiviä.

Esimerkkiohjelma käsittelee raakatavut laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan edistämistä. Se on siirrettävä ISO C17 -yhteensopiva ja sulkee tahallisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembly-suorituksen, prosessin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Esimerkki

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
    const uint8_t input[] = {UINT8_C(76)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 076 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja käynnistä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-076-established-library-boundary-design \
  examples/c/c-076-established-library-boundary-design.c
./c-076-established-library-boundary-design
```

Odotettu tuloste:

```text
C 076 status-checked byte: 76
```

## Ohjattu harjoitus

1. Tunnista ohjelmassa jokainen osoitin+pituus- tai puskuri+kapasiteetti-sopimus.
2. Etsi varhaisin hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavut-puskurin loppumerkkinä.
4. Mainitse, mikä tuloste kirjoitetaan vain sen jälkeen, kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokäytön kryptografiaa, vaikka aiheena käsitellään hajautusta tai eheyttä.

## Itsenäinen tehtävä

Lisää yksi ylimääräinen deterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttien tarkistukset, eksplisiittiset pituudet ja tulosteen-palautusarvon tarkistukset. Jos muutat mitään allokaatiolaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-076-established-library-boundary-design.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-076-sanitized \
  examples/c/c-076-established-library-boundary-design.c
./c-076-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavataulukon sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa pedagogiset eheys- tai leikkihajautusesimerkit asianmukaisesta todellisen maailman kryptografisesta suojasta.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut saattavat sisältää `0x00`, joten sentinellin skannaus ei ole kelvollinen pituuden laskenta.
2. Eteenpäin kulkeva läpikäyntiehto on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen kuin niiden tuloksia käytetään.
4. Vahvistettu ohjelma muuttaa tulostetilaa vasta sitten, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraavassa oppitunnissa palataan venäjänkieliseen linjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
