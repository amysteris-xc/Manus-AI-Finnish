# C 077 — Tavumuodon uhkamallin muistiinpanot

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–076
**Companion program:** [`examples/c/c-077-byte-format-threat-model-notes.c`](../../examples/c/c-077-byte-format-threat-model-notes.c)
**Deterministic test:** [`tests/c-077-byte-format-threat-model-notes.sh`](../../tests/c-077-byte-format-threat-model-notes.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat kirjata syötteen rajat ja virhetilanteet tavumuodolle käyttäen eksplisiittisiä jäsentäjän tarkistuksia.

## Muistiharjoitus

1. Miksi tavupuskurin on kannettava eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Uhkamallinnus alkaa hyödykkeistä, syötteistä, rajoista ja virhekäyttäytymisestä sen sijaan, että keksittäisiin kryptografiaa.

Companion-ohjelma käsittelee raakatavut laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan edistämistä. Se on siirrettävä ISO C17 -ohjelma ja tarkoituksellisesti jättää pois shellcodea, natiivi­koodin lataajia, mielivaltaista assembly-koodin suoritusta, prosessin injektiota, itseään muokkaavaa koodia ja todellista kryptografista toteutusta.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one_byte(const uint8_t *data, size_t len, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (len != 1) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(77)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 077 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-077-byte-format-threat-model-notes \
  examples/c/c-077-byte-format-threat-model-notes.c
./c-077-byte-format-threat-model-notes
```

Odotettu tulostus:

```text
C 077 status-checked byte: 77
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteetti-sopimukset.
2. Paikanna varhaisin hylätty virheellisesti muotoillun syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` tavupuskurin lopuksi.
4. Määrittele mikä tulostus kirjoitetaan vain validoinnin onnistumisen jälkeen.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka sen aihe käsittelisi hajautuksia tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulos-paluuarvojen tarkistukset. Jos muutat mitään varauksen laskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-077-byte-format-threat-model-notes.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-077-sanitized \
  examples/c/c-077-byte-format-threat-model-notes.c
./c-077-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat ilmaista tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulostuksen ja erottaa opetukselliset eheys- tai leikkikentän hajautusesimerkit oikeasta käytännön kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellin skannaus ei ole kelvollinen pituuden laskenta.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O:n, varauksen, jäsentämisen ja muotoilun tulokset on tarkistettava ennen kuin niiden tuloksia käytetään.
4. Validioitu ohjelma muuttaa ulostulotilaa vasta kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen osioon.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
