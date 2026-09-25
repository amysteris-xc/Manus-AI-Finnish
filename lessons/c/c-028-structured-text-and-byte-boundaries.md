# C 028 — Structured Text and Byte Boundaries

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–027 ja edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-028-structured-text-and-byte-boundaries.c`](../../examples/c/c-028-structured-text-and-byte-boundaries.c)
**Deterministinen testi:** [`tests/c-028-structured-text-and-byte-boundaries.sh`](../../tests/c-028-structured-text-and-byte-boundaries.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun tulisi pystyä jäsentämään yksinkertainen avain–arvo -tietue rajatusta puskurista ilman, että luet puskurin loppua pidemmälle tai oletat nollaterminaattoria.

## Kertaus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin päätearvoon perustuvaa lopetusta?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden palautusarvot on tarkistettava ennen niiden tuottaman datan käyttöä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Peruskäsite

Esimerkissä tavut käsitellään laskettuina raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteteetteja, varmistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivien lataajien, prosessiin injektion, shellcoden ja itseään muokkaavan koodin käyttöä.

## Käytännön esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_colon(const uint8_t *buf, size_t len, size_t *colon_idx)
{
    if (buf == NULL || colon_idx == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        if (buf[i] == (uint8_t)':') {
            *colon_idx = i;
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    const uint8_t record[] = {'k', 'e', 'y', ':', 'v', 'a', 'l'};
    size_t colon_pos = 0;
    if (!find_colon(record, sizeof record, &colon_pos)) return EXIT_FAILURE;
    if (printf("Key length: %zu\nValue length: %zu\n", colon_pos, sizeof record - colon_pos - 1) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-028-structured-text-and-byte-boundaries \
  examples/c/c-028-structured-text-and-byte-boundaries.c
./c-028-structured-text-and-byte-boundaries
```

Odotettu tuloste:

```text
Key length: 3
Value length: 3
```

## Ohjattu harjoitus

1. Tunnista sisään- ja ulostulopuskurin rajat.
2. Etsi jokainen palautusarvon ja kapasiteettitarkistuksen kohta.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellisen syötteen ehto, joka aiheuttaa ennenaikaisen virheen palauttamisen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan yksi lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-028-structured-text-and-byte-boundaries.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos järjestelmä tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-028-sanitized \
  examples/c/c-028-structured-text-and-byte-boundaries.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toiminnallinen kryptografinen turvallisuusmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatuvapuskurit voivat sisältää `0x00` kelvollisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaisiin virheisiin.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoja ja niiden tulee palauttaa virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteäleveysiset kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
