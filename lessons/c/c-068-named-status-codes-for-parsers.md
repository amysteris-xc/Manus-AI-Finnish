# C 068 — Nimetyt tilakoodit jäsentimille

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitietovaatimukset:** C 001–067
**Seuralainen ohjelma:** [`examples/c/c-068-named-status-codes-for-parsers.c`](../../examples/c/c-068-named-status-codes-for-parsers.c)
**Deterministinen testi:** [`tests/c-068-named-status-codes-for-parsers.sh`](../../tests/c-068-named-status-codes-for-parsers.sh)
**Kirjoittaja:** Manus AI

## Mihin pystyt tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat palauttaa luettavia, yksiselitteisiä tilakoodeja tavujonon jäsentimestä epäselvien booleanien sijaan.

## Muistiharjoitus

1. Miksi tavupuskurilla pitää olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka pitää tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Kutsujat voivat erottaa virheellisesti muotoillun datan, kapasiteettivirheen ja huonot argumentit nimettyjen tilojen avulla.

Seuralainen ohjelma käsittelee raakatavut pituustietoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan edistämistä. Ohjelma on siirrettävä ISO C17 -koodi ja jättää tahallisesti pois shellcode:n, natiivikoodilataajat, mielivaltaisen assembly-koodin suorituksen, prosessin injektoinnin, itseään muuttavan koodin ja todellisen kryptografisen toteutuksen.

## Käytännön esimerkki

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
    const uint8_t input[] = {UINT8_C(68)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 068 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-068-named-status-codes-for-parsers \
  examples/c/c-068-named-status-codes-for-parsers.c
./c-068-named-status-codes-for-parsers
```

Odotettu tulostus:

```text
C 068 status-checked byte: 68
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteetti-sopimukset.
2. Sijaitse ohjelman varhaisin ehto, joka hylkää virheellisen syötteen.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin lopetuksena.
4. Ilmoita, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi uusi deterministinen hyväksytty syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttien tarkistukset, eksplisiittiset pituudet ja tulostuksen paluuarvojen tarkistukset. Jos muutat mitään varauslaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Varmistus

Aja deterministinen testi:

```sh
./tests/c-068-named-status-codes-for-parsers.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos käytettävissä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-068-sanitized \
  examples/c/c-068-named-status-codes-for-parsers.c
./c-068-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määrittää tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulostuksen ja erottaa opetukselliset eheys- tai oppimiseen tarkoitetut yksinkertaiset hajautusesimerkit oikean maailman kryptografisesta suojauksesta.

## Vastausvihjeet ja malli

1. Raakatavut voivat sisältää `0x00`, joten sentinellitarkastus ei ole pätevä pituuden laskenta.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O:n, varauksen, jäsentämisen ja muotoilun tulokset on tarkistettava ennen niiden tulosten käyttöä.
4. Vahvistettu ohjelma muuttaa tulostustilaa vasta kaikkien edellytysten onnistuessa.

## Seuraava oppitunti

Seuraavaksi suunniteltu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedoston syöttö/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisten työkaluketjujen koventamisen muistilista — OWASP"
