# C 054 — Yksinkertainen Base64-koodaus tavupuskurille

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–053 ja sitä välittömästi edeltävät oppitunnit
**Kaveriohjelma:** [`examples/c/c-054-simple-base64-encoding-for-byte-buffers.c`](../../examples/c/c-054-simple-base64-encoding-for-byte-buffers.c)
**Deterministinen testi:** [`tests/c-054-simple-base64-encoding-for-byte-buffers.sh`](../../tests/c-054-simple-base64-encoding-for-byte-buffers.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä loppuun mennessä

Tämän oppitunnin lopuksi osaat koodata tavuista muodostuvat kolmikot 4-merkkisiksi ASCII Base64 -lohkoiksi käyttäen eksplisiittistä pituuslaskentaa.

## Muistiharjoitus

1. Miksi tavupuskurit tulee käyttää eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin sentinel-terminointia?
2. Määritä turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen tulosten käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään määrätyn pituisina raakatietoina. Se vaatii eksplisiittisiä `size_t` kapasiteetteja, varmentaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int b64_encode3(const uint8_t in[3], char out[4])
{
    if (in == NULL || out == NULL) return 0;
    out[0] = b64_table[in[0] >> 2];
    out[1] = b64_table[((in[0] & 3) << 4) | (in[1] >> 4)];
    out[2] = b64_table[((in[1] & 0x0F) << 2) | (in[2] >> 6)];
    out[3] = b64_table[in[2] & 0x3F];
    return 1;
}

int main(void)
{
    const uint8_t raw[3] = {'M', 'a', 'n'};
    char enc[5] = {0};
    if (!b64_encode3(raw, enc)) return EXIT_FAILURE;
    if (printf("Base64: %s\n", enc) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-054-simple-base64-encoding-for-byte-buffers \
  examples/c/c-054-simple-base64-encoding-for-byte-buffers.c
./c-054-simple-base64-encoding-for-byte-buffers
```

Odotettu tuloste:

```text
Base64: TWFu
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajapinnat.
2. Etsi jokainen paluuarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` -arvot säilyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellinen syöteolosuhde, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa siten, että se validoi jonkin lisäreunatapauksen tai testisyötteen. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-054-simple-base64-encoding-for-byte-buffers.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-054-sanitized \
  examples/c/c-054-simple-base64-encoding-for-byte-buffers.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virheilmoitusten paluuarvot ja miksi tämän oppitunnin koodi ei ole toiminnallinen kryptografinen turvaratkaisu.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavuja sisältävissä puskurissa voi esiintyä `0x00` laillisena datana; `strlen` ei pysty määrittämään niiden kokoa.
2. Eteenpäin kulkevan silmukan yläraja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoitteita ja niiden tulee palauttaa virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed width integer types — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Arithmetic operators — cppreference.com"
