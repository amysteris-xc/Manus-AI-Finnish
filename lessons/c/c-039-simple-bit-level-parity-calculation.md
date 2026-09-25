# C 039 — Yksinkertainen bittitason pariteettilaskenta

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–038 ja välittömästi edeltävät oppitunnit
**Esimerkkiohjelma:** [`examples/c/c-039-simple-bit-level-parity-calculation.c`](../../examples/c/c-039-simple-bit-level-parity-calculation.c)
**Deterministinen testi:** [`tests/c-039-simple-bit-level-parity-calculation.sh`](../../tests/c-039-simple-bit-level-parity-calculation.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopussa osaat laskea yksibitisen parillisuuden tavutaulukolle käyttäen bittitason reduktiota.

## Muistiharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia eikä sentinellillä päättämistä?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktion paluuarvot on tarkistettava ennen tulosten käyttämistä.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavut laskettuina raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikki ulkoisten funktioiden paluuarvot ja säilyttää kannettavuuden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t calculate_parity(const uint8_t *data, size_t len)
{
    uint8_t p = 0;
    if (data == NULL) return 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t b = data[i];
        for (int bit = 0; bit < 8; ++bit) p ^= (uint8_t)((b >> bit) & 1);
    }
    return p;
}

int main(void)
{
    const uint8_t payload[] = {0x03, 0x05};
    uint8_t par = calculate_parity(payload, sizeof payload);
    if (printf("Parity bit: %u\n", par) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-039-simple-bit-level-parity-calculation \
  examples/c/c-039-simple-bit-level-parity-calculation.c
./c-039-simple-bit-level-parity-calculation
```

Odotettu tuloste:

```text
Parity bit: 0
```

## Ohjattu harjoitus

1. Tunnista tulo- ja lähtöpuskurien rajat.
2. Paikanna jokainen paluuarvon ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot ovat edelleen kelvollista dataa raakapuskurissa.
4. Nimeä yksi virheellisen syötteen ehto, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsensä harjoitus

Laajenna ohjelmaa validoimaan lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautusarvo.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-039-simple-bit-level-parity-calculation.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-039-sanitized \
  examples/c/c-039-simple-bit-level-parity-calculation.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen turvallisuusmekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` legitiiminä datana; `strlen` ei voi määrittää niiden kokoa.
2. Etenevän silmukan yläraja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
