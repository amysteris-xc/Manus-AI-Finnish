# C 071 — Binary Writer API with a Cursor

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–070
**Seuraava ohjelma (companion program):** [`examples/c/c-071-binary-writer-api-with-cursor.c`](../../examples/c/c-071-binary-writer-api-with-cursor.c)
**Deterministinen testi:** [`tests/c-071-binary-writer-api-with-cursor.sh`](../../tests/c-071-binary-writer-api-with-cursor.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin jälkeen sinun pitäisi pystyä rakentamaan pieni kirjoittaja-API, joka etenee rajatulla ulostulo­osoittimella (cursor) vain onnistuneen kirjoituksen jälkeen.

## Muistiharjoitus

1. Miksi tavutaulukon (byte buffer) mukana täytyy kulkea eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen kuin sen tulosta käytetään.
4. Muista yksi täsmällinen lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Kirjoittaja omistaa ulostulokapasiteetin ja sen on hylättävä kirjoitukset, jotka ylittäisivät sen.

Seuraava ohjelma käsittelee raakoja tavuja lasketun datan tavoin. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan siirtämistä. Se on siirrettävä ISO C17 -ohjelma ja jättää tahallaan pois shellcode-, natiivikoodin lataajat-, mielivaltaisen assembly-suorituksen, prosessin injektoinnin, itseään muuttavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int read_u16_be(const uint8_t *data, size_t len, uint16_t *out)
{
    if (data == NULL || out == NULL || len < 2) return 0;
    *out = (uint16_t)(((uint16_t)data[0] << 8) | data[1]);
    return 1;
}

int main(void)
{
    const uint8_t field[] = {UINT8_C(0), UINT8_C(71)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 071 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-071-binary-writer-api-with-cursor \
  examples/c/c-071-binary-writer-api-with-cursor.c
./c-071-binary-writer-api-with-cursor
```

Odotettu tulostus:

```text
C 071 decoded field: 71
```

## Ohjattu harjoittelu

1. Tunnista jokainen pointer-plus-length- tai buffer-plus-capacity-sopimus ohjelmassa.
2. Löydä aikaisin hylätty väärin muotoiltu syöte.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin lopetuksena.
4. Ilmoita mikä ulostulo kirjoitetaan vasta validoinnin onnistumisen jälkeen.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi lisätty deterministinen kelvollinen syöte ja yksi hylätty reunatapaus. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja ulostulojen paluuarvojen tarkistukset. Jos muutat mitään varaus­laskentaa, lisää laskentaa edeltävä ylivuodon tarkistus.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-071-binary-writer-api-with-cursor.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-071-sanitized \
  examples/c/c-071-binary-writer-api-with-cursor.c
./c-071-sanitized
```

## Viimeinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää ulostulon ja erottaa opetuksellisen eheys- tai leikkihajautusesimerkin tuotantokäyttöön soveltuvasta todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli­vastaus

1. Raakatavut voivat sisältää `0x00`, joten sentineli-skannaus ei ole kelvollinen pituuden laskeminen.
2. Eteenpäin kulkeva läpikäyntiehto on `index < count`.
3. I/O-, varaus-, jäsennys- ja muotoilu­tulokset on tarkistettava ennen kuin niiden tuotoksia käytetään.
4. Validoitu ohjelma muuttaa ulostulotilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraavaksi ajoitettu oppitunti palaa venäjänkieliselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen sisääntulo/ulosanto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen rakennusketjun koventamisen tiivistelmä — OWASP"
