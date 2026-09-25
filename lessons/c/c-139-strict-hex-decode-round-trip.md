# C 139 — Tiukka heksadesimaali-dekoodauksen pyöristys

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–138
**Seuraava ohjelma:** [`examples/c/c-139-strict-hex-decode-round-trip.c`](../../examples/c/c-139-strict-hex-decode-round-trip.c)
**Deterministinen testi:** [`tests/c-139-strict-hex-decode-round-trip.sh`](../../tests/c-139-strict-hex-decode-round-trip.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat dekoodata validoidun parillispituisen heksadesimaalimerkkijonon lasketuiksi tavuiksi.

## Palautusharjoitus

1. Miksi raakabatajuferin täytyy kantaa eksplisiittistä `size_t` pituutta?
2. Mikä silmukan ehtolauseke pitää eteenpäin liikkuvan indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:sta palautuvat arvot tulee tarkistaa ennen kuin niiden tuottamiin tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Hylkää parittomat pituudet ja ei-heksadesimaalimerkit ennen kuin kirjoitat ulostulon. Dekoodattu tavujen lukumäärä on eksplisiittinen, ei pääteltävissä sentinellistä.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavut lasketulla datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautusarvon johon esimerkki luottaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivia ladattavaa koodia, mielivaltaista kokoelman suoritusta, prosessin injektiota, itse-muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_hex_prefix(const uint8_t *data, size_t count, size_t maximum)
{
    size_t shown = count < maximum ? count : maximum;
    if (data == NULL && count != 0) return 0;
    for (size_t i = 0; i < shown; ++i)
        if (printf("%02" PRIX8, data[i]) < 0) return 0;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(139), UINT8_C(255)};
    if (printf("C 139 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-139-strict-hex-decode-round-trip \
  examples/c/c-139-strict-hex-decode-round-trip.c
./c-139-strict-hex-decode-round-trip
```

Odotettu tuloste:

```text
C 139 hex: 008BFF
```

## Ohjattu harjoittelu

1. Määritä tavujen jänne tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Kerro, milloin ulostuloarvo tai -sijainti saattaa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen validi tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavujen laskut, säilytä alkuperäinen ulostulo hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen kuin muutat allokaation kokoa. Älä muuta tätä opetus-esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suorittamiseksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-139-strict-hex-decode-round-trip.sh
```

Käännä ja suorita sanitizerien kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-139-sanitized \
  examples/c/c-139-strict-hex-decode-round-trip.c
./c-139-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavujen käsittelyn todellisesta kryptografisesta suojasta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin käydessä tarkistetaan `index < count` ennen lukemista `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
