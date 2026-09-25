# C 030 — Tarkastettu tavutaulukon kääntäminen

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–029 ja välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-030-checked-byte-array-reverse.c`](../../examples/c/c-030-checked-byte-array-reverse.c)
**Deterministinen testi:** [`tests/c-030-checked-byte-array-reverse.sh`](../../tests/c-030-checked-byte-array-reverse.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun pitäisi osata kääntää tavupuskuri paikan päällä käyttämällä rajattuja symmetrisiä indeksejä.

## Kertaus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin sentinel-terminaatiota?
2. Määritä turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen tulosten käyttämistä.
4. Muista yksi tarkka lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakatietoina. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, varmistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void reverse_bytes(uint8_t *buf, size_t len)
{
    if (buf == NULL || len < 2) return;
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = tmp;
    }
}

int main(void)
{
    uint8_t data[] = {1, 2, 3, 4, 5};
    reverse_bytes(data, sizeof data);
    if (printf("Reversed: %u %u %u %u %u\n", data[0], data[1], data[2], data[3], data[4]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-030-checked-byte-array-reverse \
  examples/c/c-030-checked-byte-array-reverse.c
./c-030-checked-byte-array-reverse
```

Odotettu tuloste:

```text
Reversed: 5 4 3 2 1
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen paluuarvon ja kapasiteetin tarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellisen syötteen ehto, joka aiheuttaa varhaisen virhepaluun.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa niin, että se validoi jonkin lisärajatapauksen tai testisyötteen. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-030-checked-byte-array-reverse.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-030-sanitized \
  examples/c/c-030-checked-byte-array-reverse.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurien sopimuksen, virhepaluusarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen turvamekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden tulee palauttaa virhekoodi.

## Seuraava oppitunti

Seuraava oppitunti palaa takaisin venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
