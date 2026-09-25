# C 069 — Safe `size_t` Multiplication

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–068
**Companion program:** [`examples/c/c-069-safe-size-t-multiplication.c`](../../examples/c/c-069-safe-size-t-multiplication.c)
**Deterministic test:** [`tests/c-069-safe-size-t-multiplication.sh`](../../tests/c-069-safe-size-t-multiplication.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat havaita kokojen kertolaskun ylivuodon ennen kuin lasket tietueen tallennustarpeita.

## Kertaus

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luottaisit `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka täytyy tarkistaa ennen tuloksen käyttöä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Älä koskaan laske `count * item_size` ennen kuin olet varmistanut, että se mahtuu `size_t`.

Seuraava esimerkkiohjelma käsittelee raakatavuja laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan päivittämistä. Ohjelma on siirrettävä ISO C17 -yhteensopiva ja tarkoituksella poissulkee shellcode-toteutukset, natiivikoodin lataimet, mielivaltaisen assembly-koodin suorittamisen, prosessiin injektoinnin, itseään muokkaavan koodin ja todelliset kryptografiset toteutukset.

## Käytännön esimerkki

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
    const uint8_t source[] = {UINT8_C(0), UINT8_C(69), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 069 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-069-safe-size-t-multiplication \
  examples/c/c-069-safe-size-t-multiplication.c
./c-069-safe-size-t-multiplication
```

Odotettu tuloste:

```text
C 069 copied bytes: 0 69 255
```

## Ohjatut harjoitustehtävät

1. Tunnista ohjelmasta kaikki osoitin+pituus- ja puskuri+kapasiteetti-sopimukset.
2. Etsi varhaisin virheellisen syötteen hylkäävä ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin lopetusmerkkinä.
4. Mainitse mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka se käsittelisi hajautusta tai eheystarkistusta — ero opetusmielisten esimerkkien ja todellisen kryptografian välillä on olennainen.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja paluuarvojen tarkistukset tulostuksen yhteydessä. Jos muutat mitään muistinvarauslaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Tarkistus

Suorita deterministinen testi:

```sh
./tests/c-069-safe-size-t-multiplication.sh
```

Käännä ja suorita sanitizerien kanssa, jos ympäristö tukee:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-069-sanitized \
  examples/c/c-069-safe-size-t-multiplication.c
./c-069-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetusluontoiset eheys- tai hajautusesimerkit todellisesta, tuotantokelpoisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten lopetusmerkin etsintä ei ole kelvollinen pituuden laskentatapa.
2. Etenemisen ehtona on `index < count`.
3. I/O-, muistinvaraus-, jäsennys- ja muotoilufunktioiden paluuarvot on tarkistettava ennen niiden tulosten käyttöä.
4. Vahvistettu ohjelma muuttaa ulostilaa vain, kun kaikki ennakkoehdot ovat onnistuneet.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
