# C 177 — Yhdisteltävät rajatut jäsentämisvaiheet

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–176
**Seurana oleva ohjelma:** [`examples/c/c-177-composable-bounded-parser-stages.c`](../../examples/c/c-177-composable-bounded-parser-stages.c)
**Deterministinen testi:** [`tests/c-177-composable-bounded-parser-stages.sh`](../../tests/c-177-composable-bounded-parser-stages.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat koostaa kaksi rajattua jäsentämisvaihetta siten, että toinen alkaa vasta, kun ensimmäinen on onnistuneesti suoritettu.

## Muistiharjoitus

1. Miksi raaka tavutaulukko on kannettava sisällään eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin liikkuvan indeksin sisällä `count`-alkioisen puskurin rajoissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämisrajapinnoista on tarkistettava ennen kuin niiden tuottamiin tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Vaiheet ilmoittavat eksplisiittisesti jäljellä olevan pituuden ja tilan. Älä jatka väärinmuodostetun prefiksin jälkeen äläkä hiljaa tulkitse tavujonoa uudelleen.

Tässä oppitunnissa käytetään kannateltavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki nojautuu, ja säilyttää tulostilan, kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(177), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 177 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-177-composable-bounded-parser-stages \
  examples/c/c-177-composable-bounded-parser-stages.c
./c-177-composable-bounded-parser-stages
```

Odotettu tuloste:

```text
C 177 reader byte: 177
```

## Ohjattu harjoitus

1. Tunnista tavujono tai tilasopimus kunkin apufunktion rajapinnassa.
2. Löydä aikaisin hylkäävä ehto, joka torjuu virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu laskuehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä muutakaan tätä opetusesimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-177-composable-bounded-parser-stages.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-177-sanitized \
  examples/c/c-177-composable-bounded-parser-stages.c
./c-177-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit ilmoittaa syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausvinkit ja malli-vastaus

1. Lasketun tavurajapinnan syöttö on osoitin plus eksplisiittinen pituus, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotannon turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
