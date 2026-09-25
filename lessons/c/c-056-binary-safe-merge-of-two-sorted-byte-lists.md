# C 056 — Kahden lajitellun tavulistan binääriturvallinen yhdistäminen

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–055 ja välittömästi edeltävät oppitunnit
**Seurantaohjelma:** [`examples/c/c-056-binary-safe-merge-of-two-sorted-byte-lists.c`](../../examples/c/c-056-binary-safe-merge-of-two-sorted-byte-lists.c)
**Deterministinen testi:** [`tests/c-056-binary-safe-merge-of-two-sorted-byte-lists.sh`](../../tests/c-056-binary-safe-merge-of-two-sorted-byte-lists.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat yhdistää kaksi lajiteltua tavutaulukkoa varattuun puskurimuistiin ja varmistaa puskurin kapasiteetin.

## Muistiharjoitus

1. Miksi tavupuskurit on määriteltävä eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin päätemerkin avulla tapahtuvaa terminaatiota?
2. Ilmoita turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja konversiopalautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka fraasi edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavuja käsitellään laskettuina raakadatana. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int merge_sorted(const uint8_t *a, size_t a_len, const uint8_t *b, size_t b_len, uint8_t *out, size_t out_cap)
{
    if ((a == NULL && a_len != 0) || (b == NULL && b_len != 0) || out == NULL) return 0;
    if (a_len > out_cap || b_len > out_cap - a_len) return 0;
    size_t i = 0, j = 0, k = 0;
    while (i < a_len && j < b_len) {
        if (a[i] <= b[j]) out[k++] = a[i++];
        else out[k++] = b[j++];
    }
    while (i < a_len) out[k++] = a[i++];
    while (j < b_len) out[k++] = b[j++];
    return 1;
}

int main(void)
{
    const uint8_t l1[] = {1, 4, 7};
    const uint8_t l2[] = {2, 5, 8};
    uint8_t res[6];
    if (!merge_sorted(l1, sizeof l1, l2, sizeof l2, res, sizeof res)) return EXIT_FAILURE;
    if (printf("Merged 6 items: %u %u ... %u\n", res[0], res[1], res[5]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-056-binary-safe-merge-of-two-sorted-byte-lists \
  examples/c/c-056-binary-safe-merge-of-two-sorted-byte-lists.c
./c-056-binary-safe-merge-of-two-sorted-byte-lists
```

Odotettu tulostus:

```text
Merged 6 items: 1 2 ... 8
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskurissa.
4. Nimeä yksi virheellinen syöte-ehto, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi fraasi edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan ylimääräinen reunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-056-binary-safe-merge-of-two-sorted-byte-lists.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-056-sanitized \
  examples/c/c-056-binary-safe-merge-of-two-sorted-byte-lists.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen turvallisuusmekanismi.

## Vastausvinkki: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` kelvollisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan yläraja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden tulee palauttaa virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen sarjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
