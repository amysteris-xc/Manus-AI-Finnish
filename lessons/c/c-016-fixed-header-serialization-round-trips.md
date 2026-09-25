# C 016 — Kiinteän otsikon serialisointi ja pyöräytystestit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–007 sekä tätä laajennettua sarjaa edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-016-fixed-header-serialization-round-trips.c`](../../examples/c/c-016-fixed-header-serialization-round-trips.c)
**Deterministinen testi:** [`tests/c-016-fixed-header-serialization-round-trips.sh`](../../tests/c-016-fixed-header-serialization-round-trips.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat koodata pienen, kiinteäkokoisen otsikon (8 tavua) big-endian-muotoon ja purkaa sen takaisin kattavalla pyöräytystestillä.

## Muistiharjoitus

Vastaa ennen kuin avaat aiemmat oppitunnit uudelleen.

1. Miksi tavupuskurin tarvitsee eksplisiittinen `size_t` pituus?
2. Määritä turvallinen etenevän silmukan ehto `count` kelvolliselle alkiomäärälle.
3. Mitä siirtosuuruudelta (shift count) täytyy edellyttää, jotta se on pätevä 32-bittiselle sanalle?
4. Venäjä 007:stä, käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Kiinteässä formaatissa kenttäleveydet, tavujärjestys ja offsetit on dokumentoitava. Koodi kirjoittaa kunkin `uint16_t`- ja `uint32_t`‑kentän määrättyihin tavun indekseihin ja purkaa ne samasta paikasta ilman riippuvuutta rakenteen täytteestä tai isäntäjärjestelmän endiannista.

Ohjelma käyttää siirrettävää ISO C17 -koodia, tarkistaa paluuarvot jokaisella ulkoisella rajapinnalla ja pitää loogiset arvot erillään C-merkkijonoista. Se käyttää `uint8_t`‑ ja `uint32_t`‑rakenteita vain siellä, missä niiden bitintarkka merkitys kuuluu oppitunnin sopimukseen. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin tai muokkaa omia ohjeitaan.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void put_be16(uint8_t out[2], uint16_t value) { out[0] = (uint8_t)(value >> 8); out[1] = (uint8_t)value; }
static uint16_t get_be16(const uint8_t in[2]) { return (uint16_t)(((uint16_t)in[0] << 8) | in[1]); }
static void put_be32(uint8_t out[4], uint32_t value) { out[0] = (uint8_t)(value >> 24); out[1] = (uint8_t)(value >> 16); out[2] = (uint8_t)(value >> 8); out[3] = (uint8_t)value; }
static uint32_t get_be32(const uint8_t in[4]) { return ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) | ((uint32_t)in[2] << 8) | in[3]; }

int main(void)
{
    uint8_t header[8];
    const uint16_t version = UINT16_C(3), flags = UINT16_C(5);
    const uint32_t length = UINT32_C(291);
    put_be16(header, version); put_be16(header + 2, flags); put_be32(header + 4, length);
    if (get_be16(header) != version || get_be16(header + 2) != flags || get_be32(header + 4) != length) return EXIT_FAILURE;
    if (printf("Header bytes: %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 "\nVersion: %" PRIu16 " Flags: %" PRIu16 " Length: %" PRIu32 "\n", header[0], header[1], header[2], header[3], header[4], header[5], header[6], header[7], version, flags, length) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita mukana oleva ohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-016-fixed-header-serialization-round-trips \
  examples/c/c-016-fixed-header-serialization-round-trips.c
./c-016-fixed-header-serialization-round-trips
```

Odotettu tulostus:

```text
Header bytes: 00 03 00 05 00 00 01 23
Version: 3 Flags: 5 Length: 291
```

## Ohjattu harjoitus

1. Tunnista esimerkin eksplisiittinen syötteen raja.
2. Nimeä tila- tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei käytä sentineliä kuten `0x00` yleisenä taulun rajana.
4. Mainitse yksi virheellinen syöte, jonka mukana oleva ohjelma hylkää tai käsittelee turvallisesti.
5. Anna yhdestä välittömästi edellisestä venäjän oppitunnosta yksi tarkka kohdelause.

## Itsenäinen harjoitus

Lisää yksitavun tyyppikenttä tavuun 0 ja siirrä kaikki muut kentät dokumentoituihin uusiin offseteihin. Päivitä sekä kooderi, dekooderi että testivektori yhdessä.

Pidä kaikki lukumäärät muodossa `size_t`, validoi kaikki muunnokset ennen niiden supistamista ja säilytä alkuperäinen objekti, jos allokointi tai jäsentäminen epäonnistuu. Tämä on tietojen käsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei saa koskaan esittää sopivana turvallisuuden, salauksen, todennuksen tai todellisten tietojen suojaamiseen.

## Vahvistus

Suorita repositorion testi:

```sh
./tests/c-016-fixed-header-serialization-round-trips.sh
```

Sitten, missä tuettu, käännä AddressSanitizerilla ja UndefinedBehaviorSanitizerilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-016-sanitized \
  examples/c/c-016-fixed-header-serialization-round-trips.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää syötteen rajan, relevantin kokonaislukuleveyden, paluuarvojen tarkistukset, ylivuoto‑ ja siirtovarmistukset, deterministisen testivektorin sekä miksi esimerkki ei ole kryptografinen rakenne.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskuri käyttää eksplisiittistä pituutta, koska nollaarvoiset tavut voivat olla normaalia dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtoarvon tulee olla ei-negatiivinen ja pienempi kuin vasemman operaandin promotoidun tyypin bittileveys.
4. Я не читаю в библиотеке. tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkasti määritellyt rajat ja deterministinen testi tekevät sen käyttäytymisestä tarkastettavan; ne eivät tee siitä kryptografista suojausmekanismia.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti on **Russian 017 — integroidut rutiinit menneisyyden, nykyisyyden ja tulevaisuuden välillä.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
