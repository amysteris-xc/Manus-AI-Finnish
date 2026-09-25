# C 015 — Dynaaminen tiedoston luku tarkistetulla kasvulla

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–007 ja tätä laajennettua sarjaa edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-015-dynamic-file-reading-with-checked-growth.c`](../../examples/c/c-015-dynamic-file-reading-with-checked-growth.c)
**Deterministinen testi:** [`tests/c-015-dynamic-file-reading-with-checked-growth.sh`](../../tests/c-015-dynamic-file-reading-with-checked-growth.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat lukea binaaritiedoston dynaamiseen `uint8_t` -puskurille käyttäen rajattuja lukupaloja, tarkistettua aritmetiikkaa, väliaikaista `realloc` -osoitinta ja asianmukaista siivousta.

## Muistiharjoitus

Vastaa ennen kuin avaat aiemmat oppitunnit uudelleen.

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus?
2. Ilmaise turvallinen eteenpäin-luuppiehto `count` kelvollisille alkioille.
3. Mitä on pidettävä paikkansa siirtojen lukumäärästä tarkassa 32-bittisessä sanassa?
4. Venäjä 007: käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Lukija yhdistää rajatun I/O:n ja dynaamisen tallennuksen. Se liittää vain ne tavut `fread`, jotka todellisuudessa palautetaan, tarkistaa `ferror` lyhyen lukemisen jälkeen ja pitää vanhan allokaation kelvollisena, jos kasvatus epäonnistuu.

Ohjelma on kirjoitettu siirrettävään ISO C17 -standardiin noudattaen, tarkistaa paluuarvot kaikissa ulkoisissa rajapinnoissa ja pitää loogiset arvot erillään C-merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain siellä, missä niiden täsmäleveys kuuluu oppitunnin sopimukseen. Ohjelma ei generoi shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin eikä muokkaa omia käskyjään.

## Ratkaistu esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t *data; size_t length; size_t capacity; } Buffer;

static int append(Buffer *buffer, const uint8_t *source, size_t count)
{
    size_t needed;
    uint8_t *temporary;
    if (buffer == NULL || source == NULL || count == 0) return count == 0;
    if (SIZE_MAX - buffer->length < count) return 0;
    needed = buffer->length + count;
    if (needed > buffer->capacity) {
        size_t capacity = buffer->capacity == 0 ? 64 : buffer->capacity;
        while (capacity < needed) { if (capacity > SIZE_MAX / 2) return 0; capacity *= 2; }
        temporary = realloc(buffer->data, capacity * sizeof *buffer->data);
        if (temporary == NULL) return 0;
        buffer->data = temporary; buffer->capacity = capacity;
    }
    for (size_t index = 0; index < count; ++index) buffer->data[buffer->length + index] = source[index];
    buffer->length = needed;
    return 1;
}

int main(int argc, char *argv[])
{
    uint8_t chunk[32];
    Buffer buffer = { NULL, 0, 0 };
    FILE *in;
    if (argc != 2) return EXIT_FAILURE;
    in = fopen(argv[1], "rb"); if (in == NULL) return EXIT_FAILURE;
    for (;;) { size_t got = fread(chunk, 1, sizeof chunk, in); if (!append(&buffer, chunk, got)) { free(buffer.data); (void)fclose(in); return EXIT_FAILURE; } if (got < sizeof chunk) { if (ferror(in)) { free(buffer.data); (void)fclose(in); return EXIT_FAILURE; } break; } }
    if (fclose(in) != 0 || buffer.length == 0 || buffer.data == NULL) {
        free(buffer.data);
        return EXIT_FAILURE;
    }
    if (printf("Bytes loaded: %zu\nFirst byte: 0x%02" PRIX8 "\nLast byte: 0x%02" PRIX8 "\n", buffer.length, buffer.data[0], buffer.data[buffer.length - 1]) < 0) { free(buffer.data); return EXIT_FAILURE; }
    free(buffer.data);
    return EXIT_SUCCESS;
}
```

Käännä ja suorita mukana toimitettu kumppaniohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-015-dynamic-file-reading-with-checked-growth \
  examples/c/c-015-dynamic-file-reading-with-checked-growth.c
./c-015-dynamic-file-reading-with-checked-growth fixture.bin
```

Odotettu tuloste:

```text
Bytes loaded: 5
First byte: 0x41
Last byte: 0x43
```

## Ohjattu harjoitus

1. Tunnista esimerkin eksplisiittinen syöteraja.
2. Nimeä tila- tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei käytä sentineliä kuten `0x00` yleisenä taulun rajana.
4. Mainitse yksi virheellinen syöte, jonka kumppaniohjelma hylkää tai käsittelee turvallisesti.
5. Tuota yksi tarkka kohdeilmaisu välittömästi edeltävältä venäjän oppitunnilta.

## Itsenäinen harjoitus

Lisää eksplisiittinen suurin sallittu tiedostokoko ja hylkää syöte ennen kuin kasvatuspyyntö ylittäisi tämän rajan. Perustele, miksi maksimi on tuotteen vaatimus eikä mielivaltainen allokaattorin sisäinen raja.

Pidä kaikki lukumäärät `size_t` -tyyppisinä, validoi kaikki muunnokset ennen kavennusta ja säilytä alkuperäinen objekti, jos allokointi- tai jäsentämisoperaatio epäonnistuu. Tämä on tietojenkäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei missään tapauksessa pidä esittää sopivana turvallisuuteen, salaukseen, autentikointiin tai todellisten tietojen suojaamiseen.

## Validointi

Suorita repositorion testi:

```sh
./tests/c-015-dynamic-file-reading-with-checked-growth.sh
```

Kun tuki on saatavilla, käännä myös AddressSanitizerin ja UndefinedBehaviorSanitizerin kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-015-sanitized \
  examples/c/c-015-dynamic-file-reading-with-checked-growth.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää syöterajan, siihen liittyvän kokonaislukuleveyden, paluuarvojen tarkistukset, ylivuoto- ja siirtosuojauksen, deterministisen testivektorin sekä miksi esimerkki ei ole kryptografinen konstruktio.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskuri tarvitsee eksplisiittisen pituuden, koska nollaarvot voivat olla osa dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtoluvun on oltava ei-negatiivinen ja tiukasti pienempi kuin promotoidun vasemman operandin leveys.
4. **Я не читаю в библиотеке.** tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkistettu raja ja deterministinen testi tekevät sen käyttäytymisestä todennettavan; ne eivät tee siitä kryptografista turvamekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Venäjä 016 — halu: 'хочу' ja infinitiivit.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
