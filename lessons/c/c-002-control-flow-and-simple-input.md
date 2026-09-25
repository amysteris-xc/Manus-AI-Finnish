# C 002 — Lausekkeet, päätökset, silmukat ja tarkistettu syöte

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** [C 001 — Rakentaminen, tyypit ja tulostus](c-001-build-types-output.md)
**Seuralaisohjelma:** [`examples/c/c-002-control-flow-and-simple-input.c`](../../examples/c/c-002-control-flow-and-simple-input.c)
**Deterministinen testi:** [`tests/c-002-control-flow-and-simple-input.sh`](../../tests/c-002-control-flow-and-simple-input.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat arvioida perus C-lausekkeita, valita suoritettavan polun käyttäen `if` ja `else`, toistaa rajatun toiminnon `for` avulla, lukea yhden rajatun rivin `fgets`:lla sekä muuntaa kymmenjärjestelmän kokonaisluvun `strtol`:lla vasta sen jälkeen, kun olet tarkistanut puuttuvat numerot, ylimääräisen tekstin, alueen muodon ja ohjelman hyväksymän alueen.

## Kertauslämmittely

Älä avaa C 001 -oppituntia; vastaa seuraaviin kysymyksiin.

1. Mikä otsikkotiedosto määrittelee `printf`?
2. Mikä muunnosspesifikaattori vastaa `int`?
3. Mitä `return 0;` tavallisesti raportoi `main`sta?
4. Kumpi tyyppi sopii paremmin kokonaislukuiselle oppituntien määrälle: `int` vai `double`?

Sitten muistele yksi asia Russian 002: miten kohteliaasti kysyt "Where are you from?" Vastaukset ovat paljastusosiossa. Tämä lyhyt kertaus ylläpitää molempia kurssiratoja.

## 1. Lausekkeet luovat arvoja

Lauseke (expression) tuottaa arvon. C-ohjelmat tekevät päätöksiä ja toistavat tehtäviä arvioimalla lausekkeita, kuten vertailuja ja aritmetiikkaa.

```c
const int completed = 12;
const int group_size = 5;
const int checkpoints = completed / group_size;
const int remainder = completed % group_size;
```

`/`‑operaattori kahden `int` arvon välissä suorittaa kokonaislukujakamisen, joten `12 / 5` on `2`. `%`‑operaattori on jäännösoperaattori, joten `12 % 5` on `2`. Käytä `%` vain kokonaislukutyypeillä.

| Lauseke | Merkitys kun `completed` on 12 |
|---|---|
| `completed > 0` | tosi, esitetty ei‑nollaisena arvona |
| `completed == 12` | tosi |
| `completed != 12` | epätosi, esitetty nollana |
| `completed % 5 == 0` | epätosi |
| `completed / 5` | `2` |

Älä käytä `=` kun tarkoitat "on yhtä kuin". `=` asettaa arvon, kun taas `==` vertaa arvoja. Kääntäjät voivat varoittaa joistain vahingossa tehdystä sijoituksista ehdoissa, mutta selkeä koodi ja huolellinen tarkastus ovat ensisijainen suoja.

## 2. Päätökset valitsevat yhden polun

Lause `if` suorittaa lohkonsa, kun sen ehto ei ole nolla. `else` tarjoaa vaihtoehtoisen suorituskulun. Pidä aaltosulkeet (braces) aina mukana, vaikka lohko sisältäisi vain yhden lauseen; ne tekevät myöhemmistä muutoksista turvallisempia ja helpommin tarkastettavia.

```c
if (lesson_count == 1) {
    printf("Plan: 1 lesson\n");
} else {
    printf("Plan: %d lessons\n", lesson_count);
}
```

Ehto arvioidaan kerran. Tarkasti yksi haara suoritetaan. Seuralaisohjelmassa virheellinen syöte menee virhepolulle; kelvollinen syöte vie joko yksikkö‑ tai monikkovastauspolulle.

> Rajaohje: Älä anna muunnosfunktion paluuarvon yksin päättää, että syöte on kelvollinen. Validoinnin on huomioitava koko syöte, muunnoksen sallima alue ja ohjelman oma hyväksymä alue.

## 3. `for`‑silmukka toistaa rajatun toiminnon

Lause `for` sisältää alustuslausekkeen, ehdon ja iteraatioilmaisun.[3] Ehto testataan ennen jokaista silmukan toistoa. Kun se muuttuu nollaksi, silmukka lopettaa.

```c
int checkpoint_count = 0;

for (int day = 1; day <= lesson_count; ++day) {
    if (day % 5 == 0) {
        ++checkpoint_count;
    }
}
```

Tämä esimerkki laskee viiden päivän välein päiviä `1`—`lesson_count`. Silmukka on rajattu, koska `day` alkaa arvosta `1`, kasvaa kerran per iteraatio ja pysähtyy, kun se ylittää `lesson_count`. Silmukalla täytyy olla päättymissyytä, joka voidaan varmistaa koodikatselmuksessa.

Vaihtoehtoisesti `lesson_count == 12`-tapauksessa `if`-ehto onnistuu päivinä `5` ja `10`, joten `checkpoint_count` päättyy arvoon `2`.

## 4. Lue rivi ennen muuntamista

Ensimmäiseksi tarkastetuksi syöteraajaksi, älä pyydä `scanf` sijoittamaan kokonaislukua suoraan `int`:aan. Lue sen sijaan rivi kiinteäkokoiseen puskurimuuttujaan ja pura sekä validoi merkkijono tarkoituksellisesti.

```c
char input[64];

if (fgets(input, sizeof input, stdin) == NULL) {
    /* Handle end of input or an input error. */
}
```

`fgets` lukee korkeintaan yhden merkin vähemmän kuin annettu määrä, pysähtyy rivinvaihdon jälkeen jos sellainen luetaan, ja liittää nollaterminointimerkin onnistuneen syötteen jälkeen.[1] `sizeof input` antaa kääntäjän laskea oikean kapasiteetin todellisesta taulukosta; se välttää numeron manuaalisen toiston `64`:ssa.

Kiinteä puskurikoko on tarkoituksellinen raja. Se tarkoittaa, että ohjelman on päätettävä, mitä tehdä jos rivi on pidempi kuin puskurikoko. Seuralaisohjelma hylkää tällaisen rivin sen jälkeen kun se on heittänyt jäljellä olevat merkit stdin:stä. Se ei hiljaa katkaise syötettä ja teeskennellä sen olleen täydellinen.

## 5. Muunna ja validoi käyttäen `strtol`

`strtol` muuntaa nollaterminoitu tavumerkkijonon (`long`:ksi) ja ilmoittaa, missä jäsentäminen pysähtyi lopetusosoittimen kautta.[2] Vankka kutsu tarvitsee useita tarkistuksia:

1. Aseta `errno = 0` ennen kutsua, koska `errno` voi sisältää vanhan virheen.
2. Tarkista `end == text`; tämä tarkoittaa, että yhtään numeroa ei muutettu.
3. Tarkista `errno == ERANGE`; tämä tarkoittaa, että muunnettu arvo ei mahtunut tyyppiin `long`.
4. Ohita vain sallitut perässä olevat välilyönnit, ja vaadi sitten `*end == '\0'`; tämä hylkää `12cats`.
5. Tarkista ohjelman oma hyväksymä alue ennen kuin teet tyyppimuunnoksen `long` -> `int`.

```c
errno = 0;
value = strtol(text, &end, 10);

if (end == text || errno == ERANGE) {
    return 0;
}

while (isspace((unsigned char)*end)) {
    ++end;
}

if (*end != '\0' || value < 1 || value > 100) {
    return 0;
}
```

Tyypinmuunnos (cast) tyyppiin `unsigned char` kohdassa `isspace((unsigned char)*end)` on merkityksellinen. Funktiot kirjastoista `<ctype.h>` on määritelty arvoille tyypiltään `EOF` tai sellaisille arvoille, jotka ovat edustettavissa tyyppinä `unsigned char`; satunnaisen negatiivisen `char` arvon välittäminen ei ole turvallista.

Tämä ohjelma hyväksyy kymmenjärjestelmän numerot väliltä `1`—`100`, valinnaisesti ympäröitynä välilyönneillä. Se hylkää tyhjän syötteen, ei‑numeraaliset syötteet, sekoitetun syötteen kuten `12cats`, valitun alueen ulkopuoliset numerot ja rivit, jotka ovat liian pitkiä kiinteälle puskurille. Nämä ovat sovelluskohtaisia sääntöjä, eivät yleispäteviä: tuleva ohjelma voi valita toisen alueen tai syötemuodon, mutta se tarvitsee silti eksplisiittiset tarkistukset.

## 6. Työstetty esimerkki: tarkistettu oppituntien lukumäärä

Seuralaisohjelma alla lukee oppituntien määrän, validoi sen, valitsee yksikkö‑ tai monikkovastauksen, ja käyttää `for`‑silmukkaa laskeakseen kertauspisteet.

```c
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    INPUT_CAPACITY = 64,
    MIN_LESSONS = 1,
    MAX_LESSONS = 100,
    REVIEW_INTERVAL = 5
};

static int parse_lesson_count(const char *text, int *result)
{
    char *end = NULL;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);

    if (end == text || errno == ERANGE) {
        return 0;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }

    if (*end != '\0' || value < MIN_LESSONS || value > MAX_LESSONS) {
        return 0;
    }

    *result = (int)value;
    return 1;
}

static int discard_remainder_of_line(void)
{
    int character;

    do {
        character = getchar();
    } while (character != '\n' && character != EOF);

    return character == EOF && ferror(stdin) ? 0 : 1;
}

int main(void)
{
    char input[INPUT_CAPACITY];
    int lesson_count;
    int checkpoint_count = 0;

    if (fputs("How many lessons (1-100)? ", stdout) == EOF) {
        return 1;
    }

    if (fgets(input, sizeof input, stdin) == NULL) {
        if (ferror(stdin)) {
            fputs("Input error.\n", stderr);
        } else {
            fputs("No input received.\n", stderr);
        }
        return 1;
    }

    if (strchr(input, '\n') == NULL) {
        if (!discard_remainder_of_line()) {
            fputs("Input error while discarding a long line.\n", stderr);
        } else {
            fputs("Input is too long.\n", stderr);
        }
        return 1;
    }

    if (!parse_lesson_count(input, &lesson_count)) {
        fputs("Enter one whole number from 1 to 100.\n", stderr);
        return 1;
    }

    if (lesson_count == 1) {
        if (printf("Plan: 1 lesson\n") < 0) {
            return 1;
        }
    } else {
        if (printf("Plan: %d lessons\n", lesson_count) < 0) {
            return 1;
        }
    }

    for (int day = 1; day <= lesson_count; ++day) {
        if (day % REVIEW_INTERVAL == 0) {
            ++checkpoint_count;
        }
    }

    if (printf("Review checkpoints: %d\n", checkpoint_count) < 0) {
        return 1;
    }

    if (lesson_count == 1) {
        if (fputs("Start with one focused session.\n", stdout) == EOF) {
            return 1;
        }
    } else {
        if (fputs("Make the plan in short, repeatable blocks.\n", stdout) == EOF) {
            return 1;
        }
    }

    return 0;
}
```

Käännä ja suorita seuralaisohjelma (liitännäinen tiedosto) täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-002-control-flow-and-simple-input \
  examples/c/c-002-control-flow-and-simple-input.c
printf '12\n' | ./c-002-control-flow-and-simple-input
```

Odotettu tulostus:

```text
How many lessons (1-100)? Plan: 12 lessons
Review checkpoints: 2
Make the plan in short, repeatable blocks.
```

Kokeile virheellistä syötettä erikseen:

```sh
printf '12cats\n' | ./c-002-control-flow-and-simple-input
```

Ohjelman tulisi päättyä ei‑nollalla tilakoodilla ja kirjoittaa tämä virheilmoitus standardivirtaan:

```text
Enter one whole number from 1 to 100.
```

## 7. Ohjattu harjoitus

Vastaa ennen kuin luet paljastusosion.

1. Mikä on ero `=` ja `==` välillä?
2. Minkä arvon `17 % 5` tuottaa?
3. Kuinka monta kertauspistettä silmukka tuottaa arvolla `lesson_count == 20`?
4. Miksi ohjelma käyttää `fgets` ennen `strtol` sen sijaan, että se muuntaisi suoraan `int`:ksi?
5. Mikä validointitarkistus hylkää `12cats`?
6. Miksi `value < MIN_LESSONS || value > MAX_LESSONS` tarkistetaan ennen `(int)value`?
7. Mitä tapahtuu, kun syöterivi on pidempi kuin kiinteä syötepuskuri?

## 8. Itsenäinen harjoitus

Luo `exercise.c` seuralaisohjelmatiedoston viereen. Käytä samaa tarkistettua rivisyötettä ja `strtol`‑mallia, mutta hyväksy harjoitussessioiden määrä väliltä `1`—`31`. Tulosta `One session scheduled.` kun `1`, muuten tulosta `N sessions scheduled.`. Käytä `for`‑silmukkaa laskeaksesi joka kolmannen session kertausistunnoksi.

Ohjelmasi on hylättävä nämä syötteet ei‑nollalla poistumistilakoodilla:

```text
0
32
7days
<an empty line>
```

Testaa vähintään `1`, `3` ja `7` kelvollisina syötteinä. Älä kierrä validointia tyyppimuunnoksella, jossa raakamuunnoksen `strtol` tulos kastetaan suoraan `int`:ksi.

## 9. Tarkista sanitisaattoreilla

Suorita deterministinen testi tässä repossa:

```sh
./tests/c-002-control-flow-and-simple-input.sh
```

Sitten rakenna AddressSanitizerin ja UndefinedBehaviorSanitizerin kanssa, kun ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-002-sanitized \
  examples/c/c-002-control-flow-and-simple-input.c
printf '12\n' | ./c-002-sanitized
printf '12cats\n' | ./c-002-sanitized
```

Kelvollinen syöte pitäisi tuottaa odotetun suorituksen. Virheellinen syöte pitäisi tuottaa odotetun diagnostiikan ja ei‑nollan poistumistilakoodin ilman sanitisaattorin raportteja. Sanitisaattorit ovat hyödyllisiä tarkistuksia, mutta ne eivät korvaa eksplisiittistä syötteen validointia lähdekoodissa.

## 10. Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt vastaamaan jokaiseen kysymykseen ilman taaksepäin kurkistamista.

1. Kuvaile kolmea osaa `for`‑lauseessa.
2. Selitä, miksi `for`‑silmukka, jolla on näkyvä raja, on helpompi tarkistaa kuin jäsentelemätön ikuinen silmukka.
3. Luettele kaksi erilaista tarkistusta, jotka tarvitaan `strtol` jälkeen: "ei numeroita" ja "numeroarvo on `long` alueen ulkopuolella".
4. Selitä, miksi ohjelma tarkistaa hyväksytyn alueen ennen kaventamista tyypistä `long` tyypiksi `int`.
5. Anna yksi syy hylätä rivi, joka ylittää kiinteään syötepuskuriin varatun koon.
6. Kirjoita virallinen venäläinen ilmaisu "Where are you from?" Russian 002:sta.

## Paljastus: vastaukset ja malliratkaisu

1. `=` asettaa arvon; `==` vertaa kahta arvoa.
2. `17 % 5` on `2`.
3. `20`ssa on kertauspisteet kohdissa `5`, `10`, `15` ja `20`, joten tulos on `4`.
4. `fgets` antaa ohjelmalle rajatun, nollaterminoitu merkkijonon, jonka se voi tarkastaa kokonaan ennen muunnosta. Se mahdollistaa tarkistukset ylimääräiselle tekstille ja riville, joka oli liian pitkä puskurille.
5. `strtol` jälkeen lopetusosoitin pysähtyy ensimmäisen numeron ulkopuolisen merkin kohdalle. Kun perässä olevat välilyönnit on ohitettu, `*end != '\0'` hylkää `12cats`.
6. Hyväksytyn alueen tarkistus varmistaa, että arvo täyttää ohjelman vaatimukset ennen kuin se kaventuu tyypiksi `int`. Onnistunut `strtol` tulos voi mahtua tyyppiin `long` mutta silti olla sovelluksen tai kohdetyypin kannalta sopimaton.
7. Seuralaisohjelma heittää pois rivin jäljelle jäävät merkit, raportoi `Input is too long.` ja poistuu epäonnistuneena. Se ei käsittele katkaistua etuliitettä täydellisenä numerona.
8. Russian 002:sta: **Откуда вы?**

Malliydin itsenäiseen harjoitukseen on:

```c
for (int session = 1; session <= session_count; ++session) {
    if (session % 3 == 0) {
        ++review_count;
    }
}
```

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjän kieleen: **Russian 003 — kieliopillinen suku, perussubstantiivien mallit ja tutuiksi tulevat esineet.**

## Lähteet

[1]: https://en.cppreference.com/w/c/io/fgets "fgets — cppreference.com"
[2]: https://en.cppreference.com/w/c/string/byte/strtol "strtol, strtoll — cppreference.com"
[3]: https://en.cppreference.com/w/c/language/for "for‑silmukka — cppreference.com"
