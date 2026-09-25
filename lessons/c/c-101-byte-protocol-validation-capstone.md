# C 101 — Tavuprotokollan validoinnin loppuprojekti

**Arvioitu oppimisaika:** 25–35 minuuttia
**Edeltävät kurssit:** C 001–100
**Companion program:** [`examples/c/c-101-byte-protocol-validation-capstone.c`](../../examples/c/c-101-byte-protocol-validation-capstone.c)
**Deterministic test:** [`tests/c-101-byte-protocol-validation-capstone.sh`](../../tests/c-101-byte-protocol-validation-capstone.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat validoida tiiviin kehyksellisen viestin, joka sisältää magic-tavun, version, pituuden, rajatun hyötykuorman ja ei-kryptografisen tarkistustavun.

## Muistiharjoitus

1. Miksi tavupuskurissa täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`-merkkiin?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttöä.
4. Muista yksi täsmällinen lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Tämä loppuprojekti on pelkästään puolustuksellinen jäsentämisharjoitus; sen tarkistustavu ei korvaa todennusta tai salausta.

Kumppaniohjelma käsittelee raakabyteja laskettuna datana (pituus mukana). Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan päivittämistä. Ohjelma on siirrettävä ja ISO C17 -yhteensopiva, ja siinä on tietoisesti jätetty pois shellcode, native-code -loaderit, mielivaltainen assemblyn suoritus, prosessiin injisointi, itseään muuttava koodi sekä todellinen kryptografinen toteutus.

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
    if (printf("C 101 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-101-byte-protocol-validation-capstone \
  examples/c/c-101-byte-protocol-validation-capstone.c
./c-101-byte-protocol-validation-capstone
```

Odotettu tulostus:

```text
C 101 accepted sequence through: 3
```

## Ohjattu harjoitus

1. Tunnista jokainen pointer-plus-length- tai buffer-plus-capacity -sopimus ohjelmassa.
2. Löydä aikaisin tapahtuva hylätty muotoiltu syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` lopputunnisteena raakabytepuskuriin.
4. Ilmoita, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aiheena käsitellään hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen hyväksytty syöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja palautusten/tulostuksen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-101-byte-protocol-validation-capstone.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-101-sanitized \
  examples/c/c-101-byte-protocol-validation-capstone.c
./c-101-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää ohjelman tuottaman tulosteen ja erottaa opetusesimerkin tason eheyden tarkistuksen tai leikiksi tarkoitetun hajautuksen sekä todellisen maailman kryptografisen suojauksen välillä.

## Ratkaisut ja mallivastaus (spoileri)

1. Raakabytestit voivat sisältää `0x00`, joten päätunnisteen (sentinel) skannausta ei voi käyttää pituuden laskemiseen.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O:n, allokoinnin, jäsentämisen ja muotoilun tulokset on tarkistettava ennen kuin niiden tuloksia käytetään.
4. Validoitu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
