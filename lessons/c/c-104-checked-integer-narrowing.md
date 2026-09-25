# C 104 — Checked Integer Narrowing

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edellytykset:** C 001–103
**Seurantaohjelma:** [`examples/c/c-104-checked-integer-narrowing.c`](../../examples/c/c-104-checked-integer-narrowing.c)
**Deterministinen testi:** [`tests/c-104-checked-integer-narrowing.sh`](../../tests/c-104-checked-integer-narrowing.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin loppuun mennessä sinun pitäisi osata hylätä arvo ennen kuin kavennat sen pienempään etumerkittömään kokonaislukutyyppiin.

## Palautumisharjoitus

1. Miksi raaka tavutaulukko täytyy kantaa mukana eksplisiittisen `size_t` pituuden kanssa?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsennys-API:en paluuarvot täytyy tarkistaa ennen kuin niiden tuotoksiin luotetaan?
4. Perustele, miksi ei-krptografinen tarkistusarvo ei ole auktorisointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Cast voi hiljaisesti pudottaa korkeat bitit. Tarkista arvo kohdekoon maksimia vastaan ennen sen muuntamista.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavut laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, konversion ja paluuarvon, joihin esimerkki nojautuu, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessiin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(104)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 104 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-104-checked-integer-narrowing \
  examples/c/c-104-checked-integer-narrowing.c
./c-104-checked-integer-narrowing
```

Odotettu tuloste:

```text
C 104 validated byte: 104
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Mainitse milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä opitun turvarajoituksen yksi täsmällinen lause.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavujen määrät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen kuin muutat allokointikoon laskentaa. Älä tee tästä opetusmallista kryptografista, suoritettavaa lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-104-checked-integer-narrowing.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-104-sanitized \
  examples/c/c-104-checked-integer-narrowing.c
./c-104-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos pystyt ilmoittamaan syötteen sopimuksen, tunnistamaan hylätyn rajatapauksen, selittämään tulosteen ja erottamaan defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausvihjeet ja malli

1. Laskettu tavu-API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa ulostulo-parameteria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen läpikäyntiä; yksittäinen pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
