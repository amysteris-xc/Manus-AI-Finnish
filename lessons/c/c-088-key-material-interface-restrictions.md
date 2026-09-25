# C 088 — Avainmateriaalin rajapintarajoitukset

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–087
**Seurantaohjelma:** [`examples/c/c-088-key-material-interface-restrictions.c`](../../examples/c/c-088-key-material-interface-restrictions.c)
**Deterministinen testi:** [`tests/c-088-key-material-interface-restrictions.sh`](../../tests/c-088-key-material-interface-restrictions.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Oppitunnin lopuksi sinun pitäisi osata mallintaa tavupohjainen API siten, että se hylkää tyhjät tai liian suuret avainta muistuttavat syötteet ilman, että se tallentaa todellisia salaisuuksia.

## Noutoharjoitus (retrieval practice)

1. Miksi tavupuskuriin on liitettävä eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen kuin sen tulosta käytetään.
4. Muistatko yhden tarkan lauseen edellisestä venäjänkielisestä oppitunnista?

## Keskeinen käsite

Tämä on rajapintaharjoitus, ei avainhallinta; todelliset salaisuudet vaativat vakiintuneita kirjastoja ja järjestelmätason kontrollimekanismeja.

Seurantaohjelma käsittelee raakatavut laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Ohjelma on kannettava ISO C17 -ohjelma ja jättää tahallaan pois shellcoden, natiivikoodin lataajat, mielivaltaisen assemblyn suorittamisen, prosessin väliininjectoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

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
    const uint8_t source[] = {UINT8_C(0), UINT8_C(88), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 088 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-088-key-material-interface-restrictions \
  examples/c/c-088-key-material-interface-restrictions.c
./c-088-key-material-interface-restrictions
```

Odotettu tuloste:

```text
C 088 copied bytes: 0 88 255
```

## Ohjattu harjoittelu

1. Tunnista ohjelmasta kaikki pointer-plus-length- tai buffer-plus-capacity-sopimukset.
2. Löydä aikaisin tapahtuva epämääräisen syötteen hylkäysehto.
3. Selitä, miksi ohjelma ei pidä `0x00` raakatavupuskurin loppumerkkinä.
4. Määritä mikä tuloste kirjoitetaan vain sen jälkeen, kun validointi on onnistunut.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi lisädeterministinen hyväksytty syöte ja yksi hylättävä reunatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteiden palautusarvojen tarkistukset. Jos muutat minkään varauksen aritmetiikkaa, lisää laskentaa edeltävä ylivuodon tarkistus.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-088-key-material-interface-restrictions.sh
```

Käännä ja suorita sanitisaattoreilla, jos ympäristö tukee:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-088-sanitized \
  examples/c/c-088-key-material-interface-restrictions.c
./c-088-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetustarkoituksessa tehdyt eheys- tai leikkimieliset hajautusesimerkit tosikäyttöön soveltuvasta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinel-skannaus ei ole pätevä pituuslaskelma.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O-, varaus-, jäsennys- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validointinsa läpäissyt ohjelma muuttaa ulostulotilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
