# C 085 — Tekstikoodaus ja tavurajojen kertaus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–084
**Seurantaohjelma:** [`examples/c/c-085-text-encoding-and-byte-boundaries-review.c`](../../examples/c/c-085-text-encoding-and-byte-boundaries-review.c)
**Deterministinen testi:** [`tests/c-085-text-encoding-and-byte-boundaries-review.sh`](../../tests/c-085-text-encoding-and-byte-boundaries-review.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata erottaa UTF-8-tyyppiset tekstitavut C-merkkijonoihin liittyvistä oletuksista pituustiedon sisältävässä puskurirajapinnassa.

## Muistiharjoitus

1. Miksi tavupuskurin täytyy kantaa eksplisiittinen `size_t` pituustieto sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen sen tuloksen käyttämistä.
4. Muista yksi tarkka lause edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Tekstikoodaus on dataa; raakatavujono tarvitsee pituuden, vaikka esityskerros käyttäisi merkkijonoja.

Seurantaohjelma käsittelee raakatavut pituustiedollisena datana. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan siirtämistä eteenpäin. Ohjelma on siirrettävä ja toteutettu ISO C17:llä, ja se sulkee tarkoituksella pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembly-koodin suorittamisen, prosessin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Esimerkki

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
    const uint8_t source[] = {UINT8_C(0), UINT8_C(85), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 085 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-085-text-encoding-and-byte-boundaries-review \
  examples/c/c-085-text-encoding-and-byte-boundaries-review.c
./c-085-text-encoding-and-byte-boundaries-review
```

Odotettu tuloste:

```text
C 085 copied bytes: 0 85 255
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteetti -sopimukset.
2. Etsi ensimmäinen kohta, jossa virheellinen syöte hylätään.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuna.
4. Ilmoita, mikä tuloste kirjoitetaan vasta validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aiheessa käsitellään hashauksen tai eheyden aiheita.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä lisäsyöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuksen palautusarvojen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuototarkistus ennen laskentaa.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-085-text-encoding-and-byte-boundaries-review.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuki on saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-085-sanitized \
  examples/c/c-085-text-encoding-and-byte-boundaries-review.c
./c-085-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurisopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetustarkoitukseen tarkoitetut eheys- tai leluhashaus-esimerkit todellisesta, tuotantokäyttöön soveltuvasta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut saattavat sisältää `0x00`, joten sentinelliskannaus ei ole pätevä pituuden laskentatapa.
2. Eteenpäin kulun ehto on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validioitu ohjelma muuttaa tulostustilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraavassa aikataulutetussa oppitunnissa palataan venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syötön ja tulostuksen toiminnot — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
