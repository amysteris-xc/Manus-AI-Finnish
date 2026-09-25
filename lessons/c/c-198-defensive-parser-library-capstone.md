# C 198 — Puolustava jäsentäjäkirjasto capstone

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–197
**Seuraava ohjelma:** [`examples/c/c-198-defensive-parser-library-capstone.c`](../../examples/c/c-198-defensive-parser-library-capstone.c)
**Deterministinen testi:** [`tests/c-198-defensive-parser-library-capstone.sh`](../../tests/c-198-defensive-parser-library-capstone.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän opetuksen lopussa osaat yhdistää eksplisiittiset pituudet, kursori­tarkistukset, tilaarvot ja konfiguroidut rajoitukset tiiviissä jäsentäjä-capstone-esimerkissä.

## Muistia harjoittelu

1. Miksi raaka tavutaulukko (byte buffer) pitää kantaa mukana eksplisiittisenä `size_t` pituutena?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä palautusarvot I/O-, allokointi- tai jäsentämis-API:sta täytyy tarkistaa ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastus­arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Capstone hyväksyy ainoastaan lasketut tavut, hylkää virheelliset syötteet ennen tilan muutoksia, eikä sisällä shellcodea, lataimia, injektiota, itse-muokkausta tai todellista kryptografiaa.

Tässä opetusohjelmassa käytetään kannateltavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, konversion ja palautetun tuloksen, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessin injektiota, itse-muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(198), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 198 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-198-defensive-parser-library-capstone \
  examples/c/c-198-defensive-parser-library-capstone.c
./c-198-defensive-parser-library-capstone
```

Odotettu tuloste:

```text
C 198 reader byte: 198
```

## Ohjattu harjoitus

1. Tunnista tavujen alue (byte span) tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määritä milloin lähtöarvo tai -asema saa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua allokaatio\-koon laskentaa. Älä tee tästä opetus­esimerkistä kryptografista, suoritinlataaja- tai mielivaltaisen koodin suoritustyökalua.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-198-defensive-parser-library-capstone.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-198-sanitized \
  examples/c/c-198-defensive-parser-library-capstone.c
./c-198-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt kertomaan syötesopimuksen, tunnistamaan hylätyn rajatapauksen, selittämään lähtötuloksen ja erottamaan puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametrin, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministiset testit että ilmoitettujen rajojen läpikäynnin; yksittäinen pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa Russian-ketjuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
