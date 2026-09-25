# C 014 — Tarkistussummat, testivektorit ja ei-kryptografinen eheys

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–007 ja tätä laajennettua sarjaa edeltävät oppitunnit
**Seuralaisohjelma:** [`examples/c/c-014-checksums-and-noncryptographic-integrity.c`](../../examples/c/c-014-checksums-and-noncryptographic-integrity.c)
**Deterministinen testi:** [`tests/c-014-checksums-and-noncryptographic-integrity.sh`](../../tests/c-014-checksums-and-noncryptographic-integrity.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat laskea yksinkertaisen allekirjoittamattoman tavujen summan (checksum) eksplisiittisen puskurin yli ja selittää, miksi se ei ole kryptografinen tiiviste tai turvallisuusmekanismi.

## Kertaus

Vastaa ennen aiempien oppituntien uudelleenavaamista.

1. Miksi tavupuskurilla pitää olla eksplisiittinen `size_t` pituus?
2. Ilmoita turvallinen eteenpäinvienti-silmukan ehto `count` kelvolliselle alkiomäärälle.
3. Mikä ehto täytyy pitää paikkansa siirto-operaation siirtomäärälle tarkassa 32-bittisessä sanassa?
4. Käännä Russian 007:sta **Я не читаю в библиотеке.**

## Keskeinen käsite

Allekirjoittamaton yhteenlasku tuottaa deterministisen modulo-tuloksen, joka voi havaita joitain tahattomia muutoksia. Yksinkertaisen summan voi helposti väärentää, joten sitä ei koskaan pidä käyttää todentamiseen, salaamiseen, turvalliseen hajautukseen tai todellisten tietojen suojaamiseen.

Ohjelma käyttää siirrettävää ISO C17 -koodia, tarkistaa paluuarvot jokaisessa ulkoisessa rajapisteessä ja pitää loogiset arvot erillään C-merkkijonoista. Se käyttää `uint8_t` ja `uint32_t` vain siellä, missä niiden täsmälleen määritelty leveys kuuluu oppitunnin sopimukseen. Se ei luo shellcodea, lataa suoritettavaa koodia, injektoi koodia muihin prosesseihin tai suorita haitallisia toimintoja.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t byte_sum32(const uint8_t bytes[], size_t count)
{
    uint32_t sum = UINT32_C(0);
    for (size_t index = 0; index < count; ++index) sum += bytes[index];
    return sum;
}

int main(void)
{
    const uint8_t vector[] = { UINT8_C(1), UINT8_C(2), UINT8_C(3), UINT8_C(4), UINT8_C(0) };
    const uint32_t result = byte_sum32(vector, sizeof vector / sizeof vector[0]);
    if (printf("Bytes: 5\nToy byte sum: 0x%08" PRIX32 "\n", result) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita mukana oleva seuralaisohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-014-checksums-and-noncryptographic-integrity \
  examples/c/c-014-checksums-and-noncryptographic-integrity.c
./c-014-checksums-and-noncryptographic-integrity
```

Odotettu tulostus:

```text
Bytes: 5
Toy byte sum: 0x0000000A
```

## Ohjattu harjoitus

1. Tunnista esimerkin eksplisiittinen syöteraja.
2. Nimeä tila- tai paluuarvo, joka tarkistetaan jokaisen ulkoisen operaation jälkeen.
3. Selitä, miksi esimerkki ei käytä sentineliä, kuten `0x00`, yleisenä taulukon rajana.
4. Ilmoita yksi virheellinen syöte, jonka seuralaisohjelma hylkää tai käsittelee turvallisesti.
5. Tuota yhdestä välittömästi edeltävästä venäjän oppitunnosta yksi tarkka kohdelause.

## Itsenäinen harjoitus

Muuta yhtä syötteen tavua ja päivitä odotettu testivektori. Kuvaile sitten kahden tavun muutos, joka säilyttää saman summan, ja osoita näin, miksi tulosta ei voi pitää kryptografisena eheystarkistuksena.

Pidä kaikki laskelmat `size_t` -muodossa, validoi kaikki muunnokset ennen kaventamista ja säilytä alkuperäinen objekti, jos varaus- tai jäsennysoperaatio epäonnistuu. Tämä on tietojenkäsittelyharjoitus, ei kryptografinen primitiivi. Sitä ei koskaan pidä esittää sopivana turvallisuuteen, salaamiseen, todentamiseen tai todellisten tietojen suojaamiseen.

## Validointi

Suorita repositorion testi:

```sh
./tests/c-014-checksums-and-noncryptographic-integrity.sh
```

Sitten, jos tuettu, rakenna AddressSanitizer- ja UndefinedBehaviorSanitizer-tarkistusten kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-014-sanitized \
  examples/c/c-014-checksums-and-noncryptographic-integrity.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää syöterajan, relevantin kokonaislukuleveyden, paluuarvojen tarkistukset, ylivuodon tai siirron suojan, deterministisen testivektorin ja sen, miksi esimerkki ei ole kryptografinen rakenne.

## Paljastus: vastaukset ja mallivastaus

1. Tavupuskuri käyttää eksplisiittistä pituutta, koska nollaarvoiset tavut voivat olla tavallista dataa.
2. Turvallinen ehto on `index < count`.
3. Siirtomäärän on oltava ei-negatiivinen ja ehdottomasti pienempi kuin vasemman operaandin promotoitu leveys.
4. **Я не читаю в библиотеке.** tarkoittaa "En lue kirjastossa."
5. Esimerkin tarkistettu raja ja deterministinen testi tekevät käyttäytymisestä toistettavaa; ne eivät kuitenkaan tee siitä kryptografista turvallisuusmekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti on **Russian 015 — kyvyn ilmaisu sanoilla могу ja можешь.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fread "fread-funktio — cppreference.com"
[4]: https://en.cppreference.com/w/c/memory/realloc "realloc-funktio — cppreference.com"
