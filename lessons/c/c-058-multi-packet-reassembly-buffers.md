# C 058 — Monipakettien uudelleenkoontipuskurit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–057
**Seuralaisohjelma:** [`examples/c/c-058-multi-packet-reassembly-buffers.c`](../../examples/c/c-058-multi-packet-reassembly-buffers.c)
**Deterministinen testi:** [`tests/c-058-multi-packet-reassembly-buffers.sh`](../../tests/c-058-multi-packet-reassembly-buffers.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Tunnin päätteeksi sinun pitäisi pystyä kokoamaan rajattuja pakettikatkelmia laskettuun tavupuskuriin siten, että kapasiteetin ylivuodot hylätään.

## Kertauskysymykset

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka täytyy tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Ydinajatus

Uudelleenkoontirutiinin on validoitava jokaisen katkelman pituus ennen kuin kirjoituslaskuria edistetään.

Seuralaisohjelma käsittelee raakatavut laskettuina datana. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Ohjelma on siirrettävä ISO C17 -tasoinen ja se jättää tarkoituksella pois shellcodea, natiivikoodin lataajia, mielivaltaista kokoonpanokoodin suorittamista, prosessiin injektointia, itseään muokkaavaa koodia ja todellista kryptografista toteutusta.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_bounded(const uint8_t *src, size_t len, uint8_t *dst, size_t cap)
{
    if ((src == NULL && len != 0) || dst == NULL || len > cap) return 0;
    if (len != 0) memmove(dst, src, len);
    return 1;
}

int main(void)
{
    const uint8_t source[] = {UINT8_C(0), UINT8_C(58), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 058 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-058-multi-packet-reassembly-buffers \
  examples/c/c-058-multi-packet-reassembly-buffers.c
./c-058-multi-packet-reassembly-buffers
```

Odotettu tuloste:

```text
C 058 copied bytes: 0 58 255
```

## Ohjattu harjoitus

1. Tunnista kaikki pointer-plus-length- tai buffer-plus-capacity-sopimukset ohjelmassa.
2. Etsi aikaisin tapahtuva hylätty viallinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` merkkijonoa raakatavupuskurin lopetuksena.
4. Mainitse, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aihe käsittelisi hajautusta tai eheyden tarkistusta — erottele opetuksellinen leikkikaluesimerkki ja todellinen maailmankäyttöön sopiva kryptografinen suojaus.

## Itsenäinen harjoitus

Lisää yksi lisädeterministinen hyväksytty syöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tuloste-palautusarvotarkastukset. Jos muutat mitään varauslaskentaa, lisää ennen laskentaa tarkistettu ylivuototarkistus.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-058-multi-packet-reassembly-buffers.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-058-sanitized \
  examples/c/c-058-multi-packet-reassembly-buffers.c
./c-058-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetukselliset eheys- tai leikkikaluhajautusesimerkit todellisesta maailmasta tulevasta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut saattavat sisältää `0x00`, joten sentinellitarkistus ei ole pätevä pituuden laskenta.
2. Eteenpäin kulkevan läpikäynnin ehto on `index < count`.
3. I/O-, varaus-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validoinnin läpäistyä ohjelma muuttaa ulostilaa vain kaikkien edellytysten täyttyessä.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen linjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/tuotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
