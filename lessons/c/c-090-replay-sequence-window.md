# C 090 — Replay Sequence Window

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–089
**Companion program:** [`examples/c/c-090-replay-sequence-window.c`](../../examples/c/c-090-replay-sequence-window.c)
**Deterministic test:** [`tests/c-090-replay-sequence-window.sh`](../../tests/c-090-replay-sequence-window.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopussa sinun pitäisi osata seurata rajattua hyväksyttyjen sekvenssien ikkunaa protokollantyyppiselle opetusvirralle.

## Kertauskysymykset

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka pitää tarkistaa ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen ilmaisu edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Pelkkä sekvenssien seuranta ei yksin suojaa toistoilta; autentikoidut protokollat vaativat vakiintuneita kryptografisia mekanismeja.

Companion-ohjelma käsittelee raakatavuja pituustietoisena datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan päivittämistä. Se on portable ISO C17 -yhteensopiva ja sulkee tarkoituksellisesti pois shellkoodin, natiivikoodin lataajat, mielivaltaisen assembly-koodin suorittamisen, prosessi-injektion, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen — tämä on opetus- ja analyysiesimerkki, ei tuotantotason kryptografiaa.

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
    if (printf("C 090 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-090-replay-sequence-window \
  examples/c/c-090-replay-sequence-window.c
./c-090-replay-sequence-window
```

Odotettu tuloste:

```text
C 090 accepted sequence through: 3
```

## Ohjattu harjoittelu

1. Tunnista kaikki ohjelmassa esiintyvät osoitin-plus-pituus- tai puskuri-plus-kapasiteetti -sopimukset.
2. Etsi varhaisin ehto, jossa virheellinen syöte hylätään.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin lopuksi.
4. Ilmoita, mikä tuloste kirjoitetaan vasta kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelisi hajautusta tai eheyden varmistusta.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä nykyiset argumenttitarkistukset, eksplisiittiset pituudet ja tulos-paluuarvon tarkistukset. Jos muutat mitään varauksen aritmetiikkaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-090-replay-sequence-window.sh
```

Käännä ja suorita sanitaattoreiden kanssa, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-090-sanitized \
  examples/c/c-090-replay-sequence-window.c
./c-090-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetusluontoiset eheys- tai leikkimieliset hajautusesimerkit todellisesta, käyttökelpoisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellihakua ei voi käyttää pituuden laskemiseen.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O-, varaus-, jäsennys- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validoitu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
