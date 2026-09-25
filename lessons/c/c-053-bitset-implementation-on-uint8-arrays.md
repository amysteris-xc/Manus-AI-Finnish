# C 053 — Bitsetin toteutus uint8_t-taulukoilla

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–052 ja välittömästi edeltävät oppitunnit
**Companion program:** [`examples/c/c-053-bitset-implementation-on-uint8-arrays.c`](../../examples/c/c-053-bitset-implementation-on-uint8-arrays.c)
**Deterministic test:** [`tests/c-053-bitset-implementation-on-uint8-arrays.sh`](../../tests/c-053-bitset-implementation-on-uint8-arrays.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Oppitunnin lopussa sinun pitäisi pystyä toteuttamaan rajatut testaus- ja asetusoperaatiot mielivaltaisen pituiselle bitvektorille (bitset).

## Kertaus ja harjoitus

1. Miksi tavutaulukoissa (byte buffers) on käytettävä eksplisiittisiä `size_t` pituuksia sentinel-terminoinnin sijaan?
2. Muotoile turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuina raakatietoina. Se pakottaa eksplisiittiset `size_t` kapasiteetit, varmistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17 -standardin puitteissa. Se välttää natiivilataimia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void bitset_set(uint8_t *bits, size_t cap_bytes, size_t bit)
{
    if (bits == NULL || bit / 8 >= cap_bytes) return;
    bits[bit / 8] |= (uint8_t)(1 << (bit % 8));
}

static int bitset_test(const uint8_t *bits, size_t cap_bytes, size_t bit)
{
    if (bits == NULL || bit / 8 >= cap_bytes) return 0;
    return (bits[bit / 8] & (1 << (bit % 8))) != 0;
}

int main(void)
{
    uint8_t set[2] = {0};
    bitset_set(set, sizeof set, 10);
    if (printf("Bit 10: %d\nBit 11: %d\n", bitset_test(set, sizeof set, 10), bitset_test(set, sizeof set, 11)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-053-bitset-implementation-on-uint8-arrays \
  examples/c/c-053-bitset-implementation-on-uint8-arrays.c
./c-053-bitset-implementation-on-uint8-arrays
```

Odotettu tulostus:

```text
Bit 10: 1
Bit 11: 0
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostaulukoiden rajat.
2. Etsi jokainen paluuarvon ja kapasiteetin tarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisena datana raakatauluissa.
4. Nimeä yksi virheelliseen syötteeseen johtava ehtorakenne, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edeltävästä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan yksi lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-053-bitset-implementation-on-uint8-arrays.sh
```

Käännä ja suorita sanitisaattoreilla siellä, missä niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-053-sanitized \
  examples/c/c-053-bitset-implementation-on-uint8-arrays.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen turvaratkaisu.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavutaulukoissa voi esiintyä `0x00` kelvollisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoja ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle kurssille.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
