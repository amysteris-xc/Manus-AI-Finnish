# C 006 — `uint8_t`, tavutaulukot ja eksplisiittiset pituudet

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** [C 001 — Kääntäminen, tyypit ja tulostus](c-001-build-types-output.md), [C 002 — Lausekkeet, valinnat, silmukat ja tarkastettu syöte](c-002-control-flow-and-simple-input.md), [C 003 — Funktiot, ulostuloparametrit ja eksplisiittiset virhepaluut](c-003-functions-and-error-returns.md), [C 004 — Kiinteän kokoiset taulukot, C-merkkijonot ja indeksi-rajat](c-004-arrays-strings-and-bounds.md), ja [C 005 — Taulukko-parametrit, `size_t` määrät ja rajattu läpikäynti](c-005-array-parameters-and-bounded-iteration.md)
**Mukana oleva esimerkki:** [`examples/c/c-006-unsigned-integers-and-byte-arrays.c`](../../examples/c/c-006-unsigned-integers-and-byte-arrays.c)
**Deterministinen testi:** [`tests/c-006-unsigned-integers-and-byte-arrays.sh`](../../tests/c-006-unsigned-integers-and-byte-arrays.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat selittää, miksi eksplisiittisen pituuden omaava `uint8_t` taulukko ei ole C-merkkijono, esittää tavallista nollarvoista dataa turvallisesti, välittää tavualueen yhdessä `size_t` pituuden kanssa, läpikäydä sen `index < count` avulla ja tulostaa sen yksiköt heksadesimaalisina käyttämällä standardin kokonaislukumuotoilumakroja.

## Kertauslämmittely

Vastaa näihin kysymyksiin avaamatta aiempia oppitunteja.

1. Miksi funktio, joka vastaanottaa taulukko-parametrin, tarvitsee myös eksplisiittisen määrän?
2. Kirjoita turvallinen eteenpäin kulkevan silmukan ehto taulukolle, jonka kelvollinen määrä on `count`.
3. Mitä `\0` tarkoittaa C-merkkijonolle?
4. Miksi `strlen` ei sovi mielivaltaiselle tavualueelle?
5. Käännä venäjästä 006: **Я читаю в библиотеке.**

Spoiler-osiosta löytyy mallivastaukset. Yritä palauttaa muistiin ennen aiempien oppituntien tarkastamista.

## 1. 8-bittinen yksikkö on dataa, ei automaattisesti tekstiä

C:n **tavu** on yksikkö, jota `sizeof` mittaa; sen bittileveys annetaan makrolla/ominaisuudella `CHAR_BIT` ja se on vähintään 8 bittiä.[3] Monet nykyaikaiset järjestelmät käyttävät 8-bittisiä tavuja, mutta kannettava C-ohjelma ei saa kevyesti olettaa, että kielten tavu on aina kahdeksan bittiä.

Kun toteutus tarjoaa `uint8_t` otsikossa `<stdint.h>`, se on allekirjoittamaton kokonaislukutyyppi, jolla on **tasan 8 bittiä** eikä täytebittejä.[1] Se on hyödyllinen silloin, kun protokolla, tiedostomuoto, testivektori tai laiteväylä määrittelee yksiköt kahdeksan bittisinä. Tarkkuusleveys-typedef on ISO C:ssä valinnainen, joten kannettavan ohjelman, joka vaatii sen, tulisi epäonnistua selvästi käännösaikana, jos alusta ei sitä tarjoa.

```c
#include <stdint.h>

#if !defined(UINT8_MAX)
#error "This program requires an exact 8-bit uint8_t type."
#endif
```

Älä käytä `uint8_t` vain siksi, että se kuulostaa teknisemmältä. Käytä sitä silloin, kun datan määrittely sanoo "kahdeksanbittinen allekirjoittamaton yksikkö." Käytä muita tyyppejä, kun niiden semantiikka on ongelmalle selvempi.

## 2. Tavupuskuri ja C-merkkijono eroavat rajauksiltaan

C-merkkijono päättyy ensimmäiseen nollimerkkiin, `\0`. Sen sijaan tavualue käyttää eksplisiittistä pituutta. Nollarvoinen yksikkö on tavallinen data tavualueen sisällä.

```c
char text[] = "A\0B";
uint8_t bytes[] = {UINT8_C(0x41), UINT8_C(0x00), UINT8_C(0x42)};
```

| Olio | Tallennetut yksiköt | Looginen raja |
|---|---|---|
| `text` | `A`, `\0`, `B`, viimeinen `\0` | C-merkkijonofunktiot pysähtyvät ensimmäiseen `\0` |
| `bytes` | `0x41`, `0x00`, `0x42` | Kutsujan antama pituus, tässä `3` |

Merkkijonolle `strlen(text)` on `1`, koska ensimmäinen nollamerkki seuraa `A`. Tavualueella kaikki kolme yksikköä kuuluvat puskurin sisään, mukaan lukien `0x00`. `strlen` kutsuminen kohdalla `bytes` ei ole tapa saada sen pituutta ja on virheellinen: tavualue ei välttämättä ole päättynyt, ja upotettu nolla ei merkitse sen loppua.[2]

> Rajausohje: Tekstille, joka on C-merkkijono, varmista nollapääte ennen merkkijonofunktioiden kutsumista. Binääriselle tai tavupohjaiselle datalle kanna eksplisiittinen osoitin-plus-pituus -pari äläkä keksi terminaattoria.

## 3. Välitä `uint8_t` puskurit yhdessä `size_t` pituuksien kanssa

C 005 vakiinnutti yleisen taulurajapinnan: taulukko-parametri säädetään osoittimeksi, joten funktio ei saa kutsujan alkioiden määrää. Sama sääntö pätee tavualueeseen.

```c
static Status count_zero_bytes(const uint8_t bytes[], size_t count,
                               size_t *zero_count_out);
```

Funktion sopimus on:

| Parametri | Merkitys |
|---|---|
| `bytes` | Osoitin ensimmäiseen kelvolliseen `uint8_t` yksikköön |
| `count` | Tarkka määrä kelvollisia yksiköitä alkaen kohdasta `bytes` |
| `zero_count_out` | Osoite, johon onnistunut tulos kirjoitetaan |

Avustajan sisällä turvallinen läpikäynti pysyy muuttumattomana:

```c
for (size_t index = 0; index < count; ++index) {
    if (bytes[index] == UINT8_C(0)) {
        ++zero_count;
    }
}
```

Silmukka käy indeksejä läpi kohdista `0` … `count - 1`. Se ei pysähdy kohtaan `0x00`; se käsittelee tuota arvoa datana ja jatkaa kunnes saavuttaa eksplisiittisen rajan.

## 4. Käytä `UINT8_C` ja `PRIX8` selkeään kiinteäleveyskoodiin

`UINT8_C(value)` luo kokonaislukuvakion, joka sopii `uint_least8_t` perheelle, kun vastaava tarkkuustyyppi on saatavilla.[1] Se tekee literalin leveystarkoituksen näkyväksi puskurin alustuksessa:

```c
UINT8_C(0x52)
UINT8_C(0x00)
UINT8_C(0xA7)
```

Kun tulostat kiinteän leveyden kokonaislukutyyppejä, `<inttypes.h>` tarjoaa vastaavat muotomakrot. `PRIX8` on isolla kirjaimella oleva heksadesimaalinen muotomakro, joka liittyy tyyppiin `uint8_t` silloin kun kyseinen tyyppi on olemassa.[1]

```c
printf("%02" PRIX8, bytes[index]);
```

`02` pyytää vähintään kahta heksadesimaalista numeroa, joten arvot näkyvät muodossa `00`, `41`, `52`, tai `A7`. Älä oleta, että `%x` on aina suoraan oikea formaatti jokaiselle typedefille. Standardin makrot ilmaisevat tyyppisuhteen eksplisiittisesti.

## 5. Varmista rajapinta ennen puskurin lukemista

Seuraohjelman apufunktio hylkää null-osoittimen, nollamäärän ja null-tulososoittimen ennen kuin se pääsee käsiksi `bytes[index]` tai kirjoittaa ulostulon.

```c
if (bytes == NULL || count == 0 || zero_count_out == NULL) {
    return STATUS_INVALID_ARGUMENT;
}
```

Nollapituisuus voi olla validi joissain laajemmissa rajapinnoissa, mutta tämä opetustarkoitteinen ohjelma hylkää sen jotta raportti aina kuvaa ainakin yhtä yksikköä. Tärkeä suunnitteluopetus ei ole, että jokaisen todellisen API:n täytyy hylätä nollapituus. Se on, että funktion sopimuksessa tulisi määritellä, mitä osoitin- ja määrä-yhdistelmä tarkoittaa, ja noudattaa tätä sopimusta ennen läpikäyntiä.

Avustaja asettaa `*zero_count_out` vasta, kun koko silmukka on päässyt onnistuneesti loppuun. Tämä säilyttää C 003:n säännön, jonka mukaan kutsujan on tarkastettava tila ennen ulostulon käyttämistä.

## 6. Harjoitusesimerkki: raportoi eksplisiittipituinen tavualue

Mukana oleva esimerkki sisältää kuusi `uint8_t` arvoa. Kaksi on `0x00`, ja molempien on sisäänilmestyttävä heksadesimaaliraportissa ja vaikuttaa nollalaskuriin. Ohjelmassa ei käytetä C-merkkijono-operaatioita.

```c
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(UINT8_MAX)
#error "This program requires an exact 8-bit uint8_t type."
#endif

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_OUTPUT_FAILURE = 2
} Status;

static Status count_zero_bytes(const uint8_t bytes[], size_t count,
                               size_t *zero_count_out)
{
    size_t zero_count = 0;

    if (bytes == NULL || count == 0 || zero_count_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    for (size_t index = 0; index < count; ++index) {
        if (bytes[index] == UINT8_C(0)) {
            ++zero_count;
        }
    }

    *zero_count_out = zero_count;
    return STATUS_OK;
}

static Status write_byte_report(const uint8_t bytes[], size_t count,
                                size_t zero_count)
{
    if (bytes == NULL || count == 0) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Byte count: %zu\nBytes:", count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    for (size_t index = 0; index < count; ++index) {
        if (printf(" %02" PRIX8, bytes[index]) < 0) {
            return STATUS_OUTPUT_FAILURE;
        }
    }

    if (printf("\nZero-valued bytes: %zu\n", zero_count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    return STATUS_OK;
}

int main(void)
{
    const uint8_t payload[] = {
        UINT8_C(0x52), UINT8_C(0x00), UINT8_C(0x41),
        UINT8_C(0xA7), UINT8_C(0x00), UINT8_C(0x7F)
    };
    const size_t payload_count = sizeof payload / sizeof payload[0];
    size_t zero_count;
    Status status;

    status = count_zero_bytes(payload, payload_count, &zero_count);
    if (status != STATUS_OK) {
        (void)fputs("Cannot inspect the byte buffer.\n", stderr);
        return EXIT_FAILURE;
    }

    status = write_byte_report(payload, payload_count, zero_count);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

Käännä ja suorita mukana toimitettu tiedosto täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-006-unsigned-integers-and-byte-arrays \
  examples/c/c-006-unsigned-integers-and-byte-arrays.c
./c-006-unsigned-integers-and-byte-arrays
```

Odotettu tuloste:

```text
Byte count: 6
Bytes: 52 00 41 A7 00 7F
Zero-valued bytes: 2
```

Tuloste todistaa, että `0x00` näkyy raportissa kahdesti. Jos ohjelma olisi käyttänyt C-merkkijonon rajaa, se pysähtyy ensimmäiseen nollaan ja menettäisi jäljellä olevan datan.

## 7. Ohjattu harjoitus

Vastaa ennen spoiler-osion lukemista.

1. Mikä on ero C-merkkijonon ja eksplisiittipituuksisen tavualueen välillä?
2. Jos `{0x41, 0x00, 0x42}` sisältää countin `3`, kuinka monta yksikköä on kelvollisia?
3. Miksi tavualue-funktion täytyy vastaanottaa `size_t count`?
4. Mikä silmukkaehto käy jokaisen kelvollisen yksikön täsmälleen kerran?
5. Miksi silmukan ei tule pysähtyä kohdassa `bytes[index] == 0`?
6. Mikä otsikko tarjoaa `uint8_t`?
7. Mikä otsikko tarjoaa `PRIX8`?
8. Venäjä 006:sta, sano venäjäksi "I am at home."

## 8. Itsenäinen harjoitus

Luo `exercise.c` viereen mukana olevan esimerkin kanssa. Määrittele kiinteä `uint8_t` taulukko, joka sisältää kuusi arvoa, mukaan lukien vähintään yksi `UINT8_C(0x00)` ja yksi arvo yli `UINT8_C(0x7F)`. Kirjoita `count_high_bytes` apufunktio, joka vastaanottaa `const uint8_t bytes[]`, `size_t count`, ja `size_t *result` ulostuloparametrin.

Apufunktion on hylättävä null-puskuriosoite, nollamäärä ja null-ulostulo. Sen täytyy käyttää `index < count` ja sellaisia lukuarvoja, joille `bytes[index] >= UINT8_C(0x80)`. Tulosta jokainen yksikkö kahdella isolla heksadesimaalimerkillä käyttäen `PRIX8`, sitten tulosta tulos. Älä käytä `strlen`, `%s`, tai mitään terminaattoripohjaista pysäytystapaa. Testaa taulukko, jossa on upotettu nolla, ja varmista, että nollan jälkeiset yksiköt näkyvät silti tulosteessa.

Tämä on datan tarkastelua, ei salaustekniikkaa. Sitä ei pidä kuvata salaukseksi, hajautukseksi tai todellisen datan suojeluksi.

## 9. Testaa sanitisaattoreilla

Suorita repositoryn deterministinen testi:

```sh
./tests/c-006-unsigned-integers-and-byte-arrays.sh
```

Sitten käännä ja suorita AddressSanitizerilla ja UndefinedBehaviorSanitizerilla, kun alusta tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-006-sanitized \
  examples/c/c-006-unsigned-integers-and-byte-arrays.c
./c-006-sanitized
```

Ohjelman tulisi tulostaa kaikki kuusi yksikköä ja nollalaskuri `2`, ilman sanitizer-raportteja. Sanitisaattorit auttavat testausta, mutta eksplisiittiset osointotarkistukset, eksplisiittinen määrä ja `index < count` suoja ovat edelleen lähdekooditason todiste puskurirajasta.

## 10. Lopputarkastus itsearviointina

Olet valmis seuraavaan oppituntiin, jos osaat kaikki seuraavat ilman paluuta materiaaliin.

1. Selitä, miksi `uint8_t` puskuri tarvitsee eksplisiittisen pituuden vaikka se sisältäisi `0x00`.
2. Erittele ero `\0` C-merkkijonon päättäjänä ja `0x00` tavualueen datana.
3. Kirjoita turvallinen silmukan otsake tavualueelle, jolla on `count` kelvollista yksikköä.
4. Nimeä otsikot, jotka tarjoavat `uint8_t` ja `PRIX8`.
5. Selitä, miksi esimerkin heksadesimaalinen silmukka tulostaa arvoja upotetun nollan jälkeen.
6. Perustele, miksi `uint8_t` saatavuus tulisi tarkistaa, kun tarkat 8-bittiset yksiköt ovat ohjelman vaatimus.
7. Muista venäläinen lause **Я дома.**

## Spoiler: vastaukset ja malli

1. Funktio saa osoittimen, ei kutsujan kokonaista taulukko-oliota, joten se tarvitsee määrän tietääkseen kelvollisen rajan.
2. `index < count`.
3. `\0` merkitsee C-merkkijonon loppua.
4. `strlen` etsii C-merkkijonon päättymistä; mielivaltainen tavualue voi sisältää upotettuja nollia tai ei lainkaan päättämistä.
5. **Я читаю в библиотеке.** tarkoittaa "Luen kirjastossa."
6. `uint8_t` tarjoaa täsmälleen 8 allekirjoittamatonta bittiä, kun toteutus sen toimittaa; C-merkkijono on erillinen tekstikonventio.
7. Kaikki **3** yksikköä ovat kelvollisia. Keskimmäinen `0x00` on dataa, ei loppumerkkiä.
8. Funktioparametri on osoitin, joten se ei säilytä kutsujan taulukon pituutta. Välitä `count` eksplisiittisesti.
9. `for (size_t index = 0; index < count; ++index)`.
10. Nollarvoinen tavu voi olla tavallista binääridataa, joten silmukka jatkuu kunnes eksplisiittinen määrä saavutetaan.
11. `<stdint.h>` toimittaa `uint8_t`; `<inttypes.h>` toimittaa `PRIX8`.
12. "I am at home" on **Я дома.**
13. Tarkkuustyyppit ovat valinnaisia ISO C:ssä. Ohjelman, joka vaatii tarkkaa 8-bittistä allekirjoittamatonta tyyppiä, tulisi havainnoida `uint8_t` puuttuminen sen sijaan, että hiljaisesti korvaisi tyypin eri leveydellä tai esityksellä.

Mallipuskuri-deklaratio on:

```c
const uint8_t bytes[] = {
    UINT8_C(0x41), UINT8_C(0x00), UINT8_C(0xC3)
};
const size_t count = sizeof bytes / sizeof bytes[0];
```

Kaikki kolme arvoa kuuluvat puskurin sisään.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjään: **Russian 007 — kyllä/ei-kysymykset, `не`, ja ytimekkäitä vastauksia rutiineista ja sijainneista.**

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/string/byte/strlen "strlen, strnlen_s — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/limits "Numeraaliset rajat — cppreference.com"
