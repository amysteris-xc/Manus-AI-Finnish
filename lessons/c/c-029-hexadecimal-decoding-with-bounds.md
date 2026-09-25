# C 029 — Heksadesimaalinen dekoodaus rajoituksin

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–028 ja välittömästi edeltävät oppitunnit
**Seuralaisohjelma:** [`examples/c/c-029-hexadecimal-decoding-with-bounds.c`](../../examples/c/c-029-hexadecimal-decoding-with-bounds.c)
**Deterministinen testi:** [`tests/c-029-hexadecimal-decoding-with-bounds.sh`](../../tests/c-029-hexadecimal-decoding-with-bounds.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopussa osaat dekoodata ASCII-heksadesimaalimerkkien pareja raakatavuiksi tiukan validoinnin kera.

## Kertaus

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että ne päättyisivät sentineliin?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen kuin käytetään niiden tuottamia arvoja.
4. Muista yksi tarkka ilmaisu edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakadatana. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17 -standardin mukaisesti. Se välttää natiiveja lataajia, prosessiin injektointia, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int hex_val(uint8_t c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static int decode_hex(const uint8_t *hex, size_t hex_len, uint8_t *out, size_t out_cap, size_t *out_len)
{
    if (hex == NULL || out == NULL || out_len == NULL || (hex_len % 2) != 0 || hex_len / 2 > out_cap) return 0;
    for (size_t i = 0; i < hex_len / 2; ++i) {
        int hi = hex_val(hex[i * 2]);
        int lo = hex_val(hex[i * 2 + 1]);
        if (hi < 0 || lo < 0) return 0;
        out[i] = (uint8_t)((hi << 4) | lo);
    }
    *out_len = hex_len / 2;
    return 1;
}

int main(void)
{
    const uint8_t hex[] = {'4', '1', '0', '0', '4', '2'};
    uint8_t bin[3];
    size_t written = 0;
    if (!decode_hex(hex, sizeof hex, bin, sizeof bin, &written)) return EXIT_FAILURE;
    if (printf("Bytes decoded: %zu\nFirst byte: 0x%02" PRIX8 "\n", written, bin[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-029-hexadecimal-decoding-with-bounds \
  examples/c/c-029-hexadecimal-decoding-with-bounds.c
./c-029-hexadecimal-decoding-with-bounds
```

Odotettu tuloste:

```text
Bytes decoded: 3
First byte: 0x41
```

## Ohjattu harjoitus

1. Tunnista syöttö- ja tulostepuskurien rajat.
2. Etsi jokainen paluuarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` -arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheelliseen syötteeseen liittyvä ehto, joka aiheuttaa varhaisen virhepaluun.
5. Muista yksi ilmaisu edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen tehtävä

Laajenna ohjelmaa validoimaan jokin lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-029-hexadecimal-decoding-with-bounds.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos käyttöympäristö tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-029-sanitized \
  examples/c/c-029-hexadecimal-decoding-with-bounds.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepaluuarvot ja miksi tämän oppitunnin koodi ei ole operatiivinen kryptografinen turvamekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` kelvollisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin etenevän silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliselle osiolle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
