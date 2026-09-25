# C 061 — Päällekkäiset tavuvälit käyttäen `memmove`

**Arvioitu opiskelu-aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–060
**Esimerkkiohjelma:** [`examples/c/c-061-memmove-and-overlapping-byte-ranges.c`](../../examples/c/c-061-memmove-and-overlapping-byte-ranges.c)
**Deterministinen testi:** [`tests/c-061-memmove-and-overlapping-byte-ranges.sh`](../../tests/c-061-memmove-and-overlapping-byte-ranges.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän tunnin jälkeen

Tämän oppitunnin jälkeen osaat siirtää laskettua tavuväliä turvallisesti, myös kun lähde- ja kohdealueet ovat päällekkäisiä.

## Muistiharjoitus

1. Miksi tavupuskuriin täytyy liittää eksplisiittinen `size_t` pituus sen sijaan, että luotetaan `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen sen tuloksen käyttämistä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Käytä `memmove` vain sen jälkeen, kun lähdealueen ja kohteen kapasiteetin kelvollisuus on varmennettu.

Esimerkkiohjelma käsittelee raakatavuja laskettua tietona. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Koodi on siirrettävää ISO C17 -koodia, ja se sulkee tietoisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembler-koodin suorituksen, prosessiin injisoinnin, itsensä muokkaavan koodin sekä todellisen kryptografisen toteutuksen.

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
    const uint8_t source[] = {UINT8_C(0), UINT8_C(61), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 061 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-061-memmove-and-overlapping-byte-ranges \
  examples/c/c-061-memmove-and-overlapping-byte-ranges.c
./c-061-memmove-and-overlapping-byte-ranges
```

Odotettu tuloste:

```text
C 061 copied bytes: 0 61 255
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki pointer-plus-length- tai buffer-plus-capacity -sopimukset.
2. Etsi aikaisin tapahtuva hylätty virheellinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuna.
4. Kerro, mikä tuloste kirjoitetaan vain varmennuksen onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi ylimääräinen deterministinen kelvollinen syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuksen palautusarvotarkistukset. Jos muutat mitään varauslaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-061-memmove-and-overlapping-byte-ranges.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-061-sanitized \
  examples/c/c-061-memmove-and-overlapping-byte-ranges.c
./c-061-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurisopimuksen, tunnistaa hylätyn syötteen, selittää ohjelman tuotoksen ja erottaa opetus- tai lelu-eheys-/hajautusesimerkit sopivasta todellisen maailman kryptografisesta suojauksesta.

## Vastausvinkit: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten terminointiin perustuva skannaus ei ole kelvollinen pituuden laskenta.
2. Etenevän läpikäynnin ehto on `index < count`.
3. I/O:n, muistinvarauksen, jäsentamisen ja muotoilun tulokset on tarkistettava ennen niiden tuottaman datan käyttöä.
4. Varmennettu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset on suoritettu onnistuneesti.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte- ja tulostusoperaatiot — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
