# C 001 — Kääntäminen, tyypit ja tulostus

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esitiedot:** Ei vaatimuksia
**Harjoitusohjelma:** [`examples/c/c-001-build-types-output.c`](../../examples/c/c-001-build-types-output.c)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat luoda pienen hosted C -ohjelman, selittää `#include`, `main`, lauseiden, muuttujien ja `return` roolit, erottaa useita perustavanlaatuisia C-tyyppejä, kääntää tiukoilla diagnostiikkalipuilla ja tuottaa ennustettavaa tekstiä käyttäen `printf`.

## Kertausharjoitus

Edellinen oppitunti oli Russian 001. Ilman että katsot, muista yksi muodollinen tervehdys, yksi epämuodollinen jäähyväinen ja kyrillinen äänne **В**. Mallivastaus on: **Здравствуйте**, **Пока**, ja **В** vastaa suunnilleen *v*-äännettä. Tämä lyhyt kertaaminen estää vuorottelevan opetuksen hajaantumasta kahdeksi toisistaan erilliseksi kurssiksi.

Tätä C-opetusta ei ole aiemmin käsitelty. Ennen kuin jatkat, sano lause ääneen: "C-lähdetiedosto on tavallista tekstiä; kääntäjä muuntaa sen ohjelmaksi, jota käyttöjärjestelmä voi suorittaa."

## 1. Pienin käytännöllinen C-ohjelma

Hosted C -ohjelma alkaa suorittautua funktiosta nimeltä `main`.[1] Käytämme muotoa `int main(void)`: `int` kertoo, että funktio palauttaa kokonaislukuarvon ympäristölle, kun taas `void` ilmaisee, ettei tässä versiossa ole parametreja.

```c
#include <stdio.h>

int main(void)
{
    printf("Hello, C!\n");
    return 0;
}
```

Lue ohjelma ylhäältä alas.

| Osa | Tarkoitus |
|---|---|
| `#include <stdio.h>` | Tekee `printf` julistuksen saataville ennen kuin sitä kutsutaan. |
| `int main(void)` | Määrittää ohjelman aloitusfunktion tässä oppitunnissa. |
| `{` ja `}` | Merkitsevät funktion rungon. |
| `printf("Hello, C!\n");` | Kutsuu muotoiltua tulostusfunktiota ja päättää rivin rivinvaihto-escapeilla `\n`. |
| `return 0;` | Ilmoittaa konvention mukaisesti onnistuneesta suorituksesta; nolla tarkoittaa onnistumista hosted-ympäristössä.[1] |

> Sääntö: C:ssä julistus kertoo kääntäjälle, mitä tunniste tarkoittaa ennen kuin sitä käytetään. `<stdio.h>` mukaan ottaminen ei ole vapaaehtoista koristeellisuutta: `printf` kutsuminen ilman sen julistusta voi johtaa virheellisiin diagnostiikkaviesteihin tai määrittelemättömään käyttäytymiseen.

## 2. Käännä ohjelma tiukoilla diagnostiikoilla

