# C 005 — Taulukko‑parametrit, `size_t` lukumäärät ja rajattu läpikäynti

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** [C 001 — Käännös, tyypit ja tulostus](c-001-build-types-output.md), [C 002 — Lausekkeet, päätökset, silmukat ja tarkistettu syöte](c-002-control-flow-and-simple-input.md), [C 003 — Funktiot, tulosparametrit ja eksplisiittiset virhepalautukset](c-003-functions-and-error-returns.md), ja [C 004 — Kiinteänkokoiset taulukot, C‑merkkijonot ja indeksirajat](c-004-arrays-strings-and-bounds.md)
**Seuraava esimerkkiohjelma:** [`examples/c/c-005-array-parameters-and-bounded-iteration.c`](../../examples/c/c-005-array-parameters-and-bounded-iteration.c)
**Deterministinen testi:** [`tests/c-005-array-parameters-and-bounded-iteration.sh`](../../tests/c-005-array-parameters-and-bounded-iteration.sh)
**Tekijä:** Manus AI

## Tämän oppitunnin jälkeen osaat

Tämän oppitunnin lopussa osaat välittää taulukon funktiolle yhdessä sen eksplisiittisen alkioiden lukumäärän kanssa, selittää miksi taulukko‑parametri käsitellään osoittimena, käyttää `size_t` alkioiden lukumäärissä ja eteenpäin suuntautuvissa indekseissä sekä kirjoittaa rajatun silmukan, joka käy läpi annetun alueen kukin alkion täsmälleen kerran ilman lukuja taulukon ulkopuolelta.

## Muistin virittely

Vastaa seuraaviin kysymyksiin ilman aiempien oppituntien avaamista.

1. Mikä on kelvollinen indeksi‑alue taulukolle, jossa on `N` alkiota?
2. Miksi näkyvä C‑merkkijono tarvitsee päätteen `\0`?
3. Miksi C 004 vertaa indeksiä arvoon `strlen(label)` eikä taulukon tallennuskapasiteetin etikettiin?
4. Miksi C 003:ssa apufunktio palauttaa tilan erikseen tulosarvosta?
5. Venäjästä 005, täydennä: **я работаю**, **я читаю**, **я ___**.

Spoiler‑osio sisältää mallivastaukset. Yritä muistaa ennen kuin tarkistat vanhoja oppitunteja.

## 1. Taulukko‑parametri on osoitinparametri

C‑taulukot ovat peräkkäisiä alkiosekvenssejä.[1] Kun taulukkoa käytetään funktion argumenttina, se konvertoituu osoittimeksi, joka osoittaa ensimmäiseen alkioon. Funktion parametrilistassa nämä ilmoitukset kuvaavat samaa parametrityyppiä:[1]

```c
static Status summarize(const int values[], size_t count, ...);
static Status summarize(const int *values, size_t count, ...);
```

Ensimmäinen tapa ilmaista parametria on hyödyllinen, koska se kertoo, että osoitteen odotetaan viittaavan kokonaisen kokonaislukutaulukon ensimmäiseen alkioon. Toinen muoto paljastaa taustalla olevan osoitintyypin. Kumpikaan tapa ei siirrä taulukon alkiomäärää funktiolle.

> Rajapintasääntö: Osoitin kertoo, mistä taulukko alkaa. Se ei kerro funktiolle, kuinka monta alkiota on kelvollisia. Lähetä lukumäärä eksplisiittisesti ja käsittele sitä osana funktion sopimusta.

Kumppaniohjelmassa `main` omistaa todellisen taulukon ja voi laskea sen lukumäärän. Se välittää sekä osoitinmaisen taulukkoargumentin että lukumäärän jokaista apufunktiota varten.

```c
const int session_minutes[] = {25, 30, 20, 45, 25};
const size_t session_count = sizeof session_minutes / sizeof session_minutes[0];

status = summarize_session_minutes(session_minutes, session_count,
                                   &total_minutes, &average_minutes);
```

