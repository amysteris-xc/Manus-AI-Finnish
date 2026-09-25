# C 040 — Hakutaulukon bittien laskenta

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–039 ja välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-040-lookup-table-bit-count-calculation.c`](../../examples/c/c-040-lookup-table-bit-count-calculation.c)
**Deterministinen testi:** [`tests/c-040-lookup-table-bit-count-calculation.sh`](../../tests/c-040-lookup-table-bit-count-calculation.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat laskea aktiiviset bitit (popcount) tavujen virrassa käyttäen esilaskettua 16-alkioista nibble-hakutaulukkoa.

## Muistiharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että ne päättyisivät sentinelin avulla?
2. Määritä turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosten paluuarvot on tarkastettava ennen tulosten käyttöä.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettavana raakadatana. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17:n puitteissa. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t count_set_bits(const uint8_t *data, size_t len)
{
    static const uint8_t nibble_bits[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
    if (data == NULL) return 0;
    size_t total = 0;
    for (size_t i = 0; i < len; ++i) {
        total += nibble_bits[data[i] & 0x0F];
        total += nibble_bits[(data[i] >> 4) & 0x0F];
    }
    return total;
}

int main(void)
{
    const uint8_t data[] = {0xFF, 0x01};
    size_t bits = count_set_bits(data, sizeof data);
    if (printf("Set bits: %zu\n", bits) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-040-lookup-table-bit-count-calculation \
  examples/c/c-040-lookup-table-bit-count-calculation.c
./c-040-lookup-table-bit-count-calculation
```

Odotettu tuloste:

```text
Set bits: 9
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajapinnat.
2. Etsi jokainen paluuarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi viallinen syöte-olosuhde, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-040-lookup-table-bit-count-calculation.sh
```

Käännä ja suorita sanitizerien kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-040-sanitized \
  examples/c/c-040-lookup-table-bit-count-calculation.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen suojausmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin etenevän silmukan yläraja: `index < count`.
3. Tarkastamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliseen linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
