# C 047 — Tarkistettu big-endian 64-bittisen uint64_t:n koodaus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–046 ja välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-047-checked-big-endian-64bit-integer-encoding.c`](../../examples/c/c-047-checked-big-endian-64bit-integer-encoding.c)
**Deterministinen testi:** [`tests/c-047-checked-big-endian-64bit-integer-encoding.sh`](../../tests/c-047-checked-big-endian-64bit-integer-encoding.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun tulisi pystyä serialisoimaan uint64_t-arvo 8:ksi big-endian-tavuksi siirrettävästi ilman oletuksia isäntäalustan tavujärjestyksestä.

## Kertaus

1. Miksi tavupuskurit (byte buffers) on esitettävä eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin sentinel-päättelyä?
2. Ilmaise turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden palautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakatietoina. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int encode_u64_be(uint64_t v, uint8_t *out, size_t cap)
{
    if (out == NULL || cap < 8) return 0;
    for (int i = 7; i >= 0; --i) {
        out[i] = (uint8_t)(v & 0xFF);
        v >>= 8;
    }
    return 1;
}

int main(void)
{
    uint8_t buf[8];
    if (!encode_u64_be(0x0102030405060708ULL, buf, sizeof buf)) return EXIT_FAILURE;
    if (printf("Encoded byte 0: %u\nEncoded byte 7: %u\n", buf[0], buf[7]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-047-checked-big-endian-64bit-integer-encoding \
  examples/c/c-047-checked-big-endian-64bit-integer-encoding.c
./c-047-checked-big-endian-64bit-integer-encoding
```

Odotettu tuloste:

```text
Encoded byte 0: 1
Encoded byte 7: 8
```

## Ohjattu harjoitus

1. Tunnista syöte- ja lähtöpuskureiden rajat.
2. Etsi jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisina datoina raakapuskureissa.
4. Nimeä yksi virheelliseen syötteeseen liittyvä tapaus, joka aiheuttaa varhaisen virheen palautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa tarkistamaan jokin ylimääräinen reunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista kaikki palautustilat.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-047-checked-big-endian-64bit-integer-encoding.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-047-sanitized \
  examples/c/c-047-checked-big-endian-64bit-integer-encoding.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen turvaratkaisu.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan yläraja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakenteellisia rajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliselle radalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
