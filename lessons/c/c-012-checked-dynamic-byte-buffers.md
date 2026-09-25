# C 012 — Tarkistetut dynaamiset tavupuskurit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–007 ja tätä laajennettua sarjaa edeltävät välittömät oppitunnit
**Kumppaniohjelma:** [`examples/c/c-012-checked-dynamic-byte-buffers.c`](../../examples/c/c-012-checked-dynamic-byte-buffers.c)
**Deterministinen testi:** [`tests/c-012-checked-dynamic-byte-buffers.sh`](../../tests/c-012-checked-dynamic-byte-buffers.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat kasvattaa dynaamisen tavupuskurin käyttämällä tarkistettua kapasiteettiaritmetiikkaa ja väliaikaista `realloc`-osoitinta.

## Muistiharjoitus

Vastaa ennen kuin avaat aikaisemmat oppitunnit uudelleen.

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t`-pituus?
2. Ilmoita turvallinen eteenpäin käyvä silmukan ehto `count` kelvollisille alkioille.
3. Mitä siirtomäärän on oltava totta tarkan 32-bittisen sanan kohdalla?
4. Venäjän oppitunnista 007, käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Dynaaminen tallennustila perustuu osoitin–pituus–kapasiteetti-sopimukseen. Ennen kasvattamista tarkista aritmetiikka `SIZE_MAX`; osoita `realloc` tulos väliaikaiseen osoittimeen, jotta epäonnistunut allokaatio ei aiheuta vanhan puskurin menettämistä.

Ohjelma on kirjoitettu siirrettävälle ISO C17 -standardille, tarkistaa paluuarvot kaikissa ulkoisissa rajapinnoissa ja pitää binääridatan erillään C-merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` ainoastaan siellä, missä niiden täsmälleen määritetty leveys kuuluu oppitunnin sopimukseen. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin eikä muokkaa omia käskyjään.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t *data; size_t length; size_t capacity; } ByteVector;

static int append(ByteVector *vector, uint8_t value)
{
    if (vector == NULL) return 0;
    if (vector->length == vector->capacity) {
        size_t new_capacity = vector->capacity == 0 ? 4 : vector->capacity * 2;
        uint8_t *temporary;
        if (new_capacity < vector->capacity || new_capacity > SIZE_MAX / sizeof *vector->data) return 0;
        temporary = realloc(vector->data, new_capacity * sizeof *vector->data);
        if (temporary == NULL) return 0;
        vector->data = temporary;
        vector->capacity = new_capacity;
    }
    vector->data[vector->length++] = value;
    return 1;
}

int main(void)
{
    const uint8_t input[] = { UINT8_C(0x41), UINT8_C(0x00), UINT8_C(0xA7), UINT8_C(0x42), UINT8_C(0x7F) };
    ByteVector vector = { NULL, 0, 0 };
    for (size_t index = 0; index < sizeof input / sizeof input[0]; ++index) if (!append(&vector, input[index])) { free(vector.data); return EXIT_FAILURE; }
    if (printf("Length: %zu\nBytes:", vector.length) < 0) { free(vector.data); return EXIT_FAILURE; }
    for (size_t index = 0; index < vector.length; ++index) if (printf(" %02" PRIX8, vector.data[index]) < 0) { free(vector.data); return EXIT_FAILURE; }
    if (printf("\nCapacity: %zu\n", vector.capacity) < 0) { free(vector.data); return EXIT_FAILURE; }
    free(vector.data);
    return EXIT_SUCCESS;
}
```

Käännä ja suorita tallennettu kumppaniohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-012-checked-dynamic-byte-buffers \
  examples/c/c-012-checked-dynamic-byte-buffers.c
./c-012-checked-dynamic-byte-buffers
```

Odotettu tulostus:

```text
Length: 5
Bytes: 41 00 A7 42 7F
Capacity: 8
```

## Ohjattu harjoitus

1. Tunnista esimerkissä mainittu eksplisiittinen syötteen raja.
2. Nimeä tila tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei käytä sentinelliä kuten `0x00` yleisenä taulun rajana.
4. Luettele yksi virheellinen syöte, jonka kumppaniohjelma hylkää tai käsittelee turvallisesti.
5. Anna yksi täsmällinen kohdelause välittömästi edeltävästä venäjän oppitunnista.

## Itsenäinen harjoitus

Kirjoita `append_many` käyttäen tarkistettua `length + count` -laskentaa. Älä laske tarvittavaa kokoa vasta sen jälkeen, kun ylivuoto on voinut jo tapahtua.

Pidä kaikki lukumäärät `size_t`:ssa, validoi kaikki muunnokset ennen kaventamista ja säilytä alkuperäinen objekti, jos allokaatio tai jäsentäminen epäonnistuu. Tämä on datankäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei missään tapauksessa saa esittää sopivana turvallisuus-, salaus-, autentikointi- tai todellisten tietojen suojauskäyttöön.

## Vahvistus

Suorita repositorion testi:

```sh
./tests/c-012-checked-dynamic-byte-buffers.sh
```

Sitten, jos tuettu, käännä AddressSanitizerilla ja UndefinedBehaviorSanitizerilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-012-sanitized \
  examples/c/c-012-checked-dynamic-byte-buffers.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt selittämään syötteen rajan, kyseisen kokonaislukuleveyden, paluuarvojen tarkistukset, ylivuoto- tai siirtosuojan, deterministisen testivektorin ja miksi esimerkki ei ole kryptografinen konstruktio.

## Paljastus: vastaukset ja mallivastaus

1. Tavupuskuri käyttää eksplisiittistä pituutta, koska nolla-arvoiset tavut voivat olla tavallista dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtomäärän tulee olla ei-negatiivinen ja ehdottomasti pienempi kuin vasemman operandin promotoitu leveys.
4. **Я не читаю в библиотеке.** tarkoittaa “En lue kirjastossa.”
5. Esimerkin tarkistettu raja ja deterministinen testi tekevät sen käyttäytymisen katselmoitavaksi; ne eivät tee siitä kryptografista turvamekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Venäjä 013 — monikon imperfekti ja ihmisryhmät.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
