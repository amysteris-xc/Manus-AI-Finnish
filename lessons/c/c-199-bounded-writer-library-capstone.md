# C 199 — Bounded Writer Library Capstone

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–198
**Seurantaohjelma:** [`examples/c/c-199-bounded-writer-library-capstone.c`](../../examples/c/c-199-bounded-writer-library-capstone.c)
**Deterministinen testi:** [`tests/c-199-bounded-writer-library-capstone.sh`](../../tests/c-199-bounded-writer-library-capstone.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat yhdistää eksplisiittisen kapasiteetin, kursorin etenemisen ja kiinteän tavujärjestyksen tiiviiksi kirjoittajaksi (writer) -capstoneksi.

## Palautusharjoitus

1. Miksi raakabatustaulukon täytyy kantaa eksplisiittistä `size_t` pituutta?
2. Mikä silmukkalauseke pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, varaus- tai jäsennys-API:sta on tarkistettava ennen tulosten luottamista?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Kirjoittaja muuttaa sijaintiaan vain onnistuneen rajatun kirjoituksen jälkeen. Sen tuotos on opetusformaatti, ei salaus, autentikointi tai suoritettava data.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavujoukkoa laskettuna datana, tarkistaa kaikki osoittimet, määrät, kapasiteetit, muunnokset ja palautusarvot, joihin esimerkki nojautuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivia lataajaa, mielivaltaista assempelisuoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t *data; size_t capacity; size_t position; } Writer;

static int writer_put(Writer *writer, uint8_t value)
{
    if (writer == NULL || writer->data == NULL || writer->position >= writer->capacity) return 0;
    writer->data[writer->position] = value;
    writer->position++;
    return 1;
}

int main(void)
{
    uint8_t bytes[2] = {0};
    Writer writer = {bytes, sizeof bytes, 0};
    if (!writer_put(&writer, UINT8_C(199))) return EXIT_FAILURE;
    if (printf("C 199 writer byte: %u\n", (unsigned)bytes[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-199-bounded-writer-library-capstone \
  examples/c/c-199-bounded-writer-library-capstone.c
./c-199-bounded-writer-library-capstone
```

Odotettu tuloste:

```text
C 199 writer byte: 199
```

## Ohjattu harjoitus

1. Määritä tavujono tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro, milloin lähtöarvo tai sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty reunaehtotapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu aritmetiikkaehto ennen mitään muuttuvaa allokaatiokoon laskentaa. Älä muuta tätä opetusmallia kryptografiseksi, suoritettavien lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-199-bounded-writer-library-capstone.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-199-sanitized \
  examples/c/c-199-bounded-writer-library-capstone.c
./c-199-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit kertoa syötteen sopimuksen, tunnistaa hylätyn reunaehtotapauksen, selittää lähtödatan ja erottaa defensiivisen tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavujen API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa lähtöparametria, kursoria tai tila-kenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
