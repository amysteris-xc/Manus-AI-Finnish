# C 123 — Iteraatiolaskurin invariantti

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–122
**Seurantoprogrammi:** [`examples/c/c-123-iteration-count-invariants.c`](../../examples/c/c-123-iteration-count-invariants.c)
**Deterministinen testi:** [`tests/c-123-iteration-count-invariants.sh`](../../tests/c-123-iteration-count-invariants.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata määritellä ja testata rajoitetun tavutaulukon laskurin invariantti.

## Muistin virkistämistä

1. Miksi raakana annettavalla tavutaulukolla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkuehto pitää etenevän indeksin sisällä `count`-alkioisessa taulukossa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:den paluuarvot on tarkistettava ennen kuin niiden tuottamiin lähtöihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Eteenpäin etenevä silmukka on turvallinen, kun sen indeksi alkaa sallitulla alueella, etenee ennakoitavasti ja verrataan eksplisiittiseen laskuriin.

Tämä oppitunti käyttää kannettavaa ISO C17 -koodia. Se käsittelee tavuja lasketun datan tavoin, tarkastaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Läpi käyty esimerkki

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
    const uint8_t input[] = {UINT8_C(123)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 123 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-123-iteration-count-invariants \
  examples/c/c-123-iteration-count-invariants.c
./c-123-iteration-count-invariants
```

Odotettu tuloste:

```text
C 123 validated byte: 123
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin esiintyvä ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua allokaatiokoon laskentaa. Älä tee tästä opetusmuotoisesta esimerkistä kryptografista, suoritinlataajaa tai mekanismia mielivaltaisen koodin suorittamiseksi.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-123-iteration-count-invariants.sh
```

Käännä ja aja sanitisaattoreilla tuetuissa ympäristöissä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-123-sanitized \
  examples/c/c-123-iteration-count-invariants.c
./c-123-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli

1. Lasketulla tavurajapinnalla käytetään osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajastettu oppitunti palaa venäjänkieliseen linjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
