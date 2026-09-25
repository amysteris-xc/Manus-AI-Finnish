# C 011 — Tiukka heksadesimaalinen jäsentäminen ja kenttien validointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–007 ja tätä laajennettua sarjaa edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-011-strict-hexadecimal-parsing-and-fields.c`](../../examples/c/c-011-strict-hexadecimal-parsing-and-fields.c)
**Deterministinen testi:** [`tests/c-011-strict-hexadecimal-parsing-and-fields.sh`](../../tests/c-011-strict-hexadecimal-parsing-and-fields.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi osaat jäsentää yhden heksadesimaalisen `uint32_t` -argumentin tiukasti, hylätä virheelliset tai ylivuotavat merkkijonot ja raportoida niiden nimetyt bittikentät.

## Muistiharjoitus

Vastaa ennen kuin avaat aiemmat oppitunnit uudelleen.

1. Miksi tavubufferi tarvitsee eksplisiittisen `size_t` pituuden?
2. Määritä turvallinen eteenpäin etenevän silmukan ehto `count` kelvollisille alkioille.
3. Mikä pitää paikkansa siirtomäärässä, kun käsitellään täsmällistä 32-bittistä sanaa?
4. Venäjä 007:stä käännä **Я не читаю в библиотеке.**

## Keskeinen käsite

Tekstistä kokonaisluvuksi muuntaminen on rajapinta. `strtoul` täytyy tarkistaa `errno`:llä, loppuosapointerin ja alueentarkistuksen avulla ennen kaventamista tyyppiin `uint32_t`.

Ohjelma noudattaa kannettavaa ISO C17 -standardia, tarkistaa paluuarvot jokaisessa ulkoisessa rajapisteessä ja pitää loogiset arvot erillään C-merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain silloin, kun niiden täsmällinen merkitys on osa oppitunnin sopimusta. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi prosesseihin tai muokkaa omia käskyjään.

## Työstetty esimerkki

```c
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *end = NULL;
    unsigned long parsed;
    uint32_t word;
    if (argc != 2) { (void)fputs("Usage: fields HEX_WORD\n", stderr); return EXIT_FAILURE; }
    errno = 0;
    parsed = strtoul(argv[1], &end, 0);
    if (errno != 0 || end == argv[1] || *end != '\0' || parsed > UINT32_MAX) { (void)fputs("Invalid uint32 value.\n", stderr); return EXIT_FAILURE; }
    word = (uint32_t)parsed;
    if (printf("Word: 0x%08" PRIX32 "\nCategory: %" PRIu32 "\nFlags: %" PRIu32 "\nSequence: %" PRIu32 "\n", word, word & UINT32_C(0xFF), (word >> 8) & UINT32_C(0xFF), (word >> 16) & UINT32_C(0xFFFF)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita tallennettu mukana oleva ohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-011-strict-hexadecimal-parsing-and-fields \
  examples/c/c-011-strict-hexadecimal-parsing-and-fields.c
./c-011-strict-hexadecimal-parsing-and-fields 0xBEEFA010
```

Odotettu tuloste:

```text
Word: 0xBEEFA010
Category: 16
Flags: 160
Sequence: 48879
```

## Ohjattu harjoitus

1. Tunnista esimerkin eksplisiittinen syöterajapinta.
2. Nimeä tilakoodi tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei käytä sentinel-arvoa kuten `0x00` yleisenä taulun rajana.
4. Mainitse yksi virheellinen syöte, jonka mukana oleva ohjelma hylkää tai käsittelee turvallisesti.
5. Kirjoita yhdestä välittömästi edeltäneestä Venäjä-oppitunnista yksi tarkka kohdelause.

## Itsenäinen harjoitus

Vaadi eksplisiittistä `0x` -etuliitettä ennen parserin kutsumista, ja säilytä kaikki olemassa olevat virheellisen tekstin ja alueen tarkistukset.

Pidä kaikki laskurit `size_t` -tyyppisinä, validoi kaikki muunnokset ennen kaventamista ja säilytä alkuperäinen objekti, kun allokointi- tai jäsentämisoperaatio epäonnistuu. Tämä on datankäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei koskaan saa esittää soveltuvaksi turvallisuuteen, salaamiseen, autentikointiin tai todellisen datan suojaukseen.

## Varmistus

Suorita repositorion testi:

```sh
./tests/c-011-strict-hexadecimal-parsing-and-fields.sh
```

Sitten, missä tuettu, käännä AddressSanitizerilla ja UndefinedBehaviorSanitizerilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-011-sanitized \
  examples/c/c-011-strict-hexadecimal-parsing-and-fields.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää syöterajapinnan, relevantin kokonaislukuleveyden, paluuarvotarkistukset, ylivuoto- tai siirtosuojauksen, deterministisen testivektorin ja miksi esimerkki ei ole kryptografinen rakenne.

## Vastausvihje: vastaukset ja mallivastaus

1. Tavubufferi käyttää eksplisiittistä pituutta, koska nolla-arvoiset tavut voivat olla tavallista dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtomäärän on oltava ei-negatiivinen ja ehdottomasti pienempi kuin vasemman operaandin promotoidun leveyden.
4. **Я не читаю в библиотеке.** tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkastettu rajapinta ja deterministinen testi tekevät sen käyttäytymisestä toistettavan; ne eivät tee siitä kryptografista turvallisuusmekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Venäjä 012 — yksikön imperfekti ja suku.**

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc — cppreference.com"
