# C 010 — Rajoitettujen palasten tiedostotilastot

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–007 ja tätä laajennettua sarjaa edeltävät oppitunnit
**Seuralaisohjelma:** [`examples/c/c-010-bounded-chunk-file-statistics.c`](../../examples/c/c-010-bounded-chunk-file-statistics.c)
**Deterministinen testi:** [`tests/c-010-bounded-chunk-file-statistics.sh`](../../tests/c-010-bounded-chunk-file-statistics.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä oppitunnin jälkeen

Tämän oppitunnin lopussa osaat skannata binaaritiedoston rajoitetuissa paloissa, laskea kaikkien tavujen esiintymismäärät, nollaarvotavun esiintymät ja korkeat tavuarvot sekä tarkistaa `size_t` laskurit ylivuodon varalta.

## Kertausharjoitus

Vastaa ennen kuin avaat aiempia oppitunteja uudelleen.

1. Miksi tavutaulukolle tarvitaan eksplisiittinen `size_t` pituus?
2. Muodosta turvallinen eteenpäin-silmukan ehto, joka varmistaa `count` kelvollisille alkiolle.
3. Mitä on oltava totta siirtonopeudesta (shift count) tarkan 32‑bittisen sanan kohdalla?
4. Venäjä 007: käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Suuri syöte ei pidä lukea rajattomaan puskuriin. Skannaaja käyttää uudelleen pientä `uint8_t` palasta, käyttää `fread` palautettua määrää kelvollisena pituutena ja tarkistaa `ferror` lyhyen lukemisen jälkeen.

Ohjelma käyttää kannettavaa ISO C17:ää, tarkistaa paluuarvot kaikissa ulkoisissa operaatioissa ja pitää loogiset arvot erillään C‑merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain silloin, kun niiden täsmäleveys on osa oppitunnin sopimusta. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi toisiin prosesseihin tai muokkaa omia käskyjään.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    uint8_t buffer[64];
    size_t bytes = 0, zeros = 0, high = 0;
    FILE *in;
    if (argc != 2) { (void)fputs("Usage: stats INPUT\n", stderr); return EXIT_FAILURE; }
    in = fopen(argv[1], "rb");
    if (in == NULL) return EXIT_FAILURE;
    for (;;) {
        size_t got = fread(buffer, 1, sizeof buffer, in);
        if (SIZE_MAX - bytes < got) { (void)fclose(in); return EXIT_FAILURE; }
        for (size_t index = 0; index < got; ++index) {
            if (buffer[index] == UINT8_C(0)) ++zeros;
            if (buffer[index] >= UINT8_C(0x80)) ++high;
        }
        bytes += got;
        if (got < sizeof buffer) { if (ferror(in)) { (void)fclose(in); return EXIT_FAILURE; } break; }
    }
    if (fclose(in) != 0) return EXIT_FAILURE;
    if (printf("Bytes: %zu\nZero bytes: %zu\nHigh bytes: %zu\n", bytes, zeros, high) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita liitetty seuralaisohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-010-bounded-chunk-file-statistics \
  examples/c/c-010-bounded-chunk-file-statistics.c
./c-010-bounded-chunk-file-statistics fixture.bin
```

Odotettu tulostus:

```text
Bytes: 5
Zero bytes: 1
High bytes: 2
```

## Ohjattu harjoitus

1. Tunnista esimerkin selkeä syöteraja.
2. Nimeä tila- tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei yleensä käytä sentineliä, kuten `0x00`, taulukon yleisenä rajana.
4. Mainitse yksi virheellinen syöte, jonka seuralaisohjelma hylkää tai käsittelee turvallisesti.
5. Tuota yhdestä tätä välittömästi edeltävästä venäjäoppitunnista yksi tarkka kohdelause.

## Itsenäinen harjoitus

Lisää laskenta tulostettaville ASCII‑arvoille väliltä `0x20` — `0x7E`. Pidä palasarjan raja ennallaan ja käytä vain indeksejä, jotka ovat ehdottomasti pienempiä kuin `got`.

Pidä kaikki laskurit tyypissä `size_t`, validoi mahdollinen muunnos ennen kaventamista ja säilytä alkuperäinen olio, jos allokointi tai jäsentäminen epäonnistuu. Tämä on tietojenkäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei saa koskaan esittää sopivana turvallisuuteen, salaamiseen, todennukseen tai todellisen datan suojaamiseen.

## Validointi

Suorita repositorion testi:

```sh
./tests/c-010-bounded-chunk-file-statistics.sh
```

Sitten, missä tuettu, käännä AddressSanitizerilla ja UndefinedBehaviorSanitizerilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-010-sanitized \
  examples/c/c-010-bounded-chunk-file-statistics.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää syöterajan, asiaankuuluvan kokonaislukuleveyden, paluuarvotarkistukset, ylivuoto‑ ja siirtosuojan, deterministisen testivektorin sekä miksi esimerkki ei ole kryptografinen rakenne.

## Paljastus: vastaukset ja mallivastaus

1. Tavutietorakenne tarvitsee eksplisiittisen pituuden, koska nollaarvotavut voivat olla normaalia dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtoarvon on oltava ei‑negatiivinen ja ehdottomasti pienempi kuin promotoidun vasemman operaandin leveys.
4. **Я не читаю в библиотеке.** tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkistettu raja ja deterministinen testi tekevät sen käyttäytymisestä ennakoitavan; ne eivät kuitenkaan tee siitä kryptografista suojaa.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Venäjä 011 — ajan ilmaukset ja yksinkertainen päivittäinen aikataulu.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
