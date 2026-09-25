# C 105 — Etumerkistä ilmanmerkkiin konversion rajatapaukset

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–104
**Seuraava ohjelma:** [`examples/c/c-105-signed-to-unsigned-conversions.c`](../../examples/c/c-105-signed-to-unsigned-conversions.c)
**Deterministinen testi:** [`tests/c-105-signed-to-unsigned-conversions.sh`](../../tests/c-105-signed-to-unsigned-conversions.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun pitäisi osata validoida allekirjoitettu arvo ennen sen käyttämistä ilmanmerkkinä laskurina tai siirtymänä.

## Hakukertausta

1. Miksi raaka tavutaulu pitää kantaa mukana eksplisiittisellä `size_t` pituudella?
2. Mikä silmukkuehto pitää eteenpäin etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä palautusarvot I/O-, allokointi- tai jäsentämisrajapinnoista on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Negatiiviset arvot on hylättävä ennen muunnosta. Myöhempi rajarajatutarkistus ei voi korjata turvatonta aiempaa muunnosta.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee tavujoukkoa lasketuksi dataksi, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja palautustuloksen, joihin esimerkki perustuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin ajoa, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(105)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 105 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-105-signed-to-unsigned-conversions \
  examples/c/c-105-signed-to-unsigned-conversions.c
./c-105-signed-to-unsigned-conversions
```

Odotettu tuloste:

```text
C 105 validated byte: 105
```

## Ohjattu harjoitus

1. Tunnista tavuväli tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoitteen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokointikoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin ajomekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-105-signed-to-unsigned-conversions.sh
```

Käännä ja suorita sanitisaattoreilla, siellä missä ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-105-sanitized \
  examples/c/c-105-signed-to-unsigned-conversions.c
./c-105-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit ilmaista syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavurajapinnan syöttö on osoitin plus eksplisiittinen pituus, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
