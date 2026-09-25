# C 126 — Callback Error Propagation

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–125
**Seuraava ohjelma:** [`examples/c/c-126-callback-error-propagation.c`](../../examples/c/c-126-callback-error-propagation.c)
**Deterministinen testi:** [`tests/c-126-callback-error-propagation.sh`](../../tests/c-126-callback-error-propagation.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopuksi osaat välittää callback-virheen eteenpäin rajatussa iteraatiossa siten, ettet jatka virheellisen tilan kanssa.

## Kertauskysymykset

1. Miksi raakabufferin on sisällettävä eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:den paluuarvot on tarkistettava ennen kuin niiden tuotoksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole todennus aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Callbackin tulos on osa kutsujan sopimusta. Keskeytä ensimmäiseen epäonnistumiseen, ellei API nimenomaisesti tue palautumista.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavujoukkoa laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkin riippuvuuteen perustuvan palautusarvon sekä säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assemblyn suorittamista, prosessi-injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(126)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 126 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-126-callback-error-propagation \
  examples/c/c-126-callback-error-propagation.c
./c-126-callback-error-propagation
```

Odotettu tuloste:

```text
C 126 validated byte: 126
```

## Ohjattu harjoitus

1. Tunnista tavujoukko tai tilasopimus jokaisella apufunktion rajalla.
2. Etsi aikaisin ehdot, jotka hylkäävät virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita milloin ulostusarvo tai positio voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapauksen. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokaatiokoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritettavaa latausmekanismia tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-126-callback-error-propagation.sh
```

Käännä ja suorita sanitisaattoreiden kanssa siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-126-sanitized \
  examples/c/c-126-callback-error-propagation.c
./c-126-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausvinkki ja malli-vaste

1. Laskettu tavua-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulosteen parametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
