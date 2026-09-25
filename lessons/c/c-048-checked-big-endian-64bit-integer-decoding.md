# C 048 — Tarkistettu big-endian 64-bittisen kokonaisluvun dekoodaus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–047 ja välittömästi edeltävät oppitunnit
**Seurantaohjelma:** [`examples/c/c-048-checked-big-endian-64bit-integer-decoding.c`](../../examples/c/c-048-checked-big-endian-64bit-integer-decoding.c)
**Deterministinen testi:** [`tests/c-048-checked-big-endian-64bit-integer-decoding.sh`](../../tests/c-048-checked-big-endian-64bit-integer-decoding.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat dekoodata 8 big-endian-tavua uint64_t-tyyppiseksi arvoksi käyttäen eksplisiittisiä puskureiden rajoja.

## Kertaus

1. Miksi tavupuskurit on mitoitettava eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin terminaalisentelitejä?
2. Määritä turvallinen läpikäyntiehtolauseke `count` alkioille.
3. Selitä, miksi I/O- ja konversiopalautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakadatana. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivilataimia, prosessiin injektointia, shellcodea ja itseä muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int decode_u64_be(const uint8_t *in, size_t len, uint64_t *out)
{
    if (in == NULL || out == NULL || len < 8) return 0;
    uint64_t v = 0;
    for (size_t i = 0; i < 8; ++i) v = (v << 8) | in[i];
    *out = v;
    return 1;
}

int main(void)
{
    const uint8_t raw[8] = {0, 0, 0, 0, 0, 0, 1, 0};
    uint64_t val = 0;
    if (!decode_u64_be(raw, sizeof raw, &val)) return EXIT_FAILURE;
    if (printf("Decoded u64: %" PRIu64 "\n", val) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-048-checked-big-endian-64bit-integer-decoding \
  examples/c/c-048-checked-big-endian-64bit-integer-decoding.c
./c-048-checked-big-endian-64bit-integer-decoding
```

Odotettu tuloste:

```text
Decoded u64: 256
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät pätevänä datana raakapuskureissa.
4. Nimeä yksi virheellisesti muotoiltu syöte, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen tehtävä

Laajenna ohjelmaa validoimaan lisätapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-048-checked-big-endian-64bit-integer-decoding.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-048-sanitized \
  examples/c/c-048-checked-big-endian-64bit-integer-decoding.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen turvamekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakavirtapuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei pysty määrittämään niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoja ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