Lauseke `sizeof session_minutes / sizeof session_minutes[0]` toimii tässä, koska `session_minutes` on yhä todellinen taulukko kohdassa `main`. Se tuottaa alkioiden määrän, ei kovakoodattua arvausta.[3]

## 2. `sizeof` muuttaa merkitystään sen jälkeen, kun taulukko on parametri

Funktion sisällä `values` on osoitinparametri, vaikka se on kirjoitettu muodossa `[]`.

```c
static Status summarize_session_minutes(const int values[], size_t count,
                                        int *total_out, int *average_out)
{
    /* sizeof values is the size of a pointer here, not an array length. */
}
```

Tämän käyttäminen funktion sisällä on virheellinen tapa:

```c
size_t wrong_count = sizeof values / sizeof values[0];
```

Se jakaa osoittimen koon erään `int` koon mukaan. Tulos ei liity kutsujan todelliseen taulukon pituuteen ja voi vaihdella alustasta riippuen. C:n taulukko‑parametrin säätäminen on syy siihen, että funktiolle on annettava `count` eksplisiittisesti.[1] [3]

Oikea malli on yksinkertainen:

| Sijainti | Mitä nimi edustaa | Miten saada lukumäärä |
|---|---|---|
| `main`, missä taulukko on ilmoitettu | Kokonainen taulukko‑objekti | `sizeof array / sizeof array[0]` |
| Apufunktion parametri | Osoitin ensimmäiseen alkioon | Vastaanota eksplisiittinen `size_t count`‑parametri |

## 3. Käytä `size_t` lukumäärille ja eteenpäin suuntautuville indekseille

`size_t` on etumerkitön kokonaislukutyyppi, jonka `sizeof` palauttaa. Sillä voidaan esittää minkä tahansa teoreettisen objektin koko, ja sitä käytetään yleisesti taulukkoindekseissä ja silmukkalaskureissa.[2]

```c
for (size_t index = 0; index < count; ++index) {
    /* values[index] is within the supplied range. */
}
```

Tällä silmukalla on selkeä sopimus:

- Se alkaa ensimmäisestä kelvollisesta indeksistä, `0`.
- Se tarkistaa ehdon `index < count` ennen jokaisen alkion käyttämistä.
- Se inkrementoi käsittelyn jälkeen yhdellä.
- Se pysähtyy viimeisen kelvollisen indeksin `count - 1` jälkeen.

Älä kirjoita taaksepäin etenevää silmukkaa kuten `for (size_t index = count - 1; index >= 0; --index)`. Koska `size_t` on etumerkitön, indeksi `0` kierähtää hyvin suureksi arvoksi vähentämisen jälkeen. Käännösläpikäynti on mahdollista, mutta se vaatii erikseen huolellisesti suunnitellun mallin. Tämä oppitunti käyttää eteenpäin suuntautuvaa läpikäyntiä, koska sen raja on suora ja helpompi tarkistaa.

## 4. Osoitin kertoo tallennuksen sijainnin; määrä asettaa rajat

Ei‑nullin `values` osoittimen ja kelvollisen `count` kohdalla taulukko‑indeksointi on luettava osoitinaritmetiikkana:

```c
int value = values[index];
/* Equivalent address expression: *(values + index) */
```

Käytä `values[index]` tavalliseen läpikäyntiin. Se pitää indeksin ja rajan näkyvillä yhdessä. Älä yritä "löytää" kokonaislukutaulukon loppua muistia skannaamalla. Toisin kuin C‑merkkijonot, yleisissä `int` taulukoissa ei ole sisäänrakennettua terminointimerkkiä. Niiden kelvollinen alue tulee eksplisiittisestä lukumäärästä.

Kumppanin apufunktio hylkää nämä rajapinnan virheet ennen läpikäyntiä:

```c
if (values == NULL || count == 0 || count > (size_t)INT_MAX ||
    total_out == NULL || average_out == NULL) {
    return STATUS_INVALID_ARGUMENT;
}
```

