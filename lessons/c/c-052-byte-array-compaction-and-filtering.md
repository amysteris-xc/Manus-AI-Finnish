# C 052 — Tavujoukon tiivistäminen ja suodatus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–051 sekä välittömästi edeltävät oppitunnit
**Seuraava ohjelma:** [`examples/c/c-052-byte-array-compaction-and-filtering.c`](../../examples/c/c-052-byte-array-compaction-and-filtering.c)
**Deterministinen testi:** [`tests/c-052-byte-array-compaction-and-filtering.sh`](../../tests/c-052-byte-array-compaction-and-filtering.sh)
**Tekijä:** Manus AI

## Tämän oppitunnin jälkeen osaat

Tämän oppitunnin jälkeen osaat tiivistää tavujen viipaleen paikan päällä suodattamalla pois määritellyt erotin­tavut.

## Kertaus

1. Miksi tavu­­pusseissa on käytettävä eksplisiittisiä `size_t` pituuksia sentinel-terminoinnin sijaan?
2. Ilmoita turvallinen läpikäynti­ehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden palautusarvot on tarkistettava ennen niiden tulosten käyttämistä.
4. Muista yksi tarkka ilmaus edellisestä venäjän oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuna raakadatana. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17 -standardin puitteissa. Se välttää natiiveja lataajia, prosessiin injektointia, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t filter_byte(uint8_t *buf, size_t len, uint8_t reject)
{
    if (buf == NULL) return 0;
    size_t w = 0;
    for (size_t r = 0; r < len; ++r) {
        if (buf[r] != reject) buf[w++] = buf[r];
    }
    return w;
}

int main(void)
{
    uint8_t data[] = {'a', ' ', 'b', ' ', 'c'};
    size_t compact = filter_byte(data, sizeof data, ' ');
    if (printf("Filtered length: %zu\n", compact) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-052-byte-array-compaction-and-filtering \
  examples/c/c-052-byte-array-compaction-and-filtering.c
./c-052-byte-array-compaction-and-filtering
```

Odotettu tulostus:

```text
Filtered length: 3
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulos­puskureiden rajat.
2. Etsi jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheelliseen syötteeseen liittyvä ehto, joka aiheuttaa varhaisen virheilmoituksen.
5. Muista yksi ilmaus edellisestä venäjän oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisäreunatapaus tai testisyöte. Pidä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Vahvistaminen

Suorita deterministinen testi:

```sh
./tests/c-052-byte-array-compaction-and-filtering.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-052-sanitized \
  examples/c/c-052-byte-array-compaction-and-filtering.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt selittämään puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toiminnallinen kryptografinen turvaratkaisu.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskat voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin käyvän silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakenne­ehtoja ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraavassa oppitunnissa palataan venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
