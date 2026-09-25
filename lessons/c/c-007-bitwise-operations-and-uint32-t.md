# C 007 — `uint32_t`, maskit, siirrot ja rajatut sanakentät

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** [C 001 — Kokoaminen, tyypit ja tulostus](c-001-build-types-output.md), [C 002 — Lausekkeet, valinnat, silmukat ja tarkistettu syöte](c-002-control-flow-and-simple-input.md), [C 003 — Funktiot, lähtöparametrit ja eksplisiittiset virhepaluut](c-003-functions-and-error-returns.md), [C 004 — Kiinteän kokoiset taulukot, C-merkkijonot ja indeksi­rajat](c-004-arrays-strings-and-bounds.md), [C 005 — Taulukko­parametrit, `size_t`‑lukumäärät ja rajattu läpikäynti](c-005-array-parameters-and-bounded-iteration.md), ja [C 006 — `uint8_t`, tavutaulukot ja eksplisiittiset pituudet](c-006-unsigned-integers-and-byte-arrays.md)
**Liitännäinen ohjelma:** [`examples/c/c-007-bitwise-operations-and-uint32-t.c`](../../examples/c/c-007-bitwise-operations-and-uint32-t.c)
**Deterministinen testi:** [`tests/c-007-bitwise-operations-and-uint32-t.sh`](../../tests/c-007-bitwise-operations-and-uint32-t.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Oppitunnin lopuksi osaat käyttää täsmälleen leveydeltään `uint32_t` sanaa pienten loogisten kenttien tallentamiseen, validoida jokainen kenttä ennen sen pakkaamista maskeilla ja siirroilla, purkaa kentät oikeilla siirroilla ja maskeilla, käydä turvallisesti läpi eksplisiittisen pituuden sanataulukon sekä esittää pääsäännön siirtomääristä, joka estää määrittelemättömän käyttäytymisen.

## Kertauslämmittely

Ilman aiempien oppituntien avaamista vastaa seuraaviin kysymyksiin.

1. Miksi `uint8_t`‑puskurin täytyy käyttää eksplisiittistä `size_t`‑pituutta sen sijaan, että se päättyisi arvoon `0x00`?
2. Kirjoita turvallinen silmukkaehto jokaisen taulukon alkion käsittelemiseksi, kun taulukossa on `count` kelvollista alkiota.
3. Mitkä otsikot tarjoavat `uint8_t` ja `PRIX8`?
4. Mitä C‑merkkijonon ensimmäinen `\0` tarkoittaa?
5. Venäjä 007 -harjoituksesta: sano "En opiskele koulussa."

Vastausosiossa on mallivastaukset. Yritä palauttaa muisti ennen kuin avaat aikaisemman materiaalin.

## 1. Kun `uint32_t` on saatavilla, sana on täsmälleen 32 bittiä

Kun toteutus tarjoaa `uint32_t` kirjastoissa `<stdint.h>`, se on allekirjoittamaton kokonaislukutyyppi, jolla on täsmälleen 32 bittiä eikä täyte­bittejä.[1] Tämä on hyödyllistä, kun tietomääritys määrittelee 32‑bittisen loogisen sanan. Kuten `uint8_t`, myös `uint32_t` on valinnainen ISO C:ssä, joten ohjelman, joka tarvitsee täsmälleen 32‑bittisiä sanoja, tulee varmistaa toteutuksen tarjoavan se.

```c
#include <stdint.h>

#if !defined(UINT32_MAX)
#error "This program requires an exact 32-bit uint32_t type."
#endif
```

Tässä oppitunnissa käsitellään `uint32_t`‑arvoa loogisena sanana, jolla on nimetyt kentät. Tätä sanaa ei kirjoiteta tiedostoon tai verkkoon. Tavujen järjestys muistissa, eli endianness, vaikuttaa vain, kun data ylittää tavurajan, kuten tiedostoissa, verkkoprotokollissa tai laite­rajapinnoissa. Endianness käsitellään eksplisiittisesti myöhemmässä oppitunnissa; älä oleta, että luvun siirtäminen luo kannettavissa olevaa muistibyttijonoa.

## 2. Nimeä pieni kenttäasettelu ennen bittitason koodin kirjoittamista

Liitännäisesimerkki varaa kolme kenttää jokaista 32‑bittistä sanaa kohden:

| Kenttä | Bitin sijainnit | Leveys | Sallittu arvoalue |
|---|---:|---:|---:|
| Category | 0–7 | 8 bittiä | `0`–`255` |
| Flags | 8–15 | 8 bittiä | `0`–`255` |
| Sequence | 16–31 | 16 bittiä | `0`–`65535` |

Sana, jonka category on `42`, flags `5` ja sequence `291`, koostetaan seuraavasti:

```text
sequence = 0x0123        -> 0x01230000 after << 16
flags    = 0x05          -> 0x00000500 after << 8
category = 0x2A          -> 0x0000002A
combined                    0x0123052A
```

Kenttäsuunnitelma on osa ohjelman sopimusta. Kirjoita se ylös taulukkoon tai kommenttiin ennen siirtojen käyttämistä. Muutoin on helppo peittää kenttiä päällekkäin, jättää vahingossa aukkoja tai käyttää väärää maskia.

## 3. Maskit valitsevat bittejä; siirrot siirtävät kenttiä

C tarjoaa bittitason AND‑operaation (`&`), OR (`|`), eksklusiivinen OR (`^`) ja NOT (`~`) kokonaislukuarvoille.[2] Maskilla on `1` bittejä, joissa kenttä säilyy, ja `0` bittejä, joissa muut bitit hylätään.

```c
#define CATEGORY_MASK UINT32_C(0x000000FF)
#define FLAGS_MASK    UINT32_C(0x0000FF00)
#define SEQUENCE_MASK UINT32_C(0xFFFF0000)
```

Käytä vasenta siirtoa (`<<`) sijoittaaksesi tarkistetun kentän sille varattuihin bitteihin. Käytä bittitasoista OR:ia yhdistääksesi päällekkäämättömiä kenttiä.

```c
word = category | (flags << 8) | (sequence << 16);
```

Käytä oikeaa siirtoa (`>>`) tuodaksesi kentän alas alhaisimpiin bitteihin, ja sen jälkeen käytä maskia hylätäksesi liittymättömät bitit.

```c
category = word & CATEGORY_MASK;
flags = (word >> 8) & CATEGORY_MASK;
sequence = (word >> 16) & CATEGORY_MASK;
```

Lopullinen `CATEGORY_MASK` toimii poimituissa 8‑bittisissä kentissä, koska ne on jo siirretty alas. Sekvenssikenttä vie 16 bittiä, joten tässä esimerkissä yhdistetään kaksi 8‑bittistä osaa sen purkamiseksi. Myöhempi oppitunti laajentaa maskisanastoa; tämän päivän tavoite on tehdä jokainen operaatio näkyväksi ja tarkistetuksi.

## 4. Käytä etumerkittömiä operandeja ja osoita siirtomäärien turvallisuus

Siirto ei ole "aina turvallinen koska ohjelma on käännetty". C määrittelee siirtojen käyttäytymisen vain, kun oikea operand on ei‑negatiivinen ja ehdottomasti pienempi kuin vasemman operandin promosoidun tyypin bittien määrä.[2]

Tässä täsmälleen 32‑bittisessä ohjelmassa siirrot ovat kiinteitä arvoja `0`, `8` ja `16`. Kukin on pienempi kuin `32`, ja jokaisen siirretyn arvon tyyppi on `uint32_t`. Ohjelma ei koskaan suorita siirtoa arvoilla `32` tai suuremmilla.

```c
const uint32_t packed = category | (flags << 8) | (sequence << 16);
```

`pack_metadata`‑apufunktio todistaa ensin, että category ja flags mahtuvat kahdeksaan bittiä ja että sequence mahtuu kuuteentoista bittiä. Siksi sen siirrot eivät voi vuotaa informaatiota kentästä naapurikenttään.

```c
if (category > UINT32_C(0xFF) || flags > UINT32_C(0xFF) ||
    sequence > UINT32_C(0xFFFF)) {
    return STATUS_VALUE_OUT_OF_RANGE;
}
```

Etumerkittömillä vasemmilla operandeilla määritelty vasen siirto hylkää ne bitit, jotka siirretään ulos kohdetyypistä.[2] Tämä määritelty modulo‑käyttäytyminen ei poista tarvetta huolehtia tietomuodosta. Validoi kentän arvot ennen pakkaamista, jotta tietohäviötä ei tapahdu alun perinkään. Älä siirrä allekirjoitettuja arvoja korvikkeena tälle suunnittelulle, äläkä käytä validoimattomia siirtomääriä, jotka on johdettu syötteestä.

> **Siirtosääntö:** Ennen `value << count` tai `value >> count`, määritä haluttu etumerkitön tyyppi ja todista `0 <= count < word_width`. Tässä oppitunnissa siirrot ovat kääntöaikaisia vakioita `8` ja `16` täsmällisessä 32‑bittisessä sanassa.

## 5. Erota pakkaus, purku ja raportointi

Liitännäisohjelma käyttää kolmea pientä vastuualuetta:

| Apuohjelma | Vastuu | Keskeinen ehto |
|---|---|---|
| `pack_metadata` | Varmistaa ja yhdistää kolme kenttäarvoa | Tulosteen osoitin ei ole NULL; kentät mahtuvat leveystensä sisään |
| `unpack_metadata` | Poimii kolme kenttäarvoa yhdestä sanasta | Kaikki tulosteen osoittimet eivät ole NULL |
| `write_word_report` | Käy läpi ja näyttää sanataulukon | Sanan osoitin ei ole NULL; lukumäärä on ei‑nolla; silmukka käyttää `index < count` |

Tämä erottelu antaa testeille mahdollisuuden ajaa virhepolkuja suoraan. Se myös estää raporttikoodia piilottamasta pakkaus‑ ja purkamis­sääntöjä.

Sana‑taulukko itsessään noudattaa edelleen C 005:n osoitin‑plus‑lukumäärä‑sääntöä:

```c
for (size_t index = 0; index < count; ++index) {
    status = unpack_metadata(words[index], &category, &flags, &sequence);
    /* Check status before using outputs. */
}
```

Ohjelma ei skannaa muistia etsiessään sentinel‑sanaa. Arvo `0x00000000` voisi olla kelvollinen data; eksplisiittinen `count` määrittää loogisen rajauksen.

## 6. Työstetty esimerkki: pakkaa, pura ja raportoi kaksi sanaa

Liitännäisohjelma pakkaa kaksi rajattua metatietosanaa ja raportoi niiden heksa­desimaali­esityksen sekä kenttien desimaaliset arvot. Kyseessä on datan asetteluharjoitus, EI salaus, hajautus, avainten käsittely tai todellisen datan suojaus.

```c
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(UINT32_MAX)
#error "This program requires an exact 32-bit uint32_t type."
#endif

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_VALUE_OUT_OF_RANGE = 2,
    STATUS_OUTPUT_FAILURE = 3
} Status;

enum {
    FLAGS_SHIFT = 8,
    SEQUENCE_SHIFT = 16
};

#define CATEGORY_MASK UINT32_C(0x000000FF)
#define SEQUENCE_VALUE_MASK UINT32_C(0x0000FFFF)

static Status pack_metadata(uint32_t category, uint32_t flags,
                            uint32_t sequence, uint32_t *word_out)
{
    if (word_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (category > CATEGORY_MASK || flags > CATEGORY_MASK ||
        sequence > SEQUENCE_VALUE_MASK) {
        return STATUS_VALUE_OUT_OF_RANGE;
    }

    *word_out = category | (flags << FLAGS_SHIFT) |
                (sequence << SEQUENCE_SHIFT);
    return STATUS_OK;
}

static Status unpack_metadata(uint32_t word, uint32_t *category_out,
                              uint32_t *flags_out, uint32_t *sequence_out)
{
    if (category_out == NULL || flags_out == NULL || sequence_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    *category_out = word & CATEGORY_MASK;
    *flags_out = (word >> FLAGS_SHIFT) & CATEGORY_MASK;
    *sequence_out = (word >> SEQUENCE_SHIFT) & SEQUENCE_VALUE_MASK;
    return STATUS_OK;
}

static Status write_word_report(const uint32_t words[], size_t count)
{
    if (words == NULL || count == 0) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Word count: %zu\n", count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    for (size_t index = 0; index < count; ++index) {
        uint32_t category;
        uint32_t flags;
        uint32_t sequence;
        Status status = unpack_metadata(words[index], &category, &flags,
                                        &sequence);

        if (status != STATUS_OK) {
            return status;
        }

        if (printf("Word %zu: 0x%08" PRIX32
                   " category=%" PRIu32 " flags=%" PRIu32
                   " sequence=%" PRIu32 "\n",
                   index, words[index], category, flags, sequence) < 0) {
            return STATUS_OUTPUT_FAILURE;
        }
    }

    return STATUS_OK;
}

int main(void)
{
    uint32_t words[2];
    Status status;

    status = pack_metadata(UINT32_C(42), UINT32_C(5), UINT32_C(291),
                           &words[0]);
    if (status != STATUS_OK) {
        (void)fputs("Cannot pack the first metadata word.\n", stderr);
        return EXIT_FAILURE;
    }

    status = pack_metadata(UINT32_C(16), UINT32_C(160), UINT32_C(48879),
                           &words[1]);
    if (status != STATUS_OK) {
        (void)fputs("Cannot pack the second metadata word.\n", stderr);
        return EXIT_FAILURE;
    }

    status = write_word_report(words, sizeof words / sizeof words[0]);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

Käännä ja suorita mukana toimitettu liitännäistiedosto täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-007-bitwise-operations-and-uint32-t \
  examples/c/c-007-bitwise-operations-and-uint32-t.c
./c-007-bitwise-operations-and-uint32-t
```

Odotettu tulostus:

```text
Word count: 2
Word 0: 0x0123052A category=42 flags=5 sequence=291
Word 1: 0xBEEFA010 category=16 flags=160 sequence=48879
```

## 7. Ohjattu harjoitus

Vastaa ennen vastausosion lukemista.

1. Miksi ohjelma, joka vaatii täsmällisiä 32‑bittisiä sanoja, tarkistaa `UINT32_MAX`?
2. Mikä operaattori valitsee bittejä maskilla: `&`, `|` vai `^`?
3. Mikä on turvallinen sallittu arvoalue siirtomäärälle täsmällisessä 32‑bittisessä sanassa?
4. Miksi `category`, `flags` ja `sequence` on tarkistettava alueen suhteen ennen pakkaamista?
5. Mikä lauseke poimii flags‑kentän arvon sanasta `word` tässä asettelussa?
6. Miksi raporttisilmukka käyttää eksplisiittistä `count` sen sijaan, että se pysähtyisi kun sana on nolla?
7. Venäjä 007 -harjoituksesta: sano "En lue kirjastossa."

## 8. Itsenäinen tehtävä

Luo `exercise.c` liitännäisohjelman viereen. Määrittele sanan asettelu, jossa on 4‑bittinen priority‑kenttä biteissä 0–3, 4‑bittinen type‑kenttä biteissä 4–7 ja 16‑bittinen identifier‑kenttä biteissä 8–23. Jätä ylemmät kahdeksan bittiä nollaksi tässä harjoituksessa.

Kirjoita `pack_record`‑apufunktio, joka hyväksyy `uint32_t` syötteet ja tulosteen osoittimen. Hylkää null‑tulosteen osoittimet ja syötteet, jotka ovat suurempia kuin niiden kenttämaskit ennen siirtoa. Kirjoita vastaava `unpack_record`‑apufunktio ja raportointi‑apufunktio, joka käsittelee eksplisiittisen pituuden `uint32_t`‑taulukkoa käyttäen `size_t index` ja `index < count`.

Käytä vain siirtomääriä, jotka ovat kääntöaikaisia vakioita alle `32`. Tulosta sanat käyttämällä `PRIX32`. Testaa kelvollinen arvo jokaiselle kentälle ja yksi priority‑arvo, joka on suurempi kuin `15`, varmistaaksesi, että pakkaaja palauttaa virheen ilman, että se korvaa kutsujan olemassa olevaa tulosarvoa.

Tämä on opetuksellinen datan asetteluharjoitus. Se ei ole kryptografinen algoritmi eikä sitä saa koskaan esittää keinona suojata oikeaa dataa.

## 9. Vahvista sanitisaattoreilla

Suorita repositorion deterministinen testi:

```sh
./tests/c-007-bitwise-operations-and-uint32-t.sh
```

Käännä ja suorita sitten AddressSanitizerilla ja UndefinedBehaviorSanitizerilla, kun tuki on käytettävissä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-007-sanitized \
  examples/c/c-007-bitwise-operations-and-uint32-t.c
./c-007-sanitized
```

Kelvollisen raportin tulisi näyttää kaksi sanaa ja niiden kentät ilman sanitisaattorin raportteja. Sanitisaattorit tukevat testejä, mutta ne eivät korvaa eksplisiittisiä kenttäalueen tarkistuksia, kiinteitä turvallisia siirtomääriä, ei‑NULL‑tulostetarkistuksia tai `index < count`‑läpäisyrajauksen tarkistusta lähdekoodissa.

## 10. Lopullinen itse­tarkistus

Olet valmis seuraavaan oppituntiin, jos voit tehdä kaiken seuraavan ilman takaisin katsomista.

1. Selitä, miksi `uint32_t` on suositeltavampi kuin tavallinen `unsigned int` kun formaatti vaatii täsmälleen 32 bittiä.
2. Ilmoita mitä `&`, `|`, `<<` ja `>>` tekevät tässä kenttäpakkaus‑kontekstissa.
3. Ilmoita siirtomäärän sääntö täsmälleiselle 32‑bittiselle sanalle.
4. Selitä, miksi määritelty etumerkitön kääriytyminen ei poista tarvetta validoida kenttäleveyksiä.
5. Kirjoita rajattu silmukkaehto taulukolle, jossa on `count` alkiota.
6. Selitä, miksi tässä oppitunnissa ei ole vielä määritelty tavujen serialisointia tai endianness‑formaattia.
7. Muista venäläinen lause **Я не читаю в библиотеке.**

## Spoileri: vastaukset ja mallivastaus

1. Taulukko­parametri säädetään osoittimeksi, joten eksplisiittinen laskuri määrittää, kuinka monta alkiota on kelvollisia.
2. `index < count`.
3. `<stdint.h>` tarjoaa `uint8_t`; `<inttypes.h>` tarjoaa `PRIX8`.
4. `\0` merkitsee C‑merkkijonon loppua.
5. **Я не учусь в школе.**
6. Makro on ehdollisesti läsnä `uint32_t`; sen olemassaolo varmistaa, että tämä toteutus tarjoaa täsmälleen leveyden vaativan tyyppin, jonka formaatti tarvitsee.
7. `&` valitsee maskin säilyttämät bitit.
8. Siirtomäärän on oltava ei‑negatiivinen ja ehdottomasti pienempi kuin `32`; tämä ohjelma käyttää vain `8` ja `16`.
9. Ilman validointia liialliset kenttäbitit voisivat mennä päällekkäin toisen kentän kanssa tai joutua hylätyiksi, muuttaen dataesitystä hiljaisesti.
10. `flags = (word >> FLAGS_SHIFT) & CATEGORY_MASK;`.
11. Nollasana voi olla kelvollista dataa. Eksplisiittinen laskuri määrittää taulukon rajan.
12. "En lue kirjastossa" on **Я не читаю в библиотеке.**
13. Malli turvallisesta pakkauslausekkeesta alue­tarkistusten jälkeen on:

```c
word = category | (flags << FLAGS_SHIFT) | (sequence << SEQUENCE_SHIFT);
```

14. Endianness kuvaa tavujen järjestystä muistissa tai ulkoisessa esityksessä. Tämä oppitunti käsittelee vain loogisten numeeristen kenttien manipulointia `uint32_t`:ssa; se ei määrittele tiedosto‑, verkko‑ tai muistibytteen formaattia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjään: **Russian 008 — omistussuhteet, tutut henkilöt ja lyhyet omistusta ilmaisevat lauseet.**

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
