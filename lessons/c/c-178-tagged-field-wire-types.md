# C 178 — Tagged Field Wire Types

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–177
**Seuraava ohjelma:** [`examples/c/c-178-tagged-field-wire-types.c`](../../examples/c/c-178-tagged-field-wire-types.c)
**Deterministinen testi:** [`tests/c-178-tagged-field-wire-types.sh`](../../tests/c-178-tagged-field-wire-types.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat validoida pienen tagi- ja wire-tyypin parin ennen kentän rungon lukemista.

## Muistikoe

1. Miksi raaka tavutaulukko (byte buffer) on kannettava yhdessä selkeän `size_t` pituuden kanssa?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en palautusarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Yksi tagi ei riitä: ilmoitettu wire-tyyppi määrittää, kuinka monta tavua voidaan kuluttaa ja mitä rajoja sovelletaan.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja riippuvaisen palautustuloksen, ja säilyttää tuotetilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessien injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(178), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 178 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-178-tagged-field-wire-types \
  examples/c/c-178-tagged-field-wire-types.c
./c-178-tagged-field-wire-types
```

Odotettu tuloste:

```text
C 178 reader byte: 178
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin ulostuloarvo tai positio voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä tavujen määrät eksplisiittisinä, säilytä alkuperäinen ulostulo hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavan lataimeksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-178-tagged-field-wire-types.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-178-sanitized \
  examples/c/c-178-tagged-field-wire-types.c
./c-178-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin jos voit nimetä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavunkäsittelyn todellisesta tuotantotason kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-ketjuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
