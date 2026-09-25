# C 185 — Checked Index Offsets

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–184
**Seurantoprogrammi:** [`examples/c/c-185-checked-index-offsets.c`](../../examples/c/c-185-checked-index-offsets.c)
**Deterministinen testi:** [`tests/c-185-checked-index-offsets.sh`](../../tests/c-185-checked-index-offsets.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat validoida offsetin ja leveyden ennen tavupalan lukemista tavutiedoston kuvasta.

## Muistiharjoitus

1. Miksi raakabuffern tulee kantaa nimenomainen `size_t` pituus?
2. Mikä silmukoiden ehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O:n, varauksen tai jäsentämisen API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi kryptografisesti suojaamaton tarkastusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Offset on epäluotettava, kunnes se on tarkistettu kokonaispituutta vastaan ja tarkistettu yhteenlasku todistaa pyydetyn alueen mahtuvan.

Tämä oppitunti käyttää siirrettävää ISO C17 -koodia. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessin injektiota, itseä muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(185), UINT8_C(0)};
    Reader reader = {bytes, sizeof bytes, 0};
    uint8_t value = 0;
    if (!reader_take(&reader, &value)) return EXIT_FAILURE;
    if (printf("C 185 reader byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-185-checked-index-offsets \
  examples/c/c-185-checked-index-offsets.c
./c-185-checked-index-offsets
```

Odotettu tulostus:

```text
C 185 reader byte: 185
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai positio voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, säilytä alkuperäinen tulostus hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritettavan lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-185-checked-index-offsets.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-185-sanitized \
  examples/c/c-185-checked-index-offsets.c
./c-185-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat ilmoittaa syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli-vastaus

1. Lasketun tavun API käyttää osoitinta ja nimenomaista pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitteiden tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
