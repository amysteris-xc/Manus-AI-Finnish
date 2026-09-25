# C 195 — Continuous Integration Command Set

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edeltävät kurssit:** C 001–194
**Kumppaniohjelma:** [`examples/c/c-195-continuous-integration-command-set.c`](../../examples/c/c-195-continuous-integration-command-set.c)
**Deterministinen testi:** [`tests/c-195-continuous-integration-command-set.sh`](../../tests/c-195-continuous-integration-command-set.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi pystyä kertomaan kannettava komentosarja C17-esimerkin kääntämiseksi ja testaamiseksi.

## Hae palautusta

1. Miksi raakabufferin pitää kantaa mukana eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin liikkuvan indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:iden paluuarvot pitää tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Automaatio pitäisi ajaa samat tiukat kääntäjäflagit ja deterministiset testit, joita käytetään paikallisesti. Älä väitä, että automaatio todistaa virheettömyyden.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuina datoina, tarkastaa jokaisen osoittimen, lukeman, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodilataimia, mielivaltaista assemblyn ajamista, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(195)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 195 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-195-continuous-integration-command-set \
  examples/c/c-195-continuous-integration-command-set.c
./c-195-continuous-integration-command-set
```

Odotettu tuloste:

```text
C 195 validated byte: 195
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukeman, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä päättymismäärä.
4. Ilmoita milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavulaskut, säilytä alkuperäinen ulostulo hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen mitään muutettua allokaatio-kokolaskelmaa. Älä muuta tätä opetusmielessä olevaa esimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Varmistus

Aja deterministinen testi:

```sh
./tests/c-195-continuous-integration-command-set.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-195-sanitized \
  examples/c/c-195-continuous-integration-command-set.c
./c-195-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos voit kertoa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa defensiivisen tavukäsittelyn todellisesta maailman kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa ulostekomponenttia, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministiset testit että ilmoitettujen rajojen tarkastelun; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliselle uralle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
