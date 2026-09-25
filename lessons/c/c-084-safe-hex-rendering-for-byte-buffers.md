# C 084 — Turvallinen heksanäyttö tavupuskurille

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–083
**Seurantaohjelma:** [`examples/c/c-084-safe-hex-rendering-for-byte-buffers.c`](../../examples/c/c-084-safe-hex-rendering-for-byte-buffers.c)
**Deterministinen testi:** [`tests/c-084-safe-hex-rendering-for-byte-buffers.sh`](../../tests/c-084-safe-hex-rendering-for-byte-buffers.sh)
**Tekijä:** Manus AI

## Mitä osaat oppitunnin jälkeen

Oppitunnin lopuksi sinun pitäisi osata tulostaa pituudella määritetty tavupuskuri heksadesimaali-esityksenä ilman, että sitä käsitellään C-merkkijonona.

## Muistiharjoitus

1. Miksi tavupuskuriin on liitettävä eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edelliseltä Russian-opetuskerralta.

## Keskeinen käsite

Heksatulostuksessa käytetään nimenomaisia tavumääriä ja kiinteän levyisiä etumerkittömiä (unsigned) muotoiluja.

Seurantaohjelma käsittelee raakatavut pituudella määritettynä datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan edistämistä. Ohjelma on siirrettävä ISO C17 -yhteensopiva ja se tarkoituksellisesti sulkee pois shellcoden, natiivikoodin lataajat, mielivaltaisen assemblyn suorittamisen, prosessien injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_hex(const uint8_t *data, size_t len)
{
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i)
        if (printf("%02" PRIX8, data[i]) < 0) return 0;
    return 1;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(0), UINT8_C(84), UINT8_C(255)};
    if (printf("C 084 hex: ") < 0 || !print_hex(data, sizeof data) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-084-safe-hex-rendering-for-byte-buffers \
  examples/c/c-084-safe-hex-rendering-for-byte-buffers.c
./c-084-safe-hex-rendering-for-byte-buffers
```

Odotettu tuloste:

```text
C 084 hex: 0054FF
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta jokainen osoitin-plus-pituus- tai puskurin-plus-kapasiteetti -sopimus.
2. Etsi aikaisin tapahtuva hylätty virheellinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuna.
4. Ilmoita mikä tulostus kirjoitetaan vasta kun validointi onnistuu.
5. Kuvaile miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi ylimääräinen deterministinen hyväksytty syöte ja yksi hylätty rajatapaus. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuksen paluuarvojen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuototarkastus ennen laskutoimitusta.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-084-safe-hex-rendering-for-byte-buffers.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos ympäristö tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-084-sanitized \
  examples/c/c-084-safe-hex-rendering-for-byte-buffers.c
./c-084-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opettavaisen eheys- tai leikkihajautusesimerkin sopivasta todellisesta kryptografisesta suojauksesta.

## Ratkaisuvihjeet: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinel-skannaus ei ole pätevä pituuden laskentamenetelmä.
2. Eteenpäin kulkevan läpikäynnin ehto on `index < count`.
3. I/O-, varaus-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validointi läpäissyt ohjelma muuttaa tulostustilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-ketjuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulostulo — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
