# C 036 — Etuliitteen pituuden vertailu tavupaloissa

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–035 ja välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-036-prefix-length-matching-on-byte-slices.c`](../../examples/c/c-036-prefix-length-matching-on-byte-slices.c)
**Deterministinen testi:** [`tests/c-036-prefix-length-matching-on-byte-slices.sh`](../../tests/c-036-prefix-length-matching-on-byte-slices.sh)
**Kirjoittaja:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopussa sinun pitäisi osata laskea kahden eksplisiittisen tavupuskurin yhteisen etuliitteen pituus.

## Muistiharjoitus

1. Miksi tavupuskureiden on käytettävä eksplisiittisiä `size_t` pituuksia sentinellipäättämisen sijaan?
2. Ilmoita turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edelliseltä venäläiseltä oppitunnilta.

## Keskeinen käsite

Esimerkissä käsitellään tavuja laskettuina raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessiin injektoimista, shellcodea ja itseään muokkaavaa koodia.

## Käytännön esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static size_t common_prefix(const uint8_t *a, size_t a_len, const uint8_t *b, size_t b_len)
{
    if (a == NULL || b == NULL) return 0;
    size_t limit = (a_len < b_len) ? a_len : b_len;
    size_t i = 0;
    while (i < limit && a[i] == b[i]) ++i;
    return i;
}

int main(void)
{
    const uint8_t b1[] = {1, 2, 3, 4};
    const uint8_t b2[] = {1, 2, 0, 4};
    size_t p = common_prefix(b1, sizeof b1, b2, sizeof b2);
    if (printf("Common prefix length: %zu\n", p) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-036-prefix-length-matching-on-byte-slices \
  examples/c/c-036-prefix-length-matching-on-byte-slices.c
./c-036-prefix-length-matching-on-byte-slices
```

Odotettu tuloste:

```text
Common prefix length: 2
```

## Ohjattu harjoitus

1. Tunnista tulos- ja tulostuspuskurien rajat.
2. Etsi jokainen paluuarvon ja kapasiteettitarkistuksen kohta.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisina datoina raakapuskureissa.
4. Nimeä yksi virheellinen syöte-tila, joka aiheuttaa varhaisen virheen palautuksen.
5. Muista yksi lause edelliseltä venäläiseltä oppitunnilta.

## Itsenäinen harjoitus

Laajenna ohjelmaa niin, että se validoi lisäisen rajatapauksen tai testisyötteen. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-036-prefix-length-matching-on-byte-slices.sh
```

Käännä ja suorita sanitizerien kanssa, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-036-sanitized \
  examples/c/c-036-prefix-length-matching-on-byte-slices.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen turvaratkaisu.

## Spoileri: vastaukset ja mallivastaus

1. Raakadatana olevat tavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin käyvän silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Vialliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
