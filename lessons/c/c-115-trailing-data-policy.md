# C 115 — Trailing Data Policy

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–114
**Seurailijaohjelma:** [`examples/c/c-115-trailing-data-policy.c`](../../examples/c/c-115-trailing-data-policy.c)
**Deterministinen testi:** [`tests/c-115-trailing-data-policy.sh`](../../tests/c-115-trailing-data-policy.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin jälkeen osaat määritellä ja testata, hyväksyykö, hylkääkö tai raportoianko jäsennintä sen jälkeen tulevia tavuja täydellisen tietueen jälkeen.

## Palautusharjoitus

1. Miksi raakabyte-bufferin täytyy kantaa nimenomainen `size_t` pituus?
2. Mikä silmukkaehdon muoto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, varaus- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Jäljelle jäävät tavut ovat osa formaattipäätöstä. Tee politiikasta selkeä sen sijaan, että vahingossa sivuuttaisit odottamattoman syötteen.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa kaikki osoittimet, määrät, kapasiteetit, muunnokset ja paluuarvot, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(115), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 115 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-115-trailing-data-policy \
  examples/c/c-115-trailing-data-policy.c
./c-115-trailing-data-policy
```

Odotettu tuloste:

```text
C 115 reader byte: 115
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen kelvollinen tapaus ja yksi hylätty rajatapaus. Pidä tavumäärät eksplisiittisinä, säilytä alkuperäinen lähtö hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettavaa varauskoon laskentaa. Älä muunna tätä opettavaista esimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaista koodin suoritusta mahdollistavaksi mekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-115-trailing-data-policy.sh
```

Käännä ja aja sanitisaattoreilla, jos tuki on saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-115-sanitized \
  examples/c/c-115-trailing-data-policy.c
./c-115-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavuhandlingin todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitetta ja nimenomaista pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