Tallenna lähde nimellä `hello.c`. Terminaalissa käännä se komennolla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic -o hello hello.c
```

Liput tarkoittavat:

| Lippu | Tarkoitus |
|---|---|
| `-std=c17` | Asettaa C17-kielistandardin käyttöön. |
| `-Wall` | Ota käyttöön hyödylliset varoitukset perusasetuksella. |
| `-Wextra` | Ota käyttöön lisävaroituksia, mukaan lukien `-Wall`. |
| `-Wpedantic` | Pyydä diagnostiikkaa rakenteista, jotka ovat valitun standardin ulkopuolella. |
| `-o hello` | Nimeää tuotettavan suoritustiedoston `hello`. |

Aja ohjelma komennolla:

```sh
./hello
```

Odotettu tuloste:

```text
Hello, C!
```

Käsittele varoituksia tutkittavana työnä, ei koristeellisena päätetulostena. Puhtaan käännöksen saaminen ei todista ohjelman oikeellisuutta, mutta se paljastaa monia virheitä ennen ohjelman suoritusta.

## 3. Perustyypit: valitse ensin arvon laji

Tyyppi kertoo C:lle, miten arvo esitetään ja mitkä operaatiot ovat merkityksellisiä. Älä yritä muistaa kaikkia numeerisia rajoja tänään. Useiden perustyyppien tarkka arvoalue voi vaihdella alustoittain; myöhemmissä oppitunneissa käytetään `<limits.h>`, kiinteäleveydellisiä tyyppejä kuten `uint8_t` ja `uint32_t`, sekä eksplisiittisiä aluerajoituksia, kun tarkka leveys on tarpeen.

| Tyyppi | Ensisijainen käyttö | Esimerkki |
|---|---|---|
| `char` | Yksi merkki tai pieni kokonaislukukoodiyksikkö | `char initial = 'C';` |
| `int` | Yleinen kokonaisluku laskemiseen tai tilan kuvaamiseen | `int lesson_number = 1;` |
| `double` | Liukulukuarvio, usein mitattuihin suureisiin | `double minutes = 30.0;` |
| `void` | Ei arvoa, tai eksplisiittisesti tyhjä paramettilista funktiossa `main(void)` | `int main(void)` |

Käytä ilman suffiksia olevaa kokonaislukuliteraalia, kuten `1`, yhdessä `int` kun se sopii. Käytä desimaalipistettä, kuten `30.0`, kun tarkoitat liukulukuista literaalia. Myöhemmin, kun arvot edustavat tavuja, tiedostokokoja tai protokollakenttiä, oikean tyypin valinta on turvallisuusvaatimus eikä pelkkä tyylivalinta.

## 4. Tulostus käyttäen `printf`

`printf` on julistettu tiedostossa `<stdio.h>` ja kirjoittaa muotoiltua lähtöä standarditulostusvirtaan.[2] Sen ensimmäinen argumentti on formattimerkkijono. Teksti formattimerkkijonon sisällä tulostetaan sellaisenaan, paitsi että `%` aloittavat muunnosspeksifikaatiot, jotka kertovat `printf` miten tulostaa lisäargumentteja.

| Arvon tyyppi tässä oppitunnissa | Muunnosspeksifikaatio | Esimerkki |
|---|---|---|
| `int` | `%d` | `printf("Lesson %d\n", lesson_number);` |
| `double` | `%f` tai tarkkuutta ohjaava muoto kuten `%.0f` | `printf("%.0f minutes\n", minutes);` |
| `char` | `%c` | `printf("%c\n", initial);` |

`%.0f` tarkoittaa "tulosta liukuluku ilman desimaalien jälkeisiä numeroita." `f` speksifikaatio odottaa `double` -tyyppistä argumenttia. On tärkeää, että kukin formaattispesifikaatio vastaa argumentin tyyppiä: virheellinen vastaavuus tekee käyttäytymisestä määrittelemätöntä.[2]

Toistaiseksi käytä itse kirjoitettuja, kirjaimellisia formattimerkkijonoja. Älä koskaan anna epäluotettavaa tekstiä formattiparametrina. Myöhemmät syöte-/tulostus- ja turvallisuusoppitunnit selittävät formaattimerkkijonahaavoittuvuudet ja turvallisemmat tavat tulostaa mielivaltaista tekstiä.

## 5. Malliesimerkki: pieni opintosuunnitelmaraportti

Harjoitusohjelma tulostaa kolme arvoa käyttäen edellä mainittuja tyyppejä.

```c
#include <stdio.h>

int main(void)
{
    const int lesson_number = 1;
    const double planned_minutes = 30.0;
    const char track_initial = 'C';

    if (printf("Track: %c\n", track_initial) < 0) {
        return 1;
    }

    if (printf("Lesson: %d\n", lesson_number) < 0) {
        return 1;
    }

    if (printf("Planned study time: %.0f minutes\n", planned_minutes) < 0) {
        return 1;
    }

    return 0;
}
```

`const` -kvalifikaattori tarkoittaa, ettei tämä ohjelma muokkaa kolmea nimettyä oliota alustamisen jälkeen. Jokainen `printf` -kutsu tarkistetaan, koska muotoiltu tulostus voi epäonnistua. Tässä pienessä esimerkissä `return 1;` antaa ympäristölle ei-nollan tilan, kun tulostus epäonnistuu. Normaali kulku päättyy `return 0;`.

Käännä ja suorita liitetty harjoitusohjelma täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-001-build-types-output \
  examples/c/c-001-build-types-output.c
./c-001-build-types-output
```

Odotettu tuloste:

```text
Track: C
Lesson: 1
Planned study time: 30 minutes
```

## 6. Ohjattu harjoitus

Vastaa ennen kuin luet paljastusosion.

