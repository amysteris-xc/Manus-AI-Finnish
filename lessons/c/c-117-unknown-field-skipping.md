# C 117 — Unknown Field Skipping

**Arvioitu oppimisaika:** 25–35 minuuttia
**Esitiedot:** C 001–116
**Seuraava ohjelma:** [`examples/c/c-117-unknown-field-skipping.c`](../../examples/c/c-117-unknown-field-skipping.c)
**Deterministinen testi:** [`tests/c-117-unknown-field-skipping.sh`](../../tests/c-117-unknown-field-skipping.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat ohittaa pituudella rajatun tuntemattoman kentän vain sen ilmoitetun koon validoimisen jälkeen.

## Palautusharjoitus

1. Miksi raakabuffterin tulee kantaa ekspressiivistä `size_t` pituutta?
2. Mikä silmukkaehto pitää eteenpäin kasvavan indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:sta palautuvat arvot on tarkistettava ennen kuin niiden tuotoksiin luotetaan?
4. Selitä, miksi ei-kryptografista tarkistusarvoa ei pidä pitää todennuksena aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Laajennettavuus on turvallista vain, kun tuntemattoman kentän pituus tarkistetaan jäljellä olevien tavujen ja konfiguroitujen rajojen suhteen.

Tämä oppitunti käyttää kannatettavaa ISO C17 -kieltä. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja esimerkin riippuvuuksien palautusarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessi-injektiota, itseä muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(117), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 117 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-117-unknown-field-skipping \
  examples/c/c-117-unknown-field-skipping.c
./c-117-unknown-field-skipping
```

Odotettu tuloste:

```text
C 117 reader byte: 117
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus kunkin apurin rajalla.
2. Löydä aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Mainitse, milloin lähtöarvo tai -asema saa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaation kokolaskentaa. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritustason lataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-117-unknown-field-skipping.sh
```

Käännä ja aja sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-117-sanitized \
  examples/c/c-117-unknown-field-skipping.c
./c-117-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta ja ekspressiivistä pituutta, koska upotetut nolla‑tavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
