# C 042 — Ei-kryptografinen CRC-8-laskenta

**Arvioitu opiskelu-aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–041 ja välittömästi edeltävät oppitunnit
**Companion program:** [`examples/c/c-042-non-cryptographic-crc8-computation.c`](../../examples/c/c-042-non-cryptographic-crc8-computation.c)
**Deterministic test:** [`tests/c-042-non-cryptographic-crc8-computation.sh`](../../tests/c-042-non-cryptographic-crc8-computation.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi osaat laskea bitti kerrallaan tapahtuvan CRC-8:n polynomilla 0x07 (ATM) ja varmistaa determinismin.

## Muistin palautusharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sentinel-terminoinnin sijaan?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja konversiotoimintojen palautusarvot on tarkistettava ennen tulosten käyttämistä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavut laskettuina raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessin injektointia, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t crc8(const uint8_t *data, size_t len)
{
    uint8_t crc = 0x00;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x80) crc = (uint8_t)((crc << 1) ^ 0x07);
            else crc = (uint8_t)(crc << 1);
        }
    }
    return crc;
}

int main(void)
{
    const uint8_t test[] = {'1', '2', '3', '4', '5'};
    uint8_t val = crc8(test, sizeof test);
    if (printf("CRC-8: 0x%02" PRIX8 "\n", val) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-042-non-cryptographic-crc8-computation \
  examples/c/c-042-non-cryptographic-crc8-computation.c
./c-042-non-cryptographic-crc8-computation
```

Odotettu tulostus:

```text
CRC-8: 0xCB
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen palautusarvon ja kapasiteetin tarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisena datana raakapuskurissa.
4. Nimeä yksi viallinen syöte-tilanne, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisätty reunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-042-non-cryptographic-crc8-computation.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-042-sanitized \
  examples/c/c-042-non-cryptographic-crc8-computation.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole toiminnallinen kryptografinen suojausmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` oikeana datana; `strlen` ei pysty määrittämään niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Vialliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