1. Mikä otsake (header) pitää olla mukana ennen kuin ohjelma kutsuu `printf`?
2. Mitä `void` kertoo `int main(void)` tässä oppitunnissa?
3. Valitse paras tyyppi kullekin arvolle: oppituntien lukumäärä `12`, arvio `12.5` minuuttia, ja yksittäinen kirjain `'R'`.
4. Kirjoita `printf` -kutsu, joka tulostaa `Lesson: 12` ja rivinvaihdon, olettaen että `lesson_number` on `int` joka sisältää arvon `12`.
5. Tunnista virhe: `printf("Minutes: %d\n", planned_minutes);` kun `planned_minutes` on tyyppi `double`.

## 7. Itsenäinen harjoitus

Luo uusi tiedosto nimeltä `exercise.c` harjoitusohjelman viereen. Kirjoita ohjelma, joka määrittelee nämä kolme `const` muuttujaa:

```c
const int study_day = 2;
const double review_minutes = 15.5;
const char language_initial = 'R';
```

Tulosta täsmälleen tämä ulostulo, näyttäen `review_minutes` arvon yhdellä desimaalilla:

```text
Language: R
Study day: 2
Review: 15.5 minutes
```

Käännä se käyttäen osiossa 2 annettua tiukkaa komentoa. Muuta sitten tahallisesti `%c` joka tulostaa `language_initial` muotoon `%d`, rakenna uudelleen ja tarkkaile eroa. Palauta `%c` sen jälkeen. Tyyppi voidaan silti välittää tavalla, joka tuottaa jonkinlaista ulostuloa, mutta tarkoitettu esitys on merkki, joten `%c` on oikea spesifikaatio.

## 8. Tarkista sanitisaattoreilla

Pienessä ohjelmassa ilman dynaamista muistia sanitisaattorit eivät korvaa huolellista tarkastelua. Ne ovat silti hyödyllinen työkalu. Jos kääntäjäsi tukee niitä, rakenna harjoitusohjelma komennolla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-001-sanitized \
  examples/c/c-001-build-types-output.c
./c-001-sanitized
```

Odotettu tuloste on samat kolme riviä kuin yllä, ilman sanitisaattoridiagnostiikkaa. Jos kääntäjä ilmoittaa, että jokin sanitisaattorivaihtoehto ei ole käytettävissä, kirjaa tuo rajoitus ja jatka tiukalla, ei-sanitisoidulla käännöksellä. Älä poista varoituksia pelkästään tehdessäsi käännöksestä näennäisesti puhtaan.

## 9. Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit vastata jokaiseen kysymykseen ilman, että katsot taaksepäin.

1. Miksi ohjelma, joka käyttää `printf`, sisältää `<stdio.h>`?
2. Mikä komento kääntää tiedoston nimeltä `hello.c` C17:ksi hyödyntäen perusasetusvaroituksia?
3. Kumpi tyyppi sopii paremmin kokonaislukuiselle oppituntien määräluvulle: `int` vai `double` ?
4. Mikä muunnosspeksifikaatio vastaa `int` tässä oppitunnissa?
5. Minkä tilan `return 0;` perinteisesti ilmoittaa funktiosta `main` ?
6. Miksi `printf("%d\n", some_double);` on virheellinen?

## Paljastus: vastaukset ja malliratkaisu

1. `<stdio.h>` julistaa `printf` ennen funktion kutsumista.
2. `cc -std=c17 -Wall -Wextra -Wpedantic -o hello hello.c`
3. `int`.
4. `%d`.
5. Onnistunut suoritus.
6. `%d` odottaa `int`, kun taas `some_double` on `double`. Oikea valinta tässä on liukuluvun muunnos, kuten `%f` tai `%.1f`.

Mallirivi ohjatun harjoituksen kohtaan 4 on:

```c
printf("Lesson: %d\n", lesson_number);
```

Malliratkaisu itsenäiseen harjoitukseen on:

```c
#include <stdio.h>

int main(void)
{
    const int study_day = 2;
    const double review_minutes = 15.5;
    const char language_initial = 'R';

    printf("Language: %c\n", language_initial);
    printf("Study day: %d\n", study_day);
    printf("Review: %.1f minutes\n", review_minutes);
    return 0;
}
```

## Seuraava oppitunti

Seuraava ajastettu oppitunti palaa venäjään: **Russian 002 — tärkeät kyrilliset äännemallit, painotuksen tuntemus ja laajemmat esittelyt.**

## Viitteet

[1]: https://en.cppreference.com/w/c/language/main_function "Pääfunktio — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fprintf "printf, fprintf, sprintf, snprintf — cppreference.com"
