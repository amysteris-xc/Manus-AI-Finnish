# C 017 — Kiinteät binaaritietueet ja katkaisemisen käsittely

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–007 ja sitä edeltävät välittömät oppitunnit
**Kumppaniohjelma:** [`examples/c/c-017-fixed-binary-records-and-truncation.c`](../../examples/c/c-017-fixed-binary-records-and-truncation.c)
**Deterministinen testi:** [`tests/c-017-fixed-binary-records-and-truncation.sh`](../../tests/c-017-fixed-binary-records-and-truncation.sh)
**Tekijä:** Manus AI

## Mitä pystyt tekemään

Tämän oppitunnin lopussa osaat lukea tiedoston, joka koostuu eksplisiittisistä kahdeksantavuisista tietueista, hylätä katkaistun viimeisen tietueen ja dekoodata kukin tietue ilman, että tiedostomuotona käytetään C:n structia.

## Kertaus

Vastaa ennen kuin avaat aiemmat oppitunnit uudelleen.

1. Miksi tavupuskurilla pitää olla eksplisiittinen `size_t` pituus?
2. Anna turvallinen etenevän silmukan ehto `count` kelvollisille alkioille.
3. Mitä ehtoa siirtoluvun on täytettävä tarkkaa 32-bittistä sanaa varten?
4. Venäjä 007: käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Kiinteä tietueformaatti edellyttää silti tarkastettua I/O:ta. Tämä lukija pyytää `fread` kahdeksan tavun yksikköjä, käsittelee `0` normaalina lopetuksena vasta sen jälkeen kun `ferror` on tarkistettu, ja katsoo katkaisuksi kaikki alle kahdeksan tavun mutta nollasta poikkeavat paluut.

Ohjelma käyttää siirrettävää ISO C17:ää, tarkistaa paluuarvot jokaisessa ulkoisessa rajapinnassa ja erottaa loogiset arvot C-merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain siellä, missä niiden täsmälleen määritelty leveys kuuluu oppitunnin sopimukseen. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin eikä muokkaa omia käskyjään.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t get_be32(const uint8_t in[4]) { return ((uint32_t)in[0] << 24) | ((uint32_t)in[1] << 16) | ((uint32_t)in[2] << 8) | in[3]; }
static uint16_t get_be16(const uint8_t in[2]) { return (uint16_t)(((uint16_t)in[0] << 8) | in[1]); }

int main(int argc, char *argv[])
{
    uint8_t record[8];
    size_t count = 0;
    FILE *in;
    if (argc != 2) return EXIT_FAILURE;
    in = fopen(argv[1], "rb"); if (in == NULL) return EXIT_FAILURE;
    for (;;) {
        size_t got = fread(record, 1, sizeof record, in);
        if (got == 0) { if (ferror(in)) { (void)fclose(in); return EXIT_FAILURE; } break; }
        if (got != sizeof record) { (void)fclose(in); (void)fputs("Truncated record.\n", stderr); return EXIT_FAILURE; }
        if (printf("Record %zu: id=%" PRIu32 " flags=%" PRIu16 " kind=%" PRIu8 " status=%" PRIu8 "\n", count, get_be32(record), get_be16(record + 4), record[6], record[7]) < 0) { (void)fclose(in); return EXIT_FAILURE; }
        if (count == SIZE_MAX) { (void)fclose(in); return EXIT_FAILURE; }
        ++count;
    }
    if (fclose(in) != 0) return EXIT_FAILURE;
    if (printf("Records: %zu\n", count) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita mukana toimitettu kumppaniohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-017-fixed-binary-records-and-truncation \
  examples/c/c-017-fixed-binary-records-and-truncation.c
./c-017-fixed-binary-records-and-truncation fixture.bin
```

Odotettu tulostus:

```text
Record 0: id=42 flags=5 kind=1 status=0
Record 1: id=291 flags=160 kind=2 status=1
Records: 2
```

## Ohjattu harjoitus

1. Määritä esimerkissä oleva eksplisiittinen syötteen raja.
2. Nimeä tila- tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkissä ei käytetä sentineliä, kuten `0x00`, yleisenä taulukon rajana.
4. Kerro yksi virheellinen syöte, jonka kumppaniohjelma hylkää tai käsittelee turvallisesti.
5. Tuota yksi tarkka kohdelause välittömästi edeltävästä venäjän oppitunnista.

## Itsenäinen harjoitus

Lisää enimmäistietueiden määrä ja tarkistettu yhteenvetokenttä. Säilytä tavujen eksplisiittinen asettelu; älä kirjoita tai lue raakaa C structia kannettavana tiedostomuotona.

Pidä kaikki laskurit `size_t` -tyyppisinä, validoi kaikki muunnokset ennen niiden kaventamista ja säilytä alkuperäinen olio, jos allokointi- tai jäsentämistoiminto epäonnistuu. Tämä on aineiston käsittelyyn liittyvä harjoitus, ei kryptografinen primitiivi. Sitä ei koskaan saa esittää sopivana turvallisuuteen, salaamiseen, todennukseen tai todellisten tietojen suojaamiseen.

## Validointi

Suorita repositoriotesti:

```sh
./tests/c-017-fixed-binary-records-and-truncation.sh
```

Sitten, siellä missä tuki on saatavilla, käännä AddressSanitizerin ja UndefinedBehaviorSanitizerin kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-017-sanitized \
  examples/c/c-017-fixed-binary-records-and-truncation.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää syötterajan, asiaankuuluvan kokonaislukuleveyden, paluuarvotarkistukset, ylivuodon tai siirronsuojauksen, deterministisen testivektorin ja miksi esimerkki ei ole kryptografinen rakenne.

## Ratkaisut ja mallivastaus

1. Tavupuskurilla on eksplisiittinen pituus, koska nollaarvoiset tavut voivat olla varsinaista dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtoluvun on oltava ei-negatiivinen ja ehdottomasti pienempi kuin laajennetun vasemman operandin leveys.
4. **Я не читаю в библиотеке.** tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkistetut rajat ja deterministinen testi tekevät sen käytöksen ennustettavaksi; ne eivät tee siitä kryptografista suojausmekanismia.

## Seuraava oppitunti

Seuraavaksi ajoitettu oppitunti on **Venäjä 018 — suunnat ja prepositiot yleisissä paikoissa.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
