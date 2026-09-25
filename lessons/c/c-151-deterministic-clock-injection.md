# C 151 — Deterministinen ajan-injektio

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–150
**Seuraava ohjelma:** [`examples/c/c-151-deterministic-clock-injection.c`](../../examples/c/c-151-deterministic-clock-injection.c)
**Deterministinen testi:** [`tests/c-151-deterministic-clock-injection.sh`](../../tests/c-151-deterministic-clock-injection.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata antaa hallittu aika-tyyppinen arvo apurille ambienssi-ajan lukemisen sijaan deterministisessä testissä.

## Palautusharjoitus

1. Miksi raakabittipuskurilla pitää olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin liikkuvan indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Riippuvuuksien injektointi tekee testeistä toistettavia. Oppitunti mallintaa numeerista arvoa, ei oikeaa ajoitus- tai järjestelmäkello-API:ta.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee bittejä luetelluina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Esimerkki vaiheittaessa

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
    const uint8_t input[] = {UINT8_C(151)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 151 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-151-deterministic-clock-injection \
  examples/c/c-151-deterministic-clock-injection.c
./c-151-deterministic-clock-injection
```

Odotettu tuloste:

```text
C 151 validated byte: 151
```

## Ohjattu harjoitus

1. Tunnista tavun väli tai tilasopimus jokaisessa apurirajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö virhetilanteessa ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä muunna tätä opetus-esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritustavaksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-151-deterministic-clock-injection.sh
```

Käännä ja aja sanitisaattoreilla, kun tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-151-sanitized \
  examples/c/c-151-deterministic-clock-injection.c
./c-151-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit ilmaista syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli-vastaus

1. Laskettu tavujen API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kävely tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
