# C 032 — Run-length-tavujen purku (count, byte)

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esitiedot:** C 001–031 ja välittömästi edeltävät oppitunnit
**Kumppaniohjelma:** [`examples/c/c-032-counted-run-length-byte-decoding.c`](../../examples/c/c-032-counted-run-length-byte-decoding.c)
**Deterministinen testi:** [`tests/c-032-counted-run-length-byte-decoding.sh`](../../tests/c-032-counted-run-length-byte-decoding.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopuksi osaat laajentaa yksinkertaisia run-length-koodattuja pareja (count, byte) lähtöpuskurille siten, että kapasiteettirajat pidetään voimassa.

## Muistiharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin loppumerkkiä?
2. Määritä turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on varmistettava ennen tulosten käyttämistä.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakatietoina. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivien lataajien, prosessin injektion, shellcoden ja itseään muokkaavan koodin käyttöä.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int rle_decode(const uint8_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len)
{
    if (in == NULL || out == NULL || out_len == NULL || (in_len % 2) != 0) return 0;
    size_t written = 0;
    for (size_t i = 0; i < in_len; i += 2) {
        uint8_t count = in[i];
        uint8_t byte = in[i + 1];
        if (count > out_cap - written) return 0;
        for (uint8_t c = 0; c < count; ++c) out[written++] = byte;
    }
    *out_len = written;
    return 1;
}

int main(void)
{
    const uint8_t encoded[] = {3, 'A', 2, 'B'};
    uint8_t decoded[10];
    size_t total = 0;
    if (!rle_decode(encoded, sizeof encoded, decoded, sizeof decoded, &total)) return EXIT_FAILURE;
    if (printf("Decoded size: %zu\n", total) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-032-counted-run-length-byte-decoding \
  examples/c/c-032-counted-run-length-byte-decoding.c
./c-032-counted-run-length-byte-decoding
```

Odotettu tuloste:

```text
Decoded size: 5
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostuspuskurien rajat.
2. Etsi jokainen paluuarvo ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` -arvot pysyvät kelvollisina datoina raakapuskurissa.
4. Nimeä yksi virheellisen syötteen ehto, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisäreuna­tapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluutila.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-032-counted-run-length-byte-decoding.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-032-sanitized \
  examples/c/c-032-counted-run-length-byte-decoding.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen suojausmekanismi.

## Vastausvinkki: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei pysty määrittämään niiden kokoa.
2. Eteenpäin käyvä silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed width integer types — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Arithmetic operators — cppreference.com"
