# C 193 — Julkaisun valmiuslista

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–192
**Seurantaohjelma:** [`examples/c/c-193-release-readiness-checklist.c`](../../examples/c/c-193-release-readiness-checklist.c)
**Deterministinen testi:** [`tests/c-193-release-readiness-checklist.sh`](../../tests/c-193-release-readiness-checklist.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat soveltaa tiivistä julkaisun tarkistuslistaa pieneen tavun käsittelykomponenttiin.

## Muistin virkistys

1. Miksi raaka tavupuskurilla täytyy olla selkeä `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, varaus- tai jäsentämisrajapinnoista pitää tarkistaa ennen kuin niiden lähtöihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Julkaisun valmius kattaa puhtaat tiukat käännökset, deterministiset testit, äärirajatestit, sanitisaattorit kun saatavilla, dokumentaation ja katselmoinnin.

Tässä oppitunnissa käytetään kannettavaa ISO C17:ää. Se käsittelee tavut lasketuilla tiedoilla, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palausarvon, joihin esimerkki luottaa, ja säilyttää lähtötilan, kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessin injektiota, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one(const uint8_t *data, size_t count, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (count != 1 || data[0] == 0) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(193)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 193 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-193-release-readiness-checklist \
  examples/c/c-193-release-readiness-checklist.c
./c-193-release-readiness-checklist
```

Odotettu tuloste:

```text
C 193 validated byte: 193
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty äärirajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskemista. Älä muunna tätä opetus-esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-193-release-readiness-checklist.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-193-sanitized \
  examples/c/c-193-release-readiness-checklist.c
./c-193-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tulosopimuksen, tunnistaa hylätyn äärirajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministiset testit että ilmoitettujen rajoitusten katselmoinnin; yksittäinen pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
