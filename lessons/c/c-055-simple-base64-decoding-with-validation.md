# C 055 — Yksinkertainen Base64-purku ja validointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–054 ja välittömästi edeltävät oppitunnit
**Lisäohjelma:** [`examples/c/c-055-simple-base64-decoding-with-validation.c`](../../examples/c/c-055-simple-base64-decoding-with-validation.c)
**Deterministinen testi:** [`tests/c-055-simple-base64-decoding-with-validation.sh`](../../tests/c-055-simple-base64-decoding-with-validation.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopussa osaat purkaa 4 Base64-merkkiä takaisin kolmeksi raakatavuksi ja varmistaa rajat sekä merkkien oikeellisuuden.

## Muistiharjoitus

1. Miksi tavupuskurit on ilmaistava eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin päätemerkkiin perustuvaa terminointia?
2. Ilmoita turvallinen läpikäyntiehto `count` alkiolle.
3. Selitä, miksi I/O- ja muunnosfunktion paluuarvot on tarkistettava ennen tulosten käyttämistä.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuna raakadatana. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivilataimia, prosessi-injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int b64_idx(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

static int b64_decode4(const char in[4], uint8_t out[3])
{
    int i0 = b64_idx(in[0]), i1 = b64_idx(in[1]), i2 = b64_idx(in[2]), i3 = b64_idx(in[3]);
    if (i0 < 0 || i1 < 0 || i2 < 0 || i3 < 0) return 0;
    out[0] = (uint8_t)((i0 << 2) | (i1 >> 4));
    out[1] = (uint8_t)(((i1 & 0x0F) << 4) | (i2 >> 2));
    out[2] = (uint8_t)(((i2 & 3) << 6) | i3);
    return 1;
}

int main(void)
{
    uint8_t dec[3] = {0};
    if (!b64_decode4("TWFu", dec)) return EXIT_FAILURE;
    if (printf("Decoded: %c%c%c\n", dec[0], dec[1], dec[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-055-simple-base64-decoding-with-validation \
  examples/c/c-055-simple-base64-decoding-with-validation.c
./c-055-simple-base64-decoding-with-validation
```

Odotettu tuloste:

```text
Decoded: Man
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskureiden rajat.
2. Etsi jokainen paluuarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellisen syötteen ehto, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa siten, että se validoi jonkin lisäreunatapauksen tai testisyötteen. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-055-simple-base64-decoding-with-validation.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos järjestelmä tukee:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-055-sanitized \
  examples/c/c-055-simple-base64-decoding-with-validation.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen turvamekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Silmukan eturaja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakenne-ehdot ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava ajastettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
