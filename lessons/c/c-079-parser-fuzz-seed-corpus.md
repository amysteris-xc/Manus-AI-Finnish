# C 079 — Parser-fuzzin siemenkorpus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–078
**Kumppaniohjelma:** [`examples/c/c-079-parser-fuzz-seed-corpus.c`](../../examples/c/c-079-parser-fuzz-seed-corpus.c)
**Deterministinen testi:** [`tests/c-079-parser-fuzz-seed-corpus.sh`](../../tests/c-079-parser-fuzz-seed-corpus.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen sinun pitäisi pystyä ajamaan rajattua parseria pientä determinististä virheellisten siementen korpusta vastaan.

## Muistin palautusharjoitus

1. Miksi tavupuskurin on kannettava eksplisiittistä `size_t` pituutta sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka täytyy tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Siemenkorpus dokumentoi reunatapaukset ja tekee regressiotestauksesta toistettavaa.

Kumppaniohjelma käsittelee raakatavut laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan edistämistä. Se on kannettava ISO C17 -ohjelma ja jättää tahallisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assemblyn suorittamisen, prosessin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

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
    if (printf("C 079 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-079-parser-fuzz-seed-corpus \
  examples/c/c-079-parser-fuzz-seed-corpus.c
./c-079-parser-fuzz-seed-corpus
```

Odotettu tulostus:

```text
C 079 accepted sequence through: 3
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta jokainen osoitin-plus-pituus- tai puskuri-plus-kapasiteetti -sopimus.
2. Löydä varhaisin hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuna.
4. Määritä, mikä tuloste kirjoitetaan vasta, kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka sen aihe käsittelisi hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttitarkastukset, eksplisiittiset pituudet ja tulostukseen liittyvät paluuarvotarkistukset. Jos muutat minkä tahansa varauslaskennan, lisää ennen laskentaa tarkistettu ylivuotoehto.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-079-parser-fuzz-seed-corpus.sh
```

Käännä ja aja sanitisaattoreiden kanssa, kun tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-079-sanitized \
  examples/c/c-079-parser-fuzz-seed-corpus.c
./c-079-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetukselliset eheys- tai leikilliset hajautusesimerkit sopivasta todellisen maailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut saattavat sisältää `0x00`, joten sentinellihaku ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulkeva läpikäyntiehto on `index < count`.
3. I/O-, varaus-, jäsentämis- ja muotoilutoiminnoista palautuvat arvot on tarkistettava ennen niiden tulosten käyttämistä.
4. Validioitu ohjelma muuttaa tulosteen tilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
