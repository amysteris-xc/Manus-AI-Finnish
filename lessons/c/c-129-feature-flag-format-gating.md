# C 129 — Feature Flag Format Gating

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–128
**Seuraava ohjelma:** [`examples/c/c-129-feature-flag-format-gating.c`](../../examples/c/c-129-feature-flag-format-gating.c)
**Deterministinen testi:** [`tests/c-129-feature-flag-format-gating.sh`](../../tests/c-129-feature-flag-format-gating.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata hyväksyä valinnainen formaatin ominaisuus vain, kun sen lippu ja pituus ovat molemmat kelvollisia.

## Muistiharjoitus

1. Miksi raaka tavutaulukko (raw byte buffer) täytyy kantaa mukana eksplisiittisellä `size_t` -pituudella?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O:n, allokaation tai jäsentämisen API:sta paluuarvot pitää tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kriptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Ominaisuuden lippu muuttaa jäsentämistä vain sen jälkeen, kun sitä ympäröivä tietue on jo riittävän kelvollinen luettavaksi turvallisesti.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, konversion ja palautusarvon, mihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessi-injektioita, itsensä muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { const uint8_t *data; size_t count; size_t position; } Reader;

static int reader_take(Reader *reader, uint8_t *out)
{
    if (reader == NULL || out == NULL || reader->data == NULL || reader->position >= reader->count) return 0;
    *out = reader->data[reader->position];
    reader->position++;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(129), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 129 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-129-feature-flag-format-gating \
  examples/c/c-129-feature-flag-format-gating.c
./c-129-feature-flag-format-gating
```

Odotettu tuloste:

```text
C 129 reader byte: 129
```

## Ohjattu harjoitus

1. Tunnista jokaisen apufunktion rajapinnalla tavujen laajuus tai tilasopimus.
2. Löydä varhaisin ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Kerro, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvarajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen tapaus ja yksi hylätty rajatapauksen tapaus. Säilytä eksplisiittiset tavumäärät, pidä alkuperäinen tuloste hylkäyksessä ennallaan, ja lisää tarkistettu aritmeettinen ehto ennen kuin muutat mitään allokointikoon laskelmaa. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritinkoodin lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-129-feature-flag-format-gating.sh
```

Käännä ja aja sanitisaattoreilla kun tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-129-sanitized \
  examples/c/c-129-feature-flag-format-gating.c
./c-129-sanitized
```

## Lopputarkistus itsearviointina

Olet valmis seuraavaan oppituntiin, jos voit määrittää syöterajapinnan, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen lukemista `data[index]`.
3. Apuri tarkistaa ennen kuin se muuttaa lähtöparametria, kursoria tai tila-kenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-kurssisarjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
