# C 059 — Tarkistettu sekvenssinumeron seuranta

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–058
**Seurantaohjelma:** [`examples/c/c-059-checked-sequence-number-tracking.c`](../../examples/c/c-059-checked-sequence-number-tracking.c)
**Deterministinen testi:** [`tests/c-059-checked-sequence-number-tracking.sh`](../../tests/c-059-checked-sequence-number-tracking.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat seurata rajattua monotonisesti kasvavaa pakettisekvenssiä ja hylätä odottamattoman syötteen.

## Muistin virkistys

1. Miksi tavutaulukon tulee sisältää eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka täytyy tarkistaa ennen sen tuloksen käyttämistä.
4. Muista yksi tarkka lause edelliseltä Russian-opetustunnilta.

## Keskeinen käsite

Sekvenssiseurain erottaa hyväksytyn tilan hylätystä pakettisyötteestä.

Seurantaohjelma käsittelee raakabittejä laskettuna datana. Se tarkastaa argumentit, kapasiteetit ja paluuarvot ennen tilan edistämistä. Se on kannettava ISO C17 -ohjelma ja poissulkee tarkoituksellisesti shellcoden, natiivikoodilataimet, mielivaltaisen assemblerin suorittamisen, prosessin injektoinnin, itseään muuttavan koodin ja todellisen kryptografian toteutuksen.

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
    if (printf("C 059 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-059-checked-sequence-number-tracking \
  examples/c/c-059-checked-sequence-number-tracking.c
./c-059-checked-sequence-number-tracking
```

Odotettu tulostus:

```text
C 059 accepted sequence through: 3
```

## Ohjattu harjoittelu

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskur-plus-kapasiteetti -sopimukset.
2. Löydä aikaisin esiintyvä hylätty virheellistä syötettä ilmaiseva ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabittipuskurin lopetuksena.
4. Ilmoita mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä — erottele opetusesimerkki ja oikea kryptografinen suojaus.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä syöte ja yksi hylätty rajatapaus. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuksen paluuarvon tarkistukset. Jos muutat mitään allokoinnin aritmetiikkaa, lisää ennen laskentaa tarkistettu ylivuotoehto.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-059-checked-sequence-number-tracking.sh
```

Käännä ja aja sanitisaattoreilla siellä missä ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-059-sanitized \
  examples/c/c-059-checked-sequence-number-tracking.c
./c-059-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos pystyt määrittelemään tavapuskuri-sopimuksen, tunnistamaan hylätyn syötteen, selittämään tulosteen ja erottamaan opetukselliset eheys- tai leikkihajautusesimerkit oikeasta tuotantokäyttöön soveltuvasta kryptografiasta.

## Vastausvinkit: vastaukset ja mallivastaus

1. Raakabittijono voi sisältää `0x00`, joten sentinelliharavaus ei ole validi pituuden laskenta.
2. Eteenpäin kulkeva ehtolauseke on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validoinnin läpäistyä ohjelma muuttaa ulostulotilaa vasta kaikkien edellytysten täyttyessä.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteäleveyskokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedoston syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaohje — OWASP"
