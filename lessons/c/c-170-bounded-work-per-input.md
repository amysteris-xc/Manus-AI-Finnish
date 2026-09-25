# C 170 — Rajoitettu työmäärä per syöte

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–169
**Seuralaisohjelma:** [`examples/c/c-170-bounded-work-per-input.c`](../../examples/c/c-170-bounded-work-per-input.c)
**Deterministinen testi:** [`tests/c-170-bounded-work-per-input.sh`](../../tests/c-170-bounded-work-per-input.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa sinun pitäisi osata yhdistää syötteen pituus ja konfiguroidut rajat parsinnan työmäärän tahalliseen ylärajaan.

## Palautusharjoitus

1. Miksi raakanukkearvomuistinpuskuri (raw byte buffer) on oltava mukana selkeällä `size_t` pituudella?
2. Mikä silmukkaehto pitää eteenpäin etenevän indeksin `count` alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentely-API:sta palautuvat arvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Defensiivinen koodi huomioi ajan sekä tavut. Vältä silmukoita, joiden kokonaistyö voi kasvaa odottamattomasti sisäkkäisistä tai toistuvista syötteistä.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavuja laskettuna tietona, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja palautusarvon, johon esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaisen assemblerin suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(170)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 170 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-170-bounded-work-per-input \
  examples/c/c-170-bounded-work-per-input.c
./c-170-bounded-work-per-input
```

Odotettu tuloste:

```text
C 170 validated byte: 170
```

## Ohjattu harjoitus

1. Tunnista tavuväli tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät ennallaan, säilytä alkuperäinen lähtö hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokoinnin suuruuden laskemista. Älä muunna tätä opettavaista esimerkkiä kryptografiseksi, suoritettavaan lataajaan tai mielivaltaisen koodin ajomekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-170-bounded-work-per-input.sh
```

Käännä ja suorita sanitisaattoreiden kanssa siellä missä ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-170-sanitized \
  examples/c/c-170-bounded-work-per-input.c
./c-170-sanitized
```

## Viimeinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat kuvata syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
