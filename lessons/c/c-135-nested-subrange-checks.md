# C 135 — Sisäkkäiset alialueiden tarkistukset

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esitiedot:** C 001–134
**Seuraava ohjelma:** [`examples/c/c-135-nested-subrange-checks.c`](../../examples/c/c-135-nested-subrange-checks.c)
**Deterministinen testi:** [`tests/c-135-nested-subrange-checks.sh`](../../tests/c-135-nested-subrange-checks.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa osaat validoida sisemmän tavujoukon suhteessa jo validoituun ulompaan joukoon.

## Palautusharjoitus

1. Miksi raaka tavu­puskurin kanssa on oltava eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokaatio- tai jäsennys-API:en paluuarvot on tarkistettava ennen kuin niiden tuotoksiin luotetaan?
4. Perustele, miksi ei-salauksellinen tarkistus­arvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Jokainen sisäkkäinen pituus on tarkistettava vanhemman jäljellä olevien tavujen ja konfiguroidun maksimirajan suhteen ennen sen käyttämistä.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavuja laskettavana datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluu­tuloksen, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assembleykoodin suorittamista, prosessi­injektiota, itseä muuttavaa koodia tai todellisia kryptografisia toteutuksia.

## Käyty esimerkki

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
    const uint8_t bytes[] = {UINT8_C(135), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 135 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-135-nested-subrange-checks \
  examples/c/c-135-nested-subrange-checks.c
./c-135-nested-subrange-checks
```

Odotettu tulostus:

```text
C 135 reader byte: 135
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilalupaus jokaisessa apufunktion rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavanomaista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tulostus hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen kuin teet mitään muutettuja allokaatiokoko­laskelmia. Älä tee tästä opetus­tarkoituksessa olevasta esimerkistä kryptografista, suoritettavan lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-135-nested-subrange-checks.sh
```

Käännä ja aja sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-135-sanitized \
  examples/c/c-135-nested-subrange-checks.c
./c-135-sanitized
```

## Lopullinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syöterajapinnan, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin se muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten läpikäyntiä; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle radalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
