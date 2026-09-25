# C 094 — Virheellisten syötteiden regressiokorpus

**Arvioitu opiskelu-aika:** 25–35 minuuttia
**Esitiedot:** C 001–093
**Oheisohjelma:** [`examples/c/c-094-malformed-input-regression-corpus.c`](../../examples/c/c-094-malformed-input-regression-corpus.c)
**Deterministinen testi:** [`tests/c-094-malformed-input-regression-corpus.sh`](../../tests/c-094-malformed-input-regression-corpus.sh)
**Tekijä:** Manus AI

## Mitä tulet osaamaan

Tämän oppitunnin lopuksi sinun pitäisi pystyä tallentamaan pieni joukko virheellisiä tavutapauksia deterministisiksi regressiotesteiksi.

## Muistiharjoitus

1. Miksi tavupuskurin täytyy sisältää eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto osoittaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston palautusarvo, joka täytyy tarkistaa ennen kuin tulosta käytetään.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Regressiotapaukset estävät aiemmin korjattujen rajatapavirheiden hiljaisen paluun.

Oheisohjelma käsittelee raakadataa tavuina lukumääräisenä datana. Se tarkastaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Se on siirrettävä ISO C17 -yhteensopiva ohjelma, ja se jättää tahallaan pois shellcoden, natiivikoodin lataajat, mielivaltaisen assemblyn suorittamisen, prosessiin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

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
    if (printf("C 094 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-094-malformed-input-regression-corpus \
  examples/c/c-094-malformed-input-regression-corpus.c
./c-094-malformed-input-regression-corpus
```

Odotettu tulostus:

```text
C 094 accepted sequence through: 3
```

## Ohjattu harjoitus

1. Tunnista ohjelmassa kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteetti-sopimukset.
2. Löydä aikaisin tapahtuva virheellisen syötteen hylkäysehto.
3. Selitä, miksi ohjelma ei pidä `0x00` raakatavupuskurin loppuna.
4. Ilmoita mikä tuloste kirjoitetaan vain, kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka sen aiheena olisi hajautus tai eheys.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tuloste-palautearvotarkistukset. Jos muutat muistiallokointiin liittyvää laskentaa, lisää tarkistettu ylivuototarkistus ennen laskutoimitusta.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-094-malformed-input-regression-corpus.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-094-sanitized \
  examples/c/c-094-malformed-input-regression-corpus.c
./c-094-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetustarkoituksessa esitetyt eheys- tai leikkimieliset hajautusesimerkit todellisesta käytännön kryptografisesta suojauksesta.

## Vastausvinkki: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellihaku ei ole pätevä tapa laskea pituutta.
2. Etenemisen ehto on `index < count`.
3. I/O:n, muistiallokoinnin, jäsentämisen ja muotoilun palautusarvot on tarkistettava ennen niiden käyttöä.
4. Varmennettu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraavaksi ajoitettu oppitunti palaa venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
