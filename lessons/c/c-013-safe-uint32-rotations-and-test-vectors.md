# C 013 — Turvalliset `uint32_t` rotaatiot ja testivektorit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–007 ja tätä laajennettua sarjaa edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-013-safe-uint32-rotations-and-test-vectors.c`](../../examples/c/c-013-safe-uint32-rotations-and-test-vectors.c)
**Deterministinen testi:** [`tests/c-013-safe-uint32-rotations-and-test-vectors.sh`](../../tests/c-013-safe-uint32-rotations-and-test-vectors.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Oppitunnin lopussa osaat toteuttaa rajatun 32-bittisen vasemman rotaation, jossa siirtoluku tarkistetaan, ja varmentaa toteutuksen kiinteillä testivektoreilla.

## Palautusharjoitus

Vastaa ennen kuin avaat aiemmat oppitunnit uudelleen.

1. Miksi tavupuskuri tarvitsee eksplisiittisen `size_t` pituuden?
2. Ilmoita turvallinen etenevän silmukan ehto `count` kelvolliselle alkiomäärälle.
3. Mitä ehtoja siirtoluvun on täytettävä tarkassa 32-bittisessä sanassa?
4. Venäjä 007 -oppitunnista, käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Rotaatio ei ole tavallinen siirto. Apufunktio palauttaa syötteen muuttumattomana, jos siirtoluku on nolla, ja käyttää vain siirtolukuja 1–31, jotta kumpikaan siirto-operaandi ei saavuta arvoa 32.

Ohjelma käyttää siirrettävää ISO C17 -koodia, tarkistaa paluuarvot jokaisessa ulkoisessa rajapinnassa ja pitää loogiset arvot erillään C-merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain siellä, missä niiden täsmälleen määritelty leveys on osa oppitunnin sopimusta. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin tai muokkaa omia käskyjään.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int rotate_left32(uint32_t input, unsigned int count, uint32_t *out)
{
    if (out == NULL || count >= 32U) return 0;
    if (count == 0U) { *out = input; return 1; }
    *out = (input << count) | (input >> (32U - count));
    return 1;
}

int main(void)
{
    uint32_t first, second;
    if (!rotate_left32(UINT32_C(0x12345678), 8U, &first) || !rotate_left32(UINT32_C(0x80000001), 1U, &second)) return EXIT_FAILURE;
    if (printf("rotl32(0x12345678, 8) = 0x%08" PRIX32 "\nrotl32(0x80000001, 1) = 0x%08" PRIX32 "\n", first, second) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita mukana oleva ohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-013-safe-uint32-rotations-and-test-vectors \
  examples/c/c-013-safe-uint32-rotations-and-test-vectors.c
./c-013-safe-uint32-rotations-and-test-vectors
```

Odotettu tuloste:

```text
rotl32(0x12345678, 8) = 0x34567812
rotl32(0x80000001, 1) = 0x00000003
```

## Ohjattu harjoitus

1. Tunnista esimerkin eksplisiittinen syötteen raja.
2. Nimeä tila- tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei käytä sentineliä kuten `0x00` yleisenä taulukon rajana.
4. Mainitse yksi virheellinen syöte, jonka mukana oleva ohjelma hylkää tai käsittelee turvallisesti.
5. Kirjoita yksi täsmällinen lause välittömästi edeltävästä venäjän oppitunnista.

## Itsenäinen harjoitus

Kirjoita vastaava oikeanpuoleisen rotaation apufunktio. Käsittele siirtoluku nolla ennen kuin lasket `32 - count`, ja hylkää siirtoluvut 32 tai suuremmat.

Säilytä kaikki siirtoluvut `size_t`, tarkista kaikki muunnokset ennen niiden kaventamista, ja säilytä alkuperäinen objekti, kun allokointi tai jäsentäminen epäonnistuu. Tämä on tietojenkäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei koskaan saa esittää sopivana turvallisuuteen, salaamiseen, todennukseen tai todellisten tietojen suojaamiseen.

## Validointi

Suorita repositorion testi:

```sh
./tests/c-013-safe-uint32-rotations-and-test-vectors.sh
```

Sitten, jos mahdollista, käännä käyttäen AddressSanitizeria ja UndefinedBehaviorSanitizeria:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-013-sanitized \
  examples/c/c-013-safe-uint32-rotations-and-test-vectors.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää syötteen rajauksen, asiaankuuluvan kokonaislukuleveyden, paluuarvotarkistukset, ylivuoto- tai siirtosuojan, deterministisen testivektorin ja miksi esimerkki ei ole kryptografinen rakenne.

## Paljastus: vastaukset ja mallivastaus

1. Tavupuskuri käyttää eksplisiittistä pituutta, koska nolla-arvoiset tavut voivat olla tavallista dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtoluvun on oltava ei-negatiivinen ja ehdottomasti pienempi kuin vasemman operaandin promotoidun tyypin leveys.
4. **Я не читаю в библиотеке.** tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkistettu rajaus ja deterministinen testi tekevät sen käyttäytymisestä tarkasteltavan; ne eivät tee siitä kryptografista suojausmekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Venäjä 014 — yksinkertaiset tulevaisuudensuunnitelmat käyttäen буду ja infinitiiveja.**

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
