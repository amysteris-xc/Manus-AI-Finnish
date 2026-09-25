# C 164 — Lähdekoodin tarkastuksen esiehtoja

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–163
**Seurantoprogrammi:** [`examples/c/c-164-source-review-preconditions.c`](../../examples/c/c-164-source-review-preconditions.c)
**Deterministinen testi:** [`tests/c-164-source-review-preconditions.sh`](../../tests/c-164-source-review-preconditions.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin päätteeksi osaat tarkastaa apufunktion, joka asettaa eksplisiittiset esiehdot osoittimelle, pituudelle, kapasiteetille ja muunnoksille.

## Haku- ja muistiharjoitus

1. Miksi raakabufferrin täytyy kantaa eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai parsintakirjastoista on tarkistettava ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Ydinajatus

Esiehtojen tulee näkyä funktion alussa ja niitä tulee testata. Kommentti ei voi korvata pakollista tarkistusta.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavut laskettuina datoina, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki nojaa, ja säilyttää ulostulon tilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessiin injektointia, itseään muuttavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(164)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 164 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-164-source-review-preconditions \
  examples/c/c-164-source-review-preconditions.c
./c-164-source-review-preconditions
```

Odotettu tuloste:

```text
C 164 validated byte: 164
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apuajan rajalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin ulostuloarvo tai -asema saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavulaskelmat, säilytä alkuperäinen ulostulo hylkäyksessä, ja lisää tarkistettu aritmetiikkaehto ennen minkään muuttuvan allokoinnin kokolaskennan tekemistä. Älä tee tästä opetusmallista kryptografista, suoritinkoodin lataajaan tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-164-source-review-preconditions.sh
```

Käännä ja suorita sanitisaattoreilla, joissa tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-164-sanitized \
  examples/c/c-164-source-review-preconditions.c
./c-164-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit kertoa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavun rajapinta käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-kielelliseen linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
