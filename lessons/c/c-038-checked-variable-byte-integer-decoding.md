# C 038 — Tarkistettu muuttujapituisten tavujen kokonaislukujen dekoodaus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–037 ja välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-038-checked-variable-byte-integer-decoding.c`](../../examples/c/c-038-checked-variable-byte-integer-decoding.c)
**Deterministinen testi:** [`tests/c-038-checked-variable-byte-integer-decoding.sh`](../../tests/c-038-checked-variable-byte-integer-decoding.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata dekoodata 7-bittinen jatkotavujono uint32_t-arvoksi ylivuotosuojausten kanssa.

## Muistiharjoitus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin sentinel-päättymistä?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnoksen palautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi oikea lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuina raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessiin injektointia, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int decode_varint(const uint8_t *in, size_t len, uint32_t *val, size_t *consumed)
{
    if (in == NULL || val == NULL || consumed == NULL || len == 0) return 0;
    uint32_t result = 0;
    size_t shift = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t byte = in[i];
        if (shift >= 32) return 0;
        result |= (uint32_t)(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            *val = result;
            *consumed = i + 1;
            return 1;
        }
        shift += 7;
    }
    return 0;
}

int main(void)
{
    const uint8_t encoded[] = {0xAC, 0x02};
    uint32_t val = 0;
    size_t used = 0;
    if (!decode_varint(encoded, sizeof encoded, &val, &used) || val != 300) return EXIT_FAILURE;
    if (printf("Decoded: %" PRIu32 "\nConsumed: %zu\n", val, used) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-038-checked-variable-byte-integer-decoding \
  examples/c/c-038-checked-variable-byte-integer-decoding.c
./c-038-checked-variable-byte-integer-decoding
```

Odotettu tuloste:

```text
Decoded: 300
Consumed: 2
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Löydä jokainen palautusarvo ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisina datoina raakapuskureissa.
4. Nimeä yksi virheellisesti muotoiltu syöte-ehto, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edelliseltä venäjänkieliseltä oppitunnilta.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-038-checked-variable-byte-integer-decoding.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuki on saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-038-sanitized \
  examples/c/c-038-checked-variable-byte-integer-decoding.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurin sopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toimiva kryptografinen turvamekanismi.

## Vastausvinkit ja malli

1. Raakatavutaulukot voivat sisältää `0x00` kelvollisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan yläraja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Väärin muotoillut syötteet rikkovat kapasiteetti- tai rakennerajoitteita ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
