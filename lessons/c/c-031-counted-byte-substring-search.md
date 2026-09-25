# C 031 — Laskettujen tavujen alimerkkijonon haku

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–030 ja välittömästi edeltävät oppitunnit
**Lisäohjelma:** [`examples/c/c-031-counted-byte-substring-search.c`](../../examples/c/c-031-counted-byte-substring-search.c)
**Deterministinen testi:** [`tests/c-031-counted-byte-substring-search.sh`](../../tests/c-031-counted-byte-substring-search.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat etsiä tiettyä tavujonoa toisen tavupuskurin sisältä käyttäen eksplisiittisiä pituuksia.

## Muistiharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin loppumerkkiä?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja konversiopalautusarvot on tarkistettava ennen tulosten käyttämistä.
4. Muista yksi tarkka lause edeltävästä venäläisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään lasketuiksi raakadataksi. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikki ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiiveja lataajia, prosessien injektointia, shellcodea ja itseään muuttavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_bytes(const uint8_t *haystack, size_t h_len, const uint8_t *needle, size_t n_len, size_t *pos)
{
    if (haystack == NULL || needle == NULL || pos == NULL || n_len == 0 || n_len > h_len) return 0;
    for (size_t i = 0; i <= h_len - n_len; ++i) {
        int match = 1;
        for (size_t j = 0; j < n_len; ++j) {
            if (haystack[i + j] != needle[j]) { match = 0; break; }
        }
        if (match) { *pos = i; return 1; }
    }
    return 0;
}

int main(void)
{
    const uint8_t hay[] = {'a', 'b', 'c', 'd', 'e'};
    const uint8_t ndl[] = {'c', 'd'};
    size_t match_idx = 0;
    if (!find_bytes(hay, sizeof hay, ndl, sizeof ndl, &match_idx)) return EXIT_FAILURE;
    if (printf("Match at index: %zu\n", match_idx) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-031-counted-byte-substring-search \
  examples/c/c-031-counted-byte-substring-search.c
./c-031-counted-byte-substring-search
```

Odotettu tulostus:

```text
Match at index: 2
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskureiden rajat.
2. Etsi jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskurissa.
4. Nimeä yksi virheellinen syöteolosuhde, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edeltävästä venäläisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan yksi lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-031-counted-byte-substring-search.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-031-sanitized \
  examples/c/c-031-counted-byte-substring-search.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole operatiivinen kryptografinen suojausmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttämiseen tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakenne-ehdot ja niiden tulee palauttaa virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle oppimispolulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
