# C 130 — Taulukkoajoiset testitapaukset

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–129
**Seurantona ohjelma:** [`examples/c/c-130-table-driven-test-cases.c`](../../examples/c/c-130-table-driven-test-cases.c)
**Deterministinen testi:** [`tests/c-130-table-driven-test-cases.sh`](../../tests/c-130-table-driven-test-cases.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa osaat ajaa pienen staattisen taulukon kelvollisista ja virheellisistä tavutapauksista yhden deterministisen apufunktion läpi.

## Kertauskysymykset

1. Miksi raakabufferin tulee kantaa nimenomainen `size_t` pituus?
2. Mikä silmukkuehto pitää eteenpäin kasvavan indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varauksen- tai jäsentämis-API:en paluuarvot on tarkistettava ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Ydinajatus

Taulukkoajoinen testi tekee rajatapaukset näkyviksi ja toistettaviksi. Jokaisen rivin tulisi ilmoittaa odotettu lopputulos.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavuja laskettuna datana, tarkistaa kaikki osoittimet, lukumäärät, kapasiteetit, muunnokset ja paluuarvot, joihin esimerkin toiminta perustuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessiin injisointia, itsensä muokkaavaa koodia eikä todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(130)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 130 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-130-table-driven-test-cases \
  examples/c/c-130-table-driven-test-cases.c
./c-130-table-driven-test-cases
```

Odotettu tuloste:

```text
C 130 validated byte: 130
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä lopetusmerkki.
4. Ilmoita milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoite yhdellä tarkalla lauseella: opetusesimerkit ja formaattitarkistukset eivät ole tuotantokryptografiaa; todellista dataa varten tarvitaan vakiintuneita kirjastoja ja dokumentoituja korkeantason API:ita.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö kun hylkäys tapahtuu, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun varauksen koon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-130-table-driven-test-cases.sh
```

Käännä ja aja sanitisaattoreiden kanssa, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-130-sanitized \
  examples/c/c-130-table-driven-test-cases.c
./c-130-sanitized
```

## Loppusavotta

Olet valmis seuraavaan oppituntiin, jos pystyt ilmoittamaan syötteen sopimuksen, tunnistamaan hylätyn rajatapauksen, selittämään lähtötuloksen ja erottamaan defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että määriteltyjen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Venäjänkieliselle radalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
