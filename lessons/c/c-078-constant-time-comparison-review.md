# C 078 — Vakioaikaisen vertailun kertaus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–077
**Seurantaohjelma:** [`examples/c/c-078-constant-time-comparison-review.c`](../../examples/c/c-078-constant-time-comparison-review.c)
**Deterministinen testi:** [`tests/c-078-constant-time-comparison-review.sh`](../../tests/c-078-constant-time-comparison-review.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat tarkastella laskettua vertailusilmukkaa, joka välttää ennenaikaisen poiston yhtä pitkien tavupuskurien vertailussa.

## Kertaus- ja muistiharjoitus

1. Miksi tavupuskuriin täytyy liittää eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen sen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Vakioaikaistyyppiset silmukat vähentävät yhtä signaaliluokkaa, mutta täydellinen turvallisuus riippuu koko järjestelmästä ja vakiintuneista kirjastoista.

Seurantaohjelma käsittelee raakatavut laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan etenemistä. Ohjelma on kannettava ISO C17 -koodia ja jättää tahallisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembly-suorituksen, prosessin injektoinnin, itseään muokkaavan koodin ja varsinaisen kryptografisen toteutuksen.

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
    const uint8_t source[] = {UINT8_C(0), UINT8_C(78), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 078 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-078-constant-time-comparison-review \
  examples/c/c-078-constant-time-comparison-review.c
./c-078-constant-time-comparison-review
```

Odotettu tuloste:

```text
C 078 copied bytes: 0 78 255
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta jokainen osoittaja+pituus- tai puskuri+kapasiteetti-sopimus.
2. Etsi ohjelmasta varhaisin ehto, joka hylkää virheellisen syötteen.
3. Selitä, miksi ohjelma ei pidä `0x00` raakaa tavupuskurin loppuna.
4. Määrittele, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja ulostulon palautustarkistukset. Jos muokkaat mitään muistinvarauslaskelmia, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-078-constant-time-comparison-review.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-078-sanitized \
  examples/c/c-078-constant-time-comparison-review.c
./c-078-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos pystyt kuvaamaan tavupuskurin sopimuksen, tunnistamaan hylätyn syötteen, selittämään tulosteen ja erottamaan opetusluonteiset eheys- tai leikkihajautusesimerkit sopivasta todellisen maailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellihaku ei ole pätevä pituuden laskenta.
2. Eteenpäin kulun ehto on `index < count`.
3. I/O:n, allokoinnin, jäsentämisen ja muotoilun tulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Vahvistettu ohjelma muuttaa ulostilaa vasta kun kaikki edellytykset on täytetty onnistuneesti.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte ja tulostus — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
