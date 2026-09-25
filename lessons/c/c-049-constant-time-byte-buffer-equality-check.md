# C 049 — Vakioaikainen tavupuskurin yhtäsuuruuden tarkistus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–048 ja välittömästi sitä edeltävät oppitunnit
**Esimerkkiohjelma:** [`examples/c/c-049-constant-time-byte-buffer-equality-check.c`](../../examples/c/c-049-constant-time-byte-buffer-equality-check.c)
**Deterministinen testi:** [`tests/c-049-constant-time-byte-buffer-equality-check.sh`](../../tests/c-049-constant-time-byte-buffer-equality-check.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat verrata kahta tavupuskuria ilman ennenaikaisia poistumisia, jotta varmennuslogiikka ei aiheuta ajoitushaittakanaaleja.

## Kertauskysymykset

1. Miksi tavupuskureissa on käytettävä eksplisiittisiä `size_t` pituuksia sentinellipäättelyn sijaan?
2. Määritä turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden palautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakadatoina. Siinä vaaditaan eksplisiittiset `size_t` kapasiteetit, varmistetaan kaikki ulkoisten funktioiden palautusarvot ja säilytetään kannettavuus ISO C17:n mukaisesti. Se välttää natiivilataajat, prosessiin injektoimisen, shellcode:n ja itseään muokkaavan koodin.

## Käytännön esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational only: demonstrates constant-time loop pattern */
static int ct_equal(const uint8_t *a, const uint8_t *b, size_t len)
{
    if (a == NULL || b == NULL) return 0;
    uint8_t diff = 0;
    for (size_t i = 0; i < len; ++i) diff |= (uint8_t)(a[i] ^ b[i]);
    return diff == 0;
}

int main(void)
{
    const uint8_t k1[4] = {1, 2, 3, 4};
    const uint8_t k2[4] = {1, 2, 3, 4};
    const uint8_t k3[4] = {1, 2, 9, 4};
    if (printf("k1 == k2: %d\nk1 == k3: %d\n", ct_equal(k1, k2, 4), ct_equal(k1, k3, 4)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-049-constant-time-byte-buffer-equality-check \
  examples/c/c-049-constant-time-byte-buffer-equality-check.c
./c-049-constant-time-byte-buffer-equality-check
```

Odotettu tulos:

```text
k1 == k2: 1
k1 == k3: 0
```

## Ohjattu harjoitus

1. Tunnista syöttö- ja tulostuspuskurien rajat.
2. Etsi jokainen palautusarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisina datoina raakapuskurissa.
4. Nimeä yksi viallinen syöte, joka aiheuttaa ennenaikaisen virhepaluun.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Vahvistus

Aja deterministinen testi:

```sh
./tests/c-049-constant-time-byte-buffer-equality-check.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos sellaisia tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-049-sanitized \
  examples/c/c-049-constant-time-byte-buffer-equality-check.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole operatiivinen kryptografinen turvallisuusmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` kelpona datana; `strlen` ei pysty määrittämään niiden kokoa.
2. Eteenpäin-silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Väärin muodostetut syötteet rikkovat kapasiteetti- tai rakennerajoja ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava suunniteltu oppitunti palaa venäjänkieliseen opetukseen.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
