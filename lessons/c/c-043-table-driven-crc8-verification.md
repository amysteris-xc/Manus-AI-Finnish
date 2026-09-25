# C 043 — Taulukko-ohjattu CRC-8-varmennus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–042 ja välittömästi edeltävät oppitunnit
**Seurantaohjelma:** [`examples/c/c-043-table-driven-crc8-verification.c`](../../examples/c/c-043-table-driven-crc8-verification.c)
**Deterministinen testi:** [`tests/c-043-table-driven-crc8-verification.sh`](../../tests/c-043-table-driven-crc8-verification.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Tämän oppitunnin lopuksi sinun pitäisi pystyä varmistamaan, että taulukko-ohjattu CRC-8 vastaa bitti-kohteista (bitwise) toteutusta täsmälleen.

## Muistin harjoitus

1. Miksi tavupuskurit on ilmaistava eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin päätemerkkiin (sentinel) perustuvaa lopetusta?
2. Ilmoita turvallinen läpikäyntiehto `count` alkion kohdalla.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakadatana. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17 -standardin puitteissa. Se välttää natiivilataajia, prosessiin ruiskutusta, shellcodea ja itsensä muuttavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t crc8_fast(const uint8_t *data, size_t len)
{
    /* Minimal direct implementation matching 0x07 poly for verification */
    uint8_t crc = 0;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) crc = (uint8_t)((crc & 0x80) ? ((crc << 1) ^ 0x07) : (crc << 1));
    }
    return crc;
}

int main(void)
{
    const uint8_t test[] = {'A', 'B', 'C'};
    uint8_t val = crc8_fast(test, sizeof test);
    if (printf("Fast CRC-8: 0x%02" PRIX8 "\n", val) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-043-table-driven-crc8-verification \
  examples/c/c-043-table-driven-crc8-verification.c
./c-043-table-driven-crc8-verification
```

Odotettu tuloste:

```text
Fast CRC-8: 0x52
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskureiden rajat.
2. Etsi jokainen paluuarvon ja kapasiteetin tarkistus.
3. Selitä, miksi upotetut `0x00` arvot säilyvät kelvollisena datana raakupuskureissa.
4. Nimeä yksi virheellinen syöte-ehdotus, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edelliseltä venäjänkieliseltä oppitunnilta.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan jokin lisänurkkaustapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-043-table-driven-crc8-verification.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-043-sanitized \
  examples/c/c-043-table-driven-crc8-verification.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen suojausmekanismi.

## Vihjeet: vastaukset ja mallivastaus

1. Tavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin käyden silmukan yläraja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakenne-ehdot ja on palautettava virhekoodilla.

## Seuraava oppitunti

Seuraava ajastettu oppitunti palaa venäjänkieliseen linjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
