# C 089 — Sisäkkäisten kehysten jäsentäjän rajoitukset

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–088
**Companion program:** [`examples/c/c-089-nested-frame-parser-limits.c`](../../examples/c/c-089-nested-frame-parser-limits.c)
**Deterministic test:** [`tests/c-089-nested-frame-parser-limits.sh`](../../tests/c-089-nested-frame-parser-limits.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat validoida pienen sisäkkäisen pituuskentän samalla kun toteutat maksimisyvyyden ja kapasiteetin rajoitukset.

## Muistiharjoitus

1. Miksi tavutaulukolla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Sisäkkäisille rakenteille on asetettava eksplisiittiset maksimisyvyys- ja kokorajoitukset resurssien loppumisen estämiseksi.

Companion-ohjelma käsittelee raakabyttejä laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan siirtämistä eteenpäin. Se on siirrettävä ISO C17 -koodiksi ja tarkoituksellisesti poissulkee shellkoodin, natiivikoodin lataajat, mielivaltaisen assemblerin suorituksen, prosessin injektion, itseään muokkaavan koodin ja todellisen salauksen toteutuksen.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t expected; } SequenceState;

static int accept_byte(SequenceState *state, uint8_t byte)
{
    if (state == NULL || byte != state->expected) return 0;
    state->expected++;
    return 1;
}

int main(void)
{
    SequenceState state = {UINT8_C(1)};
    const uint8_t stream[] = {UINT8_C(1), UINT8_C(2), UINT8_C(3)};
    for (size_t i = 0; i < sizeof stream; ++i)
        if (!accept_byte(&state, stream[i])) return EXIT_FAILURE;
    if (printf("C 089 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-089-nested-frame-parser-limits \
  examples/c/c-089-nested-frame-parser-limits.c
./c-089-nested-frame-parser-limits
```

Odotettu tuloste:

```text
C 089 accepted sequence through: 3
```

## Ohjattu harjoitus

1. Tunnista kaikki ohjelmassa esiintyvät osoitin-plus-pituus- tai puskuriplus-kapasiteetti -sopimukset.
2. Etsi aikaisin tapahtuva hylätty virheellistä syötettä koskeva ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabyt-puskurin lopuna.
4. Ilmoita, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä — erottele opetuskäyttöiset leikkiesimerkit todellisesta suojauksesta.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty reunatapauksena oleva syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuksen palautustarkistukset. Jos muutat muistiallokointien laskentaa, lisää ennen laskentaa tarkistettu ylivuototarkastus.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-089-nested-frame-parser-limits.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuki on saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-089-sanitized \
  examples/c/c-089-nested-frame-parser-limits.c
./c-089-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat selittää tavapuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää ohjelman tulostuksen ja erottaa opetusesimerkit tai leikkimieliset hajautusesimerkit oikeasta, tuotantokelpoisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakabytet saattavat sisältää `0x00`, joten päätemerkkien skannaus ei ole pätevä pituuden laskentatapa.
2. Eteenpäin kulkeva läpikäyntiehto on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden tuottamien arvojen käyttämistä.
4. Validointi läpäissyt ohjelma muuttaa ulostilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte ja tulostus — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen tiivistelmä — OWASP"