Nolla‑lukumäärä on virheellinen tässä pienessä istuntotiivistelmä‑rajapinnassa, koska keskiarvo olisi määrittelemätön. `count > INT_MAX`‑tarkistus tekee myöhemmän muunnoksen kokonaislukujakoon turvalliseksi. Nämä ovat sovelluskohtaisia esiehtoja, mutta jokainen taulukkoa käsittelevä funktio tarvitsee selkeät säännöt nollaosoittimille, lukumäärille ja myöhemmälle aritmetiikalle.

## 5. Tarkista jokainen alkio ja suojaa kertynyt summa

Rajoitettu indeksi estää indeksin ulkopuolisen lukemisen. Se ei kuitenkaan todista, että kaikki tallennetut arvot ovat ohjelmalle sopivia. Esimerkki hyväksyy istuntojen kestot väliltä `0`–`180` minuuttia ja tarkistaa summan ennen uuden arvon lisäämistä.

```c
for (size_t index = 0; index < count; ++index) {
    int value = values[index];

    if (value < 0 || value > MAX_SESSION_MINUTES) {
        return STATUS_INVALID_VALUE;
    }

    if (total > INT_MAX - value) {
        return STATUS_SUM_OVERFLOW;
    }

    total += value;
}
```

Vähennys on turvallinen, koska `value` vaadittiin ensin ei‑negatiiviseksi. Tarkistus todistaa, että `total + value` mahtuu tyyppiin `int` ennen kuin lisäys tapahtuu. Vasta, kun koko läpikäynti onnistuu, apufunktio asettaa `*total_out` ja `*average_out`. Tämä noudattaa C 003:n sääntöä tulosparametreista: kutsujat eivät saa käyttää tulosarvoa, jos palautettu tila ei ilmoita onnistumista.

## 6. Työstetty esimerkki: raportoi rajattu istuntotaulukko

Kumppaniohjelma pitää kiinteää taulukkoa istuntojen kestoista. `main` laskee lukumäärän vielä silloin, kun sillä on taulukko‑objekti. Apufunktiot saavat osoitin‑ja‑lukumäärä‑parin, käyvät läpi vain ilmoitetun alueen ja raportoivat virheet `Status` kautta.

```c
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_INVALID_VALUE = 2,
    STATUS_SUM_OVERFLOW = 3,
    STATUS_OUTPUT_FAILURE = 4
} Status;

enum {
    MAX_SESSION_MINUTES = 180
};

static Status summarize_session_minutes(const int values[], size_t count,
                                        int *total_out, int *average_out)
{
    int total = 0;

    if (values == NULL || count == 0 || count > (size_t)INT_MAX ||
        total_out == NULL || average_out == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    for (size_t index = 0; index < count; ++index) {
        int value = values[index];

        if (value < 0 || value > MAX_SESSION_MINUTES) {
            return STATUS_INVALID_VALUE;
        }

        if (total > INT_MAX - value) {
            return STATUS_SUM_OVERFLOW;
        }

        total += value;
    }

    *total_out = total;
    *average_out = total / (int)count;
    return STATUS_OK;
}

static Status write_session_report(const int values[], size_t count,
                                   int total, int average)
{
    if (values == NULL || count == 0) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Session count: %zu\n", count) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    for (size_t index = 0; index < count; ++index) {
        if (printf("Session %zu: %d minutes\n", index, values[index]) < 0) {
            return STATUS_OUTPUT_FAILURE;
        }
    }

    if (printf("Total minutes: %d\n", total) < 0 ||
        printf("Average whole minutes: %d\n", average) < 0) {
        return STATUS_OUTPUT_FAILURE;
    }

    return STATUS_OK;
}

static Status write_error(const char *message)
{
    if (message == NULL || fputs(message, stderr) == EOF) {
        return STATUS_OUTPUT_FAILURE;
    }

    return STATUS_OK;
}

int main(void)
{
    const int session_minutes[] = {25, 30, 20, 45, 25};
    const size_t session_count =
        sizeof session_minutes / sizeof session_minutes[0];
    int total_minutes;
    int average_minutes;
    Status status;

    status = summarize_session_minutes(session_minutes, session_count,
                                       &total_minutes, &average_minutes);
    if (status == STATUS_INVALID_VALUE) {
        (void)write_error("Session minutes must be between 0 and 180.\n");
        return EXIT_FAILURE;
    }
    if (status == STATUS_SUM_OVERFLOW) {
        (void)write_error("Session total is too large.\n");
        return EXIT_FAILURE;
    }
    if (status != STATUS_OK) {
        (void)write_error("Cannot summarize the session array.\n");
        return EXIT_FAILURE;
    }

    status = write_session_report(session_minutes, session_count,
                                  total_minutes, average_minutes);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

Käännä ja suorita tallennettu kumppaniohjelmatiedosto täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-005-array-parameters-and-bounded-iteration \
  examples/c/c-005-array-parameters-and-bounded-iteration.c
./c-005-array-parameters-and-bounded-iteration
```

