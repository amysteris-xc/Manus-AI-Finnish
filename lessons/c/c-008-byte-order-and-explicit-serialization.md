# C 008 — tavujärjestys ja eksplisiittinen sarjallistus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–007 ja välittömästi tätä laajennettua sarjaa edeltävät oppitunnit
**Seurantaohjelma:** [`examples/c/c-008-byte-order-and-explicit-serialization.c`](../../examples/c/c-008-byte-order-and-explicit-serialization.c)
**Deterministinen testi:** [`tests/c-008-byte-order-and-explicit-serialization.sh`](../../tests/c-008-byte-order-and-explicit-serialization.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat koodata ja dekoodata yhden `uint32_t` arvon neljän eksplisiittisesti järjestetyn `uint8_t` yksikön muodossa ilman riippuvuutta muistiasettelusta.

## Kertaus

Vastaa ennen aiempien oppituntien uudelleenläpikäyntiä.

1. Miksi tavupuskurilla tarvitaan eksplisiittinen `size_t` pituus?
2. Ilmoita turvallinen, etenevä silmukkaehto `count` kelvollisille alkioille.
3. Minkä on oltava totta siirtomäärälle tarkalle 32-bittiselle sanalle?
4. Venäjä 007:sta, käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Looginen numeerinen sana ei ole automaattisesti ulkoinen tavumuoto. Ohjelma käyttää big-endian-järjestystä ja hyödyntää siirtoja sekä maskeja kirjoittaakseen ja lukeakseen kukin tavu eksplisiittisesti.

Ohjelma on siirrettävä ISO C17 -standardin mukainen, tarkistaa paluuarvot jokaisessa ulkoisessa rajapinnassa ja pitää loogiset arvot erillään C-merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain silloin, kun niiden tarkka leveys on osa oppitunnin sopimusta. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin tai muokkaa suorituskoodiaan.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_INVALID_ARGUMENT = 1, STATUS_OUTPUT_FAILURE = 2 } Status;

static Status encode_be32(uint32_t value, uint8_t out[4])
{
    if (out == NULL) return STATUS_INVALID_ARGUMENT;
    out[0] = (uint8_t)(value >> 24);
    out[1] = (uint8_t)(value >> 16);
    out[2] = (uint8_t)(value >> 8);
    out[3] = (uint8_t)value;
    return STATUS_OK;
}

static Status decode_be32(const uint8_t in[4], uint32_t *value_out)
{
    if (in == NULL || value_out == NULL) return STATUS_INVALID_ARGUMENT;
    *value_out = ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) |
                 ((uint32_t)in[2] << 8) | (uint32_t)in[3];
    return STATUS_OK;
}

int main(void)
{
    const uint32_t input = UINT32_C(0x0123052A);
    uint8_t bytes[4];
    uint32_t decoded;
    if (encode_be32(input, bytes) != STATUS_OK || decode_be32(bytes, &decoded) != STATUS_OK) return EXIT_FAILURE;
    if (printf("Input word: 0x%08" PRIX32 "\nBig-endian bytes: %02" PRIX8 " %02" PRIX8 " %02" PRIX8 " %02" PRIX8 "\nDecoded word: 0x%08" PRIX32 "\n", input, bytes[0], bytes[1], bytes[2], bytes[3], decoded) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita liitteenä oleva seurantaohjelma täsmälleen näin:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-008-byte-order-and-explicit-serialization \
  examples/c/c-008-byte-order-and-explicit-serialization.c
./c-008-byte-order-and-explicit-serialization
```

Odotettu tulostus:

```text
Input word: 0x0123052A
Big-endian bytes: 01 23 05 2A
Decoded word: 0x0123052A
```

## Ohjattu harjoitus

1. Määritä esimerkin eksplisiittinen syötteen raja.
2. Nimeä tila- tai paluuarvo, joka tarkistetaan kunkin ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkissä ei käytetä sentinelliä, kuten `0x00`, yleisenä taulukon rajana.
4. Ilmoita yksi virheellinen syöte, jonka seurantaohjelma hylkää tai käsittelee turvallisesti.
5. Välittömästi edeltävästä venäjän oppitunnista tuota yksi tarkka kohdeilmaisu.

## Itsenäinen harjoitus

Kirjoita `encode_le32` ja `decode_le32` dokumentoidulle little-endian -muodolle. Testaa sana, jossa on eri arvot kaikissa neljässä tavussa. Älä tyyppimuunna `uint32_t *` tavupointeriksi korvaamaan muodon määrittelyä.

Pidä kaikki laskurit `size_t` -tyyppisinä, validoi kaikki muunnokset ennen niiden kaventamista, ja säilytä alkuperäinen objekti, kun allokaatio tai jäsennys epäonnistuu. Tämä on tietojenkäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei saa koskaan esittää sopivana turvallisuus-, salaus-, autentikointi- tai todellisen datan suojaustarkoituksiin.

## Varmistus

Suorita repositorion testi:

```sh
./tests/c-008-byte-order-and-explicit-serialization.sh
```

Sitten, missä se on tuettu, käännä AddressSanitizerin ja UndefinedBehaviorSanitizerin kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-008-sanitized \
  examples/c/c-008-byte-order-and-explicit-serialization.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää syötteen rajan, asiaankuuluvan kokonaislukuleveyden, paluuarvojen tarkistukset, ylivuodon tai siirron suojan, deterministisen testivektorin ja miksi esimerkki ei ole kryptografinen rakenne.

## Paljastus: vastaukset ja malli­vastaus

1. Tavupuskuri tarvitsee eksplisiittisen pituuden, koska nollaarvoiset tavut voivat olla tavallista dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtomäärän on oltava ei-negatiivinen ja ehdottomasti pienempi kuin vasemman operaandin promovoidun leveyden.
4. **Я не читаю в библиотеке.** tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkistettu raja ja deterministinen testi tekevät sen käyttäytymisestä tarkasteltavissa olevan; ne eivät kuitenkaan tee siitä kryptografista turvamekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Venäjä 009 — monikon substantiivit ja pienet ryhmät.**

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
