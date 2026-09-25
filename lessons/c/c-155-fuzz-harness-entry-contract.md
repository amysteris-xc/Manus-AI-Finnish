# C 155 — Fuzz-harnessin sisäänpäässopimus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–154
**Seuraava ohjelma:** [`examples/c/c-155-fuzz-harness-entry-contract.c`](../../examples/c/c-155-fuzz-harness-entry-contract.c)
**Deterministinen testi:** [`tests/c-155-fuzz-harness-entry-contract.sh`](../../tests/c-155-fuzz-harness-entry-contract.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata määritellä rajattu parserin sisäänkäyntipiste, joka sopii fuzzer-tyyliselle kutsujalle.

## Muistiharjoitus

1. Miksi raakabufferin tulee sisältää eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O:n, varauksen tai jäsentämisen rajapintojen paluuarvot on tarkistettava ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Fuzz-targetin tulisi hyväksyä osoitin-plus-pituus -syöte, hylätä null-osoitin jos pituus on nollasta poikkeava, ja välttää sivuvaikutuksia kontrolloidun testidatan ulkopuolella.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja esimerkin varaan jäävän palautustuloksen, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessiin injektiota, itseä muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { const uint8_t *data; size_t count; size_t position; } Reader;

static int reader_take(Reader *reader, uint8_t *out)
{
    if (reader == NULL || out == NULL || reader->data == NULL || reader->position >= reader->count) return 0;
    *out = reader->data[reader->position];
    reader->position++;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(155), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 155 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-155-fuzz-harness-entry-contract \
  examples/c/c-155-fuzz-harness-entry-contract.c
./c-155-fuzz-harness-entry-contract
```

Odotettu tuloste:

```text
C 155 reader byte: 155
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Määrittele milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Oma harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettavan varauskoon laskentaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suorittamisen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-155-fuzz-harness-entry-contract.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-155-sanitized \
  examples/c/c-155-fuzz-harness-entry-contract.c
./c-155-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat ilmoittaa syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavarajapinta käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-ketjuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
