# C 044 — Laskettu tavupuskurien duplikaattien poisto

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–043 ja välittömästi edeltävät oppitunnit
**Companion program:** [`examples/c/c-044-counted-byte-buffer-deduplication.c`](../../examples/c/c-044-counted-byte-buffer-deduplication.c)
**Deterministinen testi:** [`tests/c-044-counted-byte-buffer-deduplication.sh`](../../tests/c-044-counted-byte-buffer-deduplication.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Oppitunnin lopussa osaat poistaa vierekkäiset päällekkäiset tavut paikan päällä ja päivittää kelvollisen pituuden.

## Muistiharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin lopetusmerkkiä?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosten palautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka fraasi edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakatietoina. Se edellyttää nimenomaisia `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää kannettavuuden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessin injisointia, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t dedup_adjacent(uint8_t *data, size_t len)
{
    if (data == NULL || len == 0) return 0;
    size_t w = 1;
    for (size_t r = 1; r < len; ++r) {
        if (data[r] != data[w - 1]) data[w++] = data[r];
    }
    return w;
}

int main(void)
{
    uint8_t stream[] = {1, 1, 2, 3, 3, 3, 4};
    size_t new_len = dedup_adjacent(stream, sizeof stream);
    if (printf("Deduped length: %zu\n", new_len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-044-counted-byte-buffer-deduplication \
  examples/c/c-044-counted-byte-buffer-deduplication.c
./c-044-counted-byte-buffer-deduplication
```

Odotettu tuloste:

```text
Deduped length: 4
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Paikanna jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellisen syötteen tila, joka aiheuttaa varhaisen virheilmoituksen.
5. Muista yksi fraasi edelliseltä venäjänkieliseltä oppitunnilta.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan jokin lisäreunatapaus tai testi-syöte. Säilytä eksplisiittiset pituudet ja tarkista jokaisen funktion palautustila.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-044-counted-byte-buffer-deduplication.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-044-sanitized \
  examples/c/c-044-counted-byte-buffer-deduplication.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurin sopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole operatiivinen kryptografinen suojausmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Etenevän silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava ajastettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
