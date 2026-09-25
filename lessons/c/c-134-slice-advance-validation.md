# C 134 — Slice Advance Validation

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–133
**Seurantaohjelma:** [`examples/c/c-134-slice-advance-validation.c`](../../examples/c/c-134-slice-advance-validation.c)
**Deterministinen testi:** [`tests/c-134-slice-advance-validation.sh`](../../tests/c-134-slice-advance-validation.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata edistää tavutaulukkoa (byte slice) vain silloin, kun pyydetty siirtymä on sen jäljellä olevan pituuden sisällä.

## Muistiin palautusharjoitus

1. Miksi raakabitin puskuriin on liitettävä eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokaatio- tai jäsentämisrajapintojen palautusarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Perusajatus

Alitaulukot (sub-slices) vaativat tarkistetun siirtymän ja tuottavat eksplisiittisesti lyhyemmän pituuden. Älä koskaan muodosta ensin alueen ulkopuolella olevaa osoitinta.

Tämä oppitunti käyttää kannatettavaa ISO C17 -koodia. Se käsittelee tavuja laskettaavana datana, tarkistaa kaikki osoittimet, määrät, kapasiteetit, konversiot ja palautusarvot, joihin esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista käskykannan suoritusta, prosessi-injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(134), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 134 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-134-slice-advance-validation \
  examples/c/c-134-slice-advance-validation.c
./c-134-slice-advance-validation
```

Odotettu tuloste:

```text
C 134 reader byte: 134
```

## Ohjattu harjoittelu

1. Tunnista tavuala (byte span) tai tilasopimus jokaisessa apurajapinnan kohdassa.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asento saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen kuin teet minkäänlaista muutettua allokaatiokoon laskentaa. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritinlataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-134-slice-advance-validation.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-134-sanitized \
  examples/c/c-134-slice-advance-validation.c
./c-134-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavurajapinnan tapa vaatii osoittimen ja eksplisiittisen pituuden, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio tarkistaa ennen kuin muuttaa lähtöparametria, kursoria tai tila-aluetta.
4. Oikeellisuus edellyttää sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
