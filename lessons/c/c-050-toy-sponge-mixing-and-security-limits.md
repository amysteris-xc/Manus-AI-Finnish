# C 050 — Lelusienen sekoittuminen ja turvallisuusrajat

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–049 ja välittömästi edeltävät oppitunnit
**Seuralaisohjelma:** [`examples/c/c-050-toy-sponge-mixing-and-security-limits.c`](../../examples/c/c-050-toy-sponge-mixing-and-security-limits.c)
**Deterministinen testi:** [`tests/c-050-toy-sponge-mixing-and-security-limits.sh`](../../tests/c-050-toy-sponge-mixing-and-security-limits.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi osaat havainnollistaa opetuksellisen lelusienen imeytymistilan ja selittää, miksi se ei ole turvallinen oikeassa kryptografiassa.

## Kertauskysymykset

1. Miksi tavupuskurien on käytettävä nimenomaisia `size_t` pituuksia sen sijaan, että käytettäisiin erikoismerkillä päättämistä (sentinel-terminaatiota)?
2. Ilmoita turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden palautusarvot on varmistettava ennen tulosten käyttämistä.
4. Muista yksi tarkka lause edelliseltä venäläiseltä oppitunnilta.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuina raakatietoina. Se edellyttää nimenomaisia `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessiin injektointia, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Deliberately insecure toy sponge pattern for education only */
static uint32_t toy_sponge(const uint8_t *data, size_t len)
{
    uint32_t state = 0xA5A5A5A5;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        state ^= data[i];
        state = (state << 5) | (state >> 27);
        state *= 0x5BD1E995;
    }
    return state;
}

int main(void)
{
    const uint8_t input[] = {'A', 'B', 'C'};
    uint32_t h = toy_sponge(input, sizeof input);
    if (printf("Toy sponge: 0x%08" PRIX32 "\n", h) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-050-toy-sponge-mixing-and-security-limits \
  examples/c/c-050-toy-sponge-mixing-and-security-limits.c
./c-050-toy-sponge-mixing-and-security-limits
```

Odotettu tuloste:

```text
Toy sponge: 0x46A71FF8
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen palautusarvo ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskurissa.
4. Nimeä yksi väärinmuotoisen syötteen ehto, joka aiheuttaa varhaisen virheilmoituksen.
5. Muista yksi lause edelliseltä venäläiseltä oppitunnilta.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan jokin lisärajatapaus tai testisyöte. Säilytä nimenomaiset pituudet ja tarkista jokainen palautustila.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-050-toy-sponge-mixing-and-security-limits.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-050-sanitized \
  examples/c/c-050-toy-sponge-mixing-and-security-limits.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen turvaratkaisu.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei määritä niiden kokoa.
2. Eteenpäin etenevän silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
