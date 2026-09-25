# C 125 — Opaque Handle Interface Boundaries

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–124
**Seurantoprogrammi:** [`examples/c/c-125-opaque-handle-interface-boundaries.c`](../../examples/c/c-125-opaque-handle-interface-boundaries.c)
**Deterministinen testi:** [`tests/c-125-opaque-handle-interface-boundaries.sh`](../../tests/c-125-opaque-handle-interface-boundaries.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin jälkeen osaat suunnitella pienen handle-tyyppisen rajapinnan, joka piilottaa muuttuvan tilan tarkastettujen funktioiden taakse.

## Muistiharjoitus

1. Miksi raakabufferin on kantava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O:n, allokaation tai parsimisen API:en paluuarvot on tarkistettava ennen niiden outputin luottamista?
4. Perustele, miksi ei-kriptografinen tarkastusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Rajapinta kaventaa sitä, mitä kutsujat voivat muuttaa suoraan, ja keskittää validoinnin nimetylle sisäänkäyntipisteille.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee bittejä lasketuissa datoissa, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, konversion ja paluuarvon, joihin esimerkki luottaa, ja säilyttää output-tilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(125)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 125 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-125-opaque-handle-interface-boundaries \
  examples/c/c-125-opaque-handle-interface-boundaries.c
./c-125-opaque-handle-interface-boundaries
```

Odotettu tulostus:

```text
C 125 validated byte: 125
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus jokaisessa apurajapinnassa.
2. Löydä varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Kerro, milloin output-arvo tai -positio voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen output hylkäyksellä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muutetun allokaatiokoon laskemista. Älä muunna tätä opetusmaista esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusta mahdollistavaksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-125-opaque-handle-interface-boundaries.sh
```

Käännä ja suorita sanitisaattoreilla, joissa tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-125-sanitized \
  examples/c/c-125-opaque-handle-interface-boundaries.c
./c-125-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää outputin ja erottaa defensiivisen tavukäsittelyn oikean maailman kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavupohjainen API käyttää osoitinta plus eksplisiittistä pituutta, koska sisään upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio tarkistaa ennen kuin muuttaa output-parametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; yksikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
