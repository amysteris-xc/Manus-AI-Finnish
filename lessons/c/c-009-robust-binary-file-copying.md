# C 009 — Luotettava binaaritiedoston kopiointi

**Arvioitu opiskeluajan pituus:** 25–35 minuuttia
**Esitiedot:** C 001–007 ja tätä laajennettua sarjaa edeltävät oppitunnit
**Kumppaniohjelma:** [`examples/c/c-009-robust-binary-file-copying.c`](../../examples/c/c-009-robust-binary-file-copying.c)
**Deterministinen testi:** [`tests/c-009-robust-binary-file-copying.sh`](../../tests/c-009-robust-binary-file-copying.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopussa osaat kopioida binaaritiedoston kiinteissä paloissa käyttäen tarkistettuja `fread`, käsitellä osittaisia kirjoituksia, käyttää `ferror` ja `fflush`, sekä tarkistaa sulkemisen tulokset.

## Muistiharjoitus

Vastaa ennen aiempien oppituntien uudelleen avaamista.

1. Miksi tavubufferille tarvitaan eksplisiittinen `size_t` pituus?
2. Ilmoita turvallinen eteenpäin etenevän silmukan invarianssi ehtona `count` kelvollisille alkioille.
3. Minkä ehdon shift‑laskurin täytyy täyttää tarkan 32‑bittisen sanan käsittelyssä?
4. Russian 007:sta käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Tiedostojen I/O palauttaa arvoja, jotka on tarkistettava. `fread` voi palauttaa vähemmän tavuja tiedoston lopussa tai virheen sattuessa, kun taas `fwrite` voi kirjoittaa vähemmän tavuja kuin pyydettiin. Tämä ohjelma seuraa tavumääriä `size_t` avulla ja kirjoittaa jokaisen ei‑tyhjän palan kokonaan ennen uudelleenkirjoitusta.

Ohjelma käyttää kannettavaa ISO C17:ää, tarkistaa paluuarvot kaikissa ulkoisissa rajapinnoissa ja pitää loogiset arvot erillään C‑merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain silloin, kun niiden tarkka bittileveys on osa oppitunnin sopimusta. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin tai muokkaa omia käskyjään.

## Työstetty esimerkki

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int write_all(FILE *out, const uint8_t *buffer, size_t count)
{
    size_t written = 0;
    while (written < count) {
        size_t step = fwrite(buffer + written, 1, count - written, out);
        if (step == 0) return 0;
        written += step;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    uint8_t buffer[64];
    size_t total = 0;
    FILE *in;
    FILE *out;
    if (argc != 3) { (void)fputs("Usage: copy INPUT OUTPUT\n", stderr); return EXIT_FAILURE; }
    in = fopen(argv[1], "rb");
    if (in == NULL) { (void)fputs("Cannot open input.\n", stderr); return EXIT_FAILURE; }
    out = fopen(argv[2], "wb");
    if (out == NULL) { (void)fclose(in); (void)fputs("Cannot open output.\n", stderr); return EXIT_FAILURE; }
    for (;;) {
        size_t got = fread(buffer, 1, sizeof buffer, in);
        if (got > 0) {
            if (SIZE_MAX - total < got || !write_all(out, buffer, got)) { (void)fclose(in); (void)fclose(out); return EXIT_FAILURE; }
            total += got;
        }
        if (got < sizeof buffer) {
            if (ferror(in)) { (void)fclose(in); (void)fclose(out); return EXIT_FAILURE; }
            break;
        }
    }
    {
        int flush_status = fflush(out);
        int input_close_status = fclose(in);
        int output_close_status = fclose(out);

        if (flush_status != 0 || input_close_status != 0 ||
            output_close_status != 0) return EXIT_FAILURE;
    }
    if (printf("Copied bytes: %zu\n", total) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita repositorioon tallennettu kumppaniohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-009-robust-binary-file-copying \
  examples/c/c-009-robust-binary-file-copying.c
./c-009-robust-binary-file-copying fixture.bin copied.bin
```

Odotettu tuloste:

```text
Copied bytes: 5
```

## Ohjattu harjoitus

1. Tunnista esimerkin eksplisiittinen syötteen raja.
2. Nimeä tila‑ tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei käytä sentineliä, kuten `0x00`, yleisenä taulukon rajana.
4. Ilmoita yksi virheellinen syöte, jonka kumppaniohjelma hylkää tai käsittelee turvallisesti.
5. Anna edellisestä venäjänkielen oppitunnista yksi tarkka kohdekäännös.

## Itsenäinen harjoitus

Laajenna kopioijaa siten, että se hylkää, jos syöte‑ ja tulostepolut ovat yhtenevät, ennen kohdekohteen avaamista. Säilytä tarkistetut luvut, osittaisten kirjoitusten käsittely ja sulkemisen tarkistukset.

Pidä kaikki laskurit `size_t` rajoissa, validoi kaikki muunnokset ennen kavennusta ja säilytä alkuperäinen objekti, jos allokointi‑ tai jäsentämisoperaatio epäonnistuu. Tämä on tiedonkäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei koskaan tule esittää sopivana turva‑, salaus‑, autentikointi‑ tai todellisten tietojen suojauskäyttöön.

## Validointi

Suorita repositorion testi:

```sh
./tests/c-009-robust-binary-file-copying.sh
```

Sitten, missä se on tuettu, käännä AddressSanitizerilla ja UndefinedBehaviorSanitizerilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-009-sanitized \
  examples/c/c-009-robust-binary-file-copying.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt selittämään syötteen rajan, relevantin kokonaislukuleveyden, paluuarvojen tarkistamisen, ylivuotojen ja shift‑operaatioiden suojauksen, deterministisen testivektorin sekä miksi esimerkki ei ole kryptografinen rakenne.

## Spoileri: vastaukset ja mallivastaus

1. Tavubufferilla on eksplisiittinen pituus, koska nollaarvoiset tavut voivat olla normaalia dataa.
2. Turvallinen ehto on `index < count`.
3. Shift‑laskurin arvon on oltava ei‑negatiivinen ja selvästi pienempi kuin sen tyypin bittileveys, jolla siirto‑operaatio suoritetaan.
4. **Я не читаю в библиотеке.** tarkoittaa 'En lue kirjastossa.'
5. Esimerkin tarkistettu raja ja deterministinen testi tekevät sen käyttäytymisestä ennustettavaa; ne eivät tee siitä kryptografista turvamekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Russian 010 — Elottomat suorat objektit akkusatiivissa.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