Odotettu tulostus:

```text
Session count: 5
Session 0: 25 minutes
Session 1: 30 minutes
Session 2: 20 minutes
Session 3: 45 minutes
Session 4: 25 minutes
Total minutes: 145
Average whole minutes: 29
```

## 7. Ohjattu harjoitus

Vastaa ennen spoiler‑osion lukemista.

1. Miksi `summarize_session_minutes` täytyy vastaanottaa `count` vaikka se jo saa `values[]`?
2. Mikä on kelvollinen indeksi‑alue kun `count` on `5`?
3. Miksi silmukan ehto käyttää `index < count` eikä `index <= count`?
4. Mitä `const` tarkoittaa kohdassa `const int values[]` tälle apufunktiolle?
5. Miksi ohjelma käyttää `size_t` tyyppiä `count` ja `index` varten?
6. Mitä tapahtuu, jos yksi alkio on `200` tässä ohjelman istuntotaulukossa?
7. Miksi ohjelma tarkistaa `total > INT_MAX - value` ennen lisäämistä?
8. Venäjästä 005, sano 'Opiskelen illalla' venäjäksi.

## 8. Itsenäinen harjoitus

Luo `exercise.c` kumppaniohjelman viereen. Määrittele kiinteä taulukko, jossa on viisi ei‑negatiivista arvostelupistettä. Kirjoita `summarize_scores`‑funktio, joka vastaanottaa `const int scores[]`, yhden `size_t count`, sekä tulosparametrit summalle ja korkeimmalle pistemäärälle.

Funktiosi tulee hylätä nollaosoitin, nolla‑lukumäärä ja mikä tahansa pistemäärä, joka ei kuulu välille `0`–`100`. Käännä taulukko eteenpäin suuntautuvalla silmukalla, joka käyttää `size_t index` ja `index < count`. Tarkista summa ennen jokaista lisäystä, jotta ylivuoto ei ole mahdollista. Laske lukumäärä kohdassa `main` käyttäen `sizeof scores / sizeof scores[0]`, ja sitten lähetä se eksplisiittisesti apufunktiolle.

Kirjoita toinen apufunktio, joka tulostaa jokaisen `scores[index]` arvon. Älä laske taulukon pituutta `sizeof` avulla kummankaan apufunktion sisällä. Käännä tiukoilla kääntäjäasetuksilla ja testaa kelvollisella viiden arvon taulukolla sekä tarkoituksellisesti muuta yksi arvo `101` varmistaaksesi, että virhepolku tulee käytännössä testatuksi.

## 9. Tarkista sanitisaattoreilla

Suorita reposta deterministinen testi:

```sh
./tests/c-005-array-parameters-and-bounded-iteration.sh
```

