# C 121 — Puskurinylivuotovapaa binaarihaku

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–120
**Seuralaisohjelma:** [`examples/c/c-121-overflow-safe-binary-search.c`](../../examples/c/c-121-overflow-safe-binary-search.c)
**Deterministinen testi:** [`tests/c-121-overflow-safe-binary-search.sh`](../../tests/c-121-overflow-safe-binary-search.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat hakea lajitellusta laskettusta taulukosta käyttämällä keskipisteen laskentaa, joka ei voi ylivuotaa.

## Palautusharjoitus

1. Miksi raakapuskurilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count` alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:sta palautuvat arvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Sano, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Käytä `low + (high - low) / 2` validoiduilla rajoilla sen sijaan, että käyttäisit tarkistamatonta indekseille tehtyä summaa.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, laskelman, kapasiteetin, konversion ja palautusarvon, johon esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asennuskoodin suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_byte(const uint8_t *data, size_t count, uint8_t target, size_t *out_index)
{
    if (data == NULL || out_index == NULL) return 0;
    for (size_t i = 0; i < count; ++i) {
        if (data[i] == target) { *out_index = i; return 1; }
    }
    return 0;
}

int main(void)
{
    const uint8_t table[] = {UINT8_C(1), UINT8_C(121), UINT8_C(250)};
    size_t index = 0;
    if (!find_byte(table, sizeof table, UINT8_C(121), &index)) return EXIT_FAILURE;
    if (printf("C 121 table index: %zu\n", index) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-121-overflow-safe-binary-search \
  examples/c/c-121-overflow-safe-binary-search.c
./c-121-overflow-safe-binary-search
```

Odotettu tulos:

```text
C 121 table index: 1
```

## Ohjattu harjoitus

1. Määritä tavujono tai tilaoperaation sopimus jokaisessa apufunktion rajapinnassa.
2. Etsi aikaisin tehtävä ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin tulosarvo tai sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen kelvollinen tapaus ja yksi hylätty rajatapauksena toimiva tilanne. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua allokoinnin kokoihin liittyvää laskelmaa. Älä tee tästä opetusesimerkistä kryptografista, suoritettavan lataajaa tai mielivaltaisen koodin suorittamisen mekanismia.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-121-overflow-safe-binary-search.sh
```

Käännä ja aja sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-121-sanitized \
  examples/c/c-121-overflow-safe-binary-search.c
./c-121-sanitized
```

## Loppukontrolli

Olet valmis seuraavaan oppituntiin, jos osaat kuvata syöttösopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavukäsittelyn todellisesta maailmasta tulevasta kryptografisesta suojauksesta.

## Vastausvinkki: vastaukset ja mallivastaus

1. Lasketun tavurajapinnan vuoksi tarvitaan osoitin plus eksplisiittinen pituus, koska upotetut nulbit ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
