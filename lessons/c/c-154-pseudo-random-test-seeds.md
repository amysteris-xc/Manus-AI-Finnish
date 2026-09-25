# C 154 — Pseudo-Random Test Seeds

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–153
**Seuralaisohjelma:** [`examples/c/c-154-pseudo-random-test-seeds.c`](../../examples/c/c-154-pseudo-random-test-seeds.c)
**Deterministinen testi:** [`tests/c-154-pseudo-random-test-seeds.sh`](../../tests/c-154-pseudo-random-test-seeds.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat käyttää dokumentoitua pseudo-satunnaista siementä vaihdellaksesi ei-salaisia testitavuja toistettavasti.

## Muistiharjoitus

1. Miksi raakabitbufferilla pitää olla eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisessa buffereissa sisällä?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta on tarkistettava, ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Ennustettava generaattori on hyödyllinen vain testeissä. Älä koskaan käytä sitä avaimiin, salasanoihin, nonseille tai turvallisuuspäätöksiin.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja palautusarvon, joihin esimerkki perustuu, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataajia, mielivaltaista assemblerin suorittamista, prosessien injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(154)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 154 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-154-pseudo-random-test-seeds \
  examples/c/c-154-pseudo-random-test-seeds.c
./c-154-pseudo-random-test-seeds
```

Odotettu tuloste:

```text
C 154 validated byte: 154
```

## Ohjattu harjoitus

1. Tunnista tavujono tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Kerro, milloin lähtöarvo tai sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokaatiokoon laskemista. Älä muuta tätä opetus-esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-154-pseudo-random-test-seeds.sh
```

Käännä ja aja sanitisaattoreilla missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-154-sanitized \
  examples/c/c-154-pseudo-random-test-seeds.c
./c-154-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit ilmoittaa syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli vastaus

1. Laskettu tavu-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostusparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
