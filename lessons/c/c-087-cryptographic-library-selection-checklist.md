# C 087 — Kryptografisen kirjaston valintatarkistuslista

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edellytykset:** C 001–086
**Seurantaohjelma:** [`examples/c/c-087-cryptographic-library-selection-checklist.c`](../../examples/c/c-087-cryptographic-library-selection-checklist.c)
**Deterministinen testi:** [`tests/c-087-cryptographic-library-selection-checklist.sh`](../../tests/c-087-cryptographic-library-selection-checklist.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi pystyä koodaamaan ei-salainen käytäntötarkistuslista ylläpidettyjen kryptografisten kirjastojen valintaan.

## Muistiharjoitus

1. Miksi tavupuskurin tulee kantaa eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count` alkion puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen kuin sen tulosta käytetään.
4. Muista yksi täsmällinen lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen ajatus

Valitse ylläpidetyt, dokumentoidut ja arvioidut kirjastot ja käytä niiden korkeantason API:ita; älä kehitä omaa kryptografiaa.

Seurantaohjelma käsittelee raakatavut laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan edistämistä. Se on siirrettävä, ISO C17 -yhteensopiva ja jättää tahallisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembler-koodin suorituksen, prosessin injektoinnin, itseään muokkaavan koodin ja varsinaisen kryptografisen toteutuksen.

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
    const uint8_t input[] = {UINT8_C(87)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 087 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-087-cryptographic-library-selection-checklist \
  examples/c/c-087-cryptographic-library-selection-checklist.c
./c-087-cryptographic-library-selection-checklist
```

Odotettu tuloste:

```text
C 087 status-checked byte: 87
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki pointer-plus-length- eli puskurin ja kapasiteetin sopimukset.
2. Etsi aikaisin tapahtuva hylätty virheellinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin loppuun merkkinä.
4. Mainitse, mikä tuloste kirjoitetaan vasta varmennuksen onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aiheena on hajautus tai eheyden tarkastus — erottele opetusesimerkit ja oikea maailmankäyttö.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty rajatapaus. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteiden paluuarvojen tarkistukset. Jos muutat mitään allokointilaskelmia, lisää tarkistettu ylivuotoehto ennen laskua.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-087-cryptographic-library-selection-checklist.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-087-sanitized \
  examples/c/c-087-cryptographic-library-selection-checklist.c
./c-087-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetukselliset eheys- tai leikkimieliset hajautusesimerkit oikeaan maailmaan soveltuvasta kryptografisesta suojauksesta.

## Vastausvinkit ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinel-haku ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O-, allokaatio-, jäsentämis- ja muotoilutulokset on tarkistettava ennen kuin niiden tuottamia arvoja käytetään.
4. Vahvistettu ohjelma muuttaa tulostilaa vasta kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulostulo — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
