# C 093 — Palautuva jäsentäjän tilakone

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–092
**Seuralaisohjelma:** [`examples/c/c-093-recoverable-parser-state-machine.c`](../../examples/c/c-093-recoverable-parser-state-machine.c)
**Deterministinen testi:** [`tests/c-093-recoverable-parser-state-machine.sh`](../../tests/c-093-recoverable-parser-state-machine.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin päätteeksi osaat palauttaa rajoitetun jäsentäjän tilan tunnettuun lähtöpisteeseen väärin muotoillun tietueen jälkeen.

## Muistiharjoitus

1. Miksi tavupuskurin on kannettava nimenomainen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Palautuksen on hylättävä virheellinen osa-tila sen sijaan, että sitä käsiteltäisiin kelvollisena hyötykuormana.

Seuralaisohjelma käsittelee raakatavut laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan etenemistä. Se on kannettava ISO C17 -ohjelma ja se jättää tahallisesti pois shellcode:n, natiivikoodin lataajat, mielivaltaisen assembler-koodin suorituksen, prosessin injektoinnin, itseään muuttavan koodin ja todellisen kryptografisen toteutuksen.

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
    if (printf("C 093 accepted sequence through: %u\n", (unsigned)(state.expected - 1)) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-093-recoverable-parser-state-machine \
  examples/c/c-093-recoverable-parser-state-machine.c
./c-093-recoverable-parser-state-machine
```

Odotettu tulostus:

```text
C 093 accepted sequence through: 3
```

## Ohjattu harjoitus

1. Määritä ohjelmassa kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteetti-sopimukset.
2. Etsi varhaisin hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuna.
4. Ilmoita, mikä tuloste kirjoitetaan vasta sen jälkeen, kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aiheena olisi hajautus tai eheyden tarkistus.

## Itsenäinen harjoitus

Lisää yksi lisädeterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä nykyiset argumenttien tarkistukset, nimenomaiset pituudet ja tulostuksen paluuarvojen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskelmaa.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-093-recoverable-parser-state-machine.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-093-sanitized \
  examples/c/c-093-recoverable-parser-state-machine.c
./c-093-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetus- tai leikkimieliset hajautusesimerkit oikeasta tuotantokelpoisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja malli

1. Raakatavut saattavat sisältää `0x00`, joten sentinellikartoitus ei ole pätevä pituuslaskenta.
2. Eteenpäin kulun ehto on `index < count`.
3. I/O:n, allokoinnin, jäsennyksen ja muotoilun tulokset on tarkistettava ennen niiden tuottamien arvojen käyttöä.
4. Tarkistettu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulostulo — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikatarkistuslista — OWASP"
