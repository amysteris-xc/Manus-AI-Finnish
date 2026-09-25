# C 165 — Misuse-Resistant API Design

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–164
**Seuranto-ohjelma:** [`examples/c/c-165-misuse-resistant-api-design.c`](../../examples/c/c-165-misuse-resistant-api-design.c)
**Deterministinen testi:** [`tests/c-165-misuse-resistant-api-design.sh`](../../tests/c-165-misuse-resistant-api-design.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat muotoilla tavallisen tavuihin perustuvan API:n siten, että yleiset virheelliset kutsut epäonnistuvat aikaisin ja selkeästi.

## Palauttava harjoitus

1. Miksi raakaa tavutaulukkoa pitää kantaa mukana eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisen puskurin?
3. Mitkä palautusarvot I/O-, allokaatio- tai jäsennys-API:sta pitää tarkistaa ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Ydinajatus

Hyvät rajapinnat tekevät virheellisistä yhdistelmistä vaikeita: eksplisiittiset pituudet, nimetty raja-arvo, tilaarvot ja vakaat ulostulot vähentävät epäselvyyksiä.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavua laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautustuloksen, joihin esimerkki perustuu, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataimia, mielivaltaista assemblyn suorittamista, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(165)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 165 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-165-misuse-resistant-api-design \
  examples/c/c-165-misuse-resistant-api-design.c
./c-165-misuse-resistant-api-design
```

Odotettu tuloste:

```text
C 165 validated byte: 165
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus jokaisessa apurin rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Määrittele, milloin ulostusarvo tai -asento saattaa muuttua.
5. Selitä oppitunnin esitetty turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäämisen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen kuin muutat allokaation kokoa. Älä tee tästä opetusmallista kryptografista, suoritinlataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-165-misuse-resistant-api-design.sh
```

Käännä ja suorita sanitisaattoreilla, missä ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-165-sanitized \
  examples/c/c-165-misuse-resistant-api-design.c
./c-165-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausluukut: vastaukset ja mallivastaus

1. Lasketun tavu-API:n muodostavat osoitin ja eksplisiittinen pituus, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostoparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa Russian-kielelle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
