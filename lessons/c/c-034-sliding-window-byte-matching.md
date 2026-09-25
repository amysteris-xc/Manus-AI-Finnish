# C 034 — Liukuikkuna: tavujen vertailu

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esitiedot:** C 001–033 ja välittömästi edeltävät oppitunnit
**Seurantoprogrammi:** [`examples/c/c-034-sliding-window-byte-matching.c`](../../examples/c/c-034-sliding-window-byte-matching.c)
**Deterministinen testi:** [`tests/c-034-sliding-window-byte-matching.sh`](../../tests/c-034-sliding-window-byte-matching.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Oppitunnin lopussa osaat laskea liukuvan tavujen summan kiinteässä ikkunassa ilman, että summa lasketaan uudelleen alusta asti joka kierroksella.

## Kertausharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sentinel-päätteen sijaan?
2. Ilmoita turvallinen läpikäyntiehtoinen `count` alkioille.
3. Selitä, miksi I/O- ja konversiopalautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Esimerkissä käsitellään tavujoukkoja raakatietona. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, varmistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessien injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const uint8_t stream[] = {10, 20, 30, 40, 50};
    const size_t win = 3;
    uint32_t sum = 0;
    for (size_t i = 0; i < win; ++i) sum += stream[i];
    uint32_t first_sum = sum;
    sum = sum - stream[0] + stream[win];
    if (printf("First window: %" PRIu32 "\nSecond window: %" PRIu32 "\n", first_sum, sum) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-034-sliding-window-byte-matching \
  examples/c/c-034-sliding-window-byte-matching.c
./c-034-sliding-window-byte-matching
```

Odotettu tulostus:

```text
First window: 60
Second window: 90
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajapinnat.
2. Etsi jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisina datoina raakapuskureissa.
4. Nimeä yksi virheellisten syötteiden ehto, joka aiheuttaa varhaisen virheilmoituksen.
5. Muista yksi lause edelliseltä venäjänkieliseltä oppitunnilta.

## Itsenäinen tehtävä

Laajenna ohjelmaa niin, että se validoi jonkin lisäreuna­tapauksen tai testisyötteen. Säilytä eksplisiittiset pituudet ja tarkista kaikki palautustilat.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-034-sliding-window-byte-matching.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-034-sanitized \
  examples/c/c-034-sliding-window-byte-matching.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toiminnallinen kryptografinen turvamekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaisiin virheisiin.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoitteita ja niiden pitää palauttaa virhekoodi.

## Seuraava oppitunti

Seuraavaksi vuorossa on jälleen venäjänkielinen oppitunti.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteäleveysiset kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