Sitten rakenna ja suorita AddressSanitizerin ja UndefinedBehaviorSanitizerin kanssa, kun alusta tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-005-sanitized \
  examples/c/c-005-array-parameters-and-bounded-iteration.c
./c-005-sanitized
```

Ohjelman tulisi tulostaa viisi rajattua arvoa, summa ja keskiarvo ilman sanitisaattoriraportteja. Sanitisaattorit auttavat löytämään joitakin muistiin ja määrittelemättömään käyttäytymiseen liittyviä virheitä testauksen aikana. Ne eivät korvaa eksplisiittisiä nollaosoitin‑, lukumäärä‑, alkioiden validointi‑ eikä silmukkarajan tarkistuksia lähdekoodissa.

## 10. Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit tehdä seuraavat ilman katsomista:

1. Selitä, miksi `int values[]` ja `int *values` kuvaavat samaa tavallista funktioparametrityyppiä.
2. Kerro, miksi kutsuttu funktio ei voi palauttaa taulukon pituutta käyttäen `sizeof values`.
3. Kirjoita turvallinen eteenpäin suuntautuva läpikäyntiehto taulukolle, jossa on `count` alkiota.
4. Selitä, miksi yleinen kokonaislukutaulukko tarvitsee eksplisiittisen lukumäärän, kun taas C‑merkkijonolla on nollaterminointikonventio.
5. Kerro, miksi `size_t` sopii taulukon lukumäärille ja eteenpäin suuntautuville indekseille.
6. Selitä, miksi apufunktio asettaa tulosparametrit vasta onnistuneen läpikäynnin jälkeen.
7. Muista venäläinen lause **Вечером я учусь.**

## Spoiler: vastaukset ja mallivastaus

1. Taulukolla, jossa on `N` alkiota, kelvolliset indeksit ovat `0` … `N - 1`.
2. C‑merkkijono tarvitsee `\0` merkitsemään, missä näkyvä merkkijono päättyy.
3. `strlen(label)` mittaa loogisen näkyvän merkkijonon pituuden. Lisäkapasiteetti ei ole osa syötemerkkijonoa.
4. Eri tila tekee onnistumisen/epäonnistumisen yksiselitteiseksi ja estää kutsujaa käsittelemästä virheellistä tulosarvoa validina datana.
5. **учусь**.
6. Parametri säädetään osoittimeksi, eikä osoitin säilytä kutsujan alkiomäärää. Lähetä `count` eksplisiittisesti.
7. Kun `count == 5`, kelvolliset indeksit ovat `0`, `1`, `2`, `3`, ja `4`.
8. `index <= count` sallii `index == count`, joka on yksi viimeisen alkion jälkeen ja siksi virheellinen.
9. `const` estää tätä apufunktiota muokkaamasta alkioita kautta `values`‑parametrin.
10. `size_t` on tyyppi, jonka `sizeof` palauttaa ja se on suunniteltu kuvaamaan objektien kokoja; se sopii lukumäärille ja eteenpäin suuntautuville indekseille.
11. Arvolla `200` palautetaan `STATUS_INVALID_VALUE`; `main` ilmoittaa, että istuntominuutit pitää olla välillä `0`–`180` ja poistuu epäonnistumiseen.
12. Tarkistus todistaa, että `total + value` mahtuu tyyppiin `int` ennen lisäystä.
13. "I study in the evening" on **Вечером я учусь.**

Turvallinen rajattu läpikäynti on:

```c
for (size_t index = 0; index < count; ++index) {
    use(values[index]);
}
```

## Seuraava oppitunti

Seuraava oppitunti palaa venäjään: **Venäjä 006 — yleisiä paikkoja, `где?` kysymyksiä ja perussijaintilauseita.**

## Viitteet

[1]: https://en.cppreference.com/w/c/language/array "Taulukon ilmoitus — cppreference.com"
[2]: https://en.cppreference.com/w/c/types/size_t "size_t — cppreference.com"
[3]: https://en.cppreference.com/w/c/language/sizeof "sizeof‑operaattori — cppreference.com"
