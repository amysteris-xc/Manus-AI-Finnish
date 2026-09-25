# C 003 — Funktiot, lähtöparametrit ja eksplisiittiset virhepalautukset

**Arvioitu työaika:** 25–35 minuuttia
**Esivaatimukset:** [C 001 — Build, Types, and Output](c-001-build-types-output.md) ja [C 002 — Expressions, Decisions, Loops, and Checked Input](c-002-control-flow-and-simple-input.md)
**Seuraohjelma:** [`examples/c/c-003-functions-and-error-returns.c`](../../examples/c/c-003-functions-and-error-returns.c)
**Deterministinen testi:** [`tests/c-003-functions-and-error-returns.sh`](../../tests/c-003-functions-and-error-returns.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopuksi osaat jakaa pienen C-ohjelman funktioihin, joilla on selkeät vastuut, lukea funktion signatuuria, välittää arvoja parametreina, käyttää kapeaa lähtöparametrikuvioita silloin kun funktion täytyy tuottaa sekä laskettu arvo että onnistumis-/epäonnistumistila, sekä välittää nimettyjä virhetiloja takaisin `main`.

## Alkulämmittely

Ilman C 002:n avaamista vastaa näihin kysymyksiin.

1. Miksi edellinen oppitunti lukee rajatun rivin käyttäen `fgets` ennen `strtol` kutsumista?
2. Mikä tarkistus havaitsee, että `strtol` ei muuntanut yhtään numeroa?
3. Mikä ehto hylkää jälkimmäisen ei-valkoisen tekstin, kuten `12cats`?
4. Mitä `return 1;` perinteisesti viestii aiemmissa esimerkeissä?

Sitten muista yksi kohde Russian 003: mikä pronomini vastaa neutrisukua olevaa substantiivia **окно**? Vastaukset ovat spoilereissa.

## 1. Funktio on sopimus, jolla on tarkka tehtävä

C-funktiolla on nimi, parametrit, runko ja valinnainen paluuarvo. Parametrit vastaanottavat arvoja kutsujalta; `return` -lauseke lopettaa funktion ja voi lähettää arvon takaisin kutsujalle.[1] [2]

```c
static int double_count(int count)
{
    return count * 2;
}
```

Signatuuri kertoo lukijalle, miten funktiota kutsutaan:

| Osa | Merkitys |
|---|---|
| `static` | Tämä apufunktio on yksityinen tälle lähdetiedostolle. |
| `int` nimen edellä | Funktio palauttaa `int`. |
| `double_count` | Funktion tarkoitukseen liittyvä nimi. |
| `int count` | Yksi syöteparametri, nimeltään `count`, tyypiltään `int`. |
| `return count * 2;` | Laskee tuloksen ja lähettää sen takaisin kutsujalle. |

Funktion tulisi tehdä yksi selkeä tehtävä. Funktio, joka on nimetty `parse_bounded_int`, tulisi jäsentää ja validoida rajatun kokonaisluvun. Funktio, joka on nimetty `write_plan`, tulisi kirjoittaa jo validiksi todetuksi suunnitelmaksi. Sen ei tulisi hiljaa päättää asiaan kuulumattomia politiikkoja, lukea toista syötelähdettä tai muuttaa piilotettua globaalia tilaa.

## 2. Käytä nimettyjä tilakoodeja odotetuille virheille

C-funktioiden täytyy usein erottaa onnistuminen odotetusta, toipuvasta epäonnistumisesta. Tässä oppitunnissa käytetään pientä enumia selittämättömien numeeristen paluuarvojen sijaan.

```c
typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_OUTPUT_FAILURE = 2
} Status;
```

`Status` -tulos antaa kutsujalle selkeän kysymyksen vastattavaksi: onnistuiko funktio? Kutsujan täytyy tarkistaa tulos välittömästi ja päättää, mitä seuraavaksi tehdään.

```c
Status status = parse_bounded_int(text, 1, 100, &count);

if (status != STATUS_OK) {
    return EXIT_FAILURE;
}
```

Älä käytä kelvollista numeerista tulosta, kuten `0`, sekä datana että virhesignaalina, ellei rajapinnalla ole erillistä, yksiselitteistä keinoa erottaa ne. Esimerkiksi suunnitelmassa voi laillisesti olla nolla tarkastuspistettä. Tilaarvo, joka on erillään lasketusta tuloksesta, välttää epäselvyyttä.

## 3. Yksi tulos plus yksi tila: ensimmäinen lähtöparametri

Oletetaan, että funktion täytyy laskea summa ja kertoa samalla, oliko laskenta validi. Pelkkä `int` palauttaminen on epäselvä, koska summa itse voi olla `0`. Yksi yksinkertainen C-kuvio on:

- Palauta `Status` onnistumiselle tai epäonnistumiselle.
- Vastaanota `int *result` parametri, joka osoittaa kutsujan omistamaan tallennustilaan.
- Kirjoita laskettu arvo `*result` vain sen jälkeen, kun kaikki validoinnit ovat onnistuneet.

```c
static Status calculate_total_minutes(int sessions,
                                      int minutes_per_session,
                                      int *result)
{
    if (result == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (sessions < 1 || minutes_per_session < 1) {
        return STATUS_INVALID_ARGUMENT;
    }

    *result = sessions * minutes_per_session;
    return STATUS_OK;
}
```

`*` kohdassa `int *result` kertoo, että `result` on osoitin tyyppiin `int`. `*` kohdassa `*result = ...` pääsee kutsujan `int` käsiksi tämän osoittimen kautta. Funktion kutsussa `&total_minutes` antaa kutsujan paikallisen muuttujan osoitteen.

```c
int total_minutes;
Status status = calculate_total_minutes(4, 25, &total_minutes);
```

Tämä on tarkoituksellisesti kapea ensimmäinen käyttötapaus osoittimille. Siihen ei liity dynaamista muistia, osoinlaskentaa tai raakabittejä. Nämä aiheet tulevat myöhemmin. Tällä hetkellä seuraa yhtä sääntöä: vain dereferensoi osoitinta sen jälkeen, kun rajapinta on vaatinut ja tarkistanut, ettei se ole `NULL`.

## 4. Suojaa laskenta ennen sen suorittamista

Ennen kuin kerrot kaksi positiivista `int` arvoa, tarkista, ylikirjoittuuko tulo `INT_MAX`.

```c
if (sessions > INT_MAX / minutes_per_session) {
    return STATUS_INVALID_ARGUMENT;
}

*result = sessions * minutes_per_session;
```

Jakaminen on turvallista tässä, koska funktio on jo vaatinut `minutes_per_session >= 1`. Tämä on pieni esimerkki esiehtotarkistuksesta: varmista ehdot, jotka tekevät seuraavasta operaatiosta turvallisen, ennen sen suorittamista.

Seuraohjelma käyttää myös sovelluskohtaisia rajoja: `1–100` sessiota ja `1–180` minuuttia per sessio. Nämä rajat tekevät ohjelman odotukset eksplisiittisiksi ja pitävät demonstraatioarvot kaukana `INT_MAX`. Ylivuototarkistus on silti arvokas, koska funktioiden rajapintojen pitäisi puolustaa omia aritmeettisia oletuksiaan, eikä luottaa pelkästään nykyiseen kutsujaan.

## 5. Virheen välittäminen tarkoittaa, että kutsuja päättää

Alentason apu voi tunnistaa epäonnistumisen, mutta sillä ei usein ole tarpeeksi kontekstia päättää, miten koko ohjelman pitäisi reagoida. Se palauttaa tilan omalle kutsujalleen. Kutsuja tarkistaa sen, lisää kontekstia jos se on hyödyllistä, ja palauttaa epäonnistumisen omalle kutsujalleen jos työ ei voi jatkua.

```c
status = calculate_total_minutes(sessions, minutes_per_session, &total_minutes);
if (status != STATUS_OK) {
    (void)write_error("Cannot calculate total planned minutes.\n");
    return EXIT_FAILURE;
}
```

Tätä kutsutaan eksplisiittiseksi virheiden välittämiseksi. Yhtään epäonnistumista ei jätetä hiljaiseksi. Funktio main muuntaa epäonnistuneen aputoiminnon ei-nollaiseen prosessin poistumistilaan.

Lähtöapu seuraa samaa kuviota. `fputs` palauttaa ei-negatiivisen arvon onnistuksessa ja `EOF` epäonnistumisessa.[3] Kääre muuttaa tämän kirjastokohtaisen säännön tämän ohjelman `Status` sopimukseksi.

```c
static Status write_error(const char *message)
{
    if (message == NULL || fputs(message, stderr) == EOF) {
        return STATUS_OUTPUT_FAILURE;
    }

    return STATUS_OK;
}
```

`(void)` ennen `write_error` -kutsua tarkoittaa, että ohjelmalla ei ole enää hyödyllistä palautuspolkua, jos diagnostinen kirjoittaminen myös epäonnistuu. Ohjelma kuitenkin palauttaa `EXIT_FAILURE`; se ei väitä valheellisesti onnistumista.

## 6. Työstetty esimerkki: modulaarinen sessiosuunnitelman laskin

Seuraohjelma ottaa vastaan kaksi komentoriviarvoa: sessioiden lukumäärän ja minuutit per sessio. Se jäsentää kummankin arvon tarkistetulla `strtol` -kuviolla C 002:sta, laskee kokonaismäärän lähtöparametrin kautta ja kirjoittaa suunnitelman vain sen jälkeen, kun kaikki aiemmat vaiheet onnistuvat.

```c
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_OUTPUT_FAILURE = 2
} Status;

enum {
    MIN_SESSIONS = 1,
    MAX_SESSIONS = 100,
    MIN_MINUTES_PER_SESSION = 1,
    MAX_MINUTES_PER_SESSION = 180
};

static Status parse_bounded_int(const char *text, int minimum, int maximum,
                                int *result)
{
    char *end = NULL;
    long value;

    if (text == NULL || result == NULL || minimum > maximum) {
        return STATUS_INVALID_ARGUMENT;
    }

    errno = 0;
    value = strtol(text, &end, 10);

    if (end == text || errno == ERANGE) {
        return STATUS_INVALID_ARGUMENT;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }

    if (*end != '\0' || value < minimum || value > maximum) {
        return STATUS_INVALID_ARGUMENT;
    }

    *result = (int)value;
    return STATUS_OK;
}

static Status calculate_total_minutes(int sessions, int minutes_per_session,
                                      int *result)
{
    if (result == NULL || sessions < MIN_SESSIONS ||
        minutes_per_session < MIN_MINUTES_PER_SESSION) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (sessions > INT_MAX / minutes_per_session) {
        return STATUS_INVALID_ARGUMENT;
    }

    *result = sessions * minutes_per_session;
    return STATUS_OK;
}

static Status write_plan(FILE *stream, int sessions, int minutes_per_session,
                         int total_minutes)
{
    if (stream == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (fprintf(stream, "Sessions: %d\n", sessions) < 0 ||
        fprintf(stream, "Minutes per session: %d\n", minutes_per_session) < 0 ||
        fprintf(stream, "Total planned minutes: %d\n", total_minutes) < 0) {
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

int main(int argc, char *argv[])
{
    int sessions;
    int minutes_per_session;
    int total_minutes;
    Status status;

    if (argc != 3) {
        (void)write_error("Usage: c-003-functions-and-error-returns SESSIONS MINUTES_PER_SESSION\n");
        return EXIT_FAILURE;
    }

    status = parse_bounded_int(argv[1], MIN_SESSIONS, MAX_SESSIONS, &sessions);
    if (status != STATUS_OK) {
        (void)write_error("Invalid session count. Use 1 through 100.\n");
        return EXIT_FAILURE;
    }

    status = parse_bounded_int(argv[2], MIN_MINUTES_PER_SESSION,
                               MAX_MINUTES_PER_SESSION, &minutes_per_session);
    if (status != STATUS_OK) {
        (void)write_error("Invalid minutes per session. Use 1 through 180.\n");
        return EXIT_FAILURE;
    }

    status = calculate_total_minutes(sessions, minutes_per_session,
                                     &total_minutes);
    if (status != STATUS_OK) {
        (void)write_error("Cannot calculate total planned minutes.\n");
        return EXIT_FAILURE;
    }

    status = write_plan(stdout, sessions, minutes_per_session, total_minutes);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

Käännä ja suorita sitoutettu seuraohjelmatiedosto täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-003-functions-and-error-returns \
  examples/c/c-003-functions-and-error-returns.c
./c-003-functions-and-error-returns 4 25
```

Odotettu tuloste:

```text
Sessions: 4
Minutes per session: 25
Total planned minutes: 100
```

Kokeile virheellistä arvoa:

```sh
./c-003-functions-and-error-returns 0 25
```

Ohjelman tulisi poistua epäonnistumisella ja kirjoittaa tämä diagnostiikka standardivirtaan:

```text
Invalid session count. Use 1 through 100.
```

## 7. Ohjattu harjoitus

Vastaa ennen spoiler-osion lukemista.

1. Missä `Status parse_bounded_int(const char *text, int minimum, int maximum, int *result)` parametreista ovat syötteitä ja mikä parametri vastaanottaa lähtöarvon?
2. Miksi `calculate_total_minutes` palauttaa `Status` sen sijaan, että se palauttaisi summan suoraan?
3. Mitä `&total_minutes` antaa funktion kutsulle?
4. Miksi `result == NULL` täytyy tarkistaa ennen `*result = ...`?
5. Mikä funktio vastaa onnistuneen suunnitelman muotoilusta?
6. Mikä funktio päättää, että virheellinen sessiomäärä saa ohjelman poistumaan epäonnistumisella?
7. Miksi laskenta tarkistaa `sessions > INT_MAX / minutes_per_session` ennen kertolaskua?

## 8. Itsenäinen tehtävä

Luo `exercise.c` seuraohjelmatiedoston viereen. Kirjoita kolme funktiota taukosuunnitelman laskentaa varten:

1. `parse_bounded_int` tai vastaava tarkistettu jäsentäjä, joka hyväksyy taukominuutit välillä `1`–`60`.
2. `calculate_weekly_minutes` joka vastaanottaa sessiot, minuutit per sessio ja lähtöosoittimen; sen täytyy hylätä null-lähtöosoitin ja suojata kertolasku ennen sen suorittamista.
3. `write_summary` joka vastaanottaa `FILE *` -virran ja kirjoittaa lyhyen suunnitelman, palauttaen tilan jos kirjoitus epäonnistuu.

Tee `main` jäsentämään kaksi komentoriviargumenttia: sessiot ja taukominuutit. Käytä nimettyjä tilaarvoja, tarkista jokainen apufunktion palautus välittömästi ja palauta `EXIT_FAILURE` kun suunnitelma ei voi jatkua. Testaa kelvollinen syöte kuten `5 10`, virheellinen teksti kuten `five 10`, alueen ulkopuolinen arvo kuten `5 61`, ja puuttuva argumentti.

## 9. Varmista sanitisaattoreilla

Suorita repositorion deterministinen testi:

```sh
./tests/c-003-functions-and-error-returns.sh
```

Sitten käännä ja suorita AddressSanitizerin ja UndefinedBehaviorSanitizerin kanssa, kun ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-003-sanitized \
  examples/c/c-003-functions-and-error-returns.c
./c-003-sanitized 4 25
./c-003-sanitized 0 25
```

Kelvollinen ajo tulostaa kolme suunnitelmariviä. Virheellinen ajo palauttaa ei-nollan ja tulostaa odotetun diagnostiikan, ilman sanitisaattoriraportteja. Sanitisaattorit auttavat löytämään tiettyjä ajoaikavirheitä, mutta lähdetason tilatarkastukset ja alueen tarkastukset ovat edelleen tarpeen.

## 10. Viimeinen itsearvio

Olet valmis seuraavaan oppituntiin, jos pystyt tekemään kaiken seuraavan ilman takaisin katsomista.

1. Selitä ero funktion parametrin ja argumentin välillä.
2. Kerro, miksi nimettyjen tilaarvojen käyttö voi olla selkeämpää kuin pelkän `0` tai `1` käyttäminen.
3. Kuvaile `calculate_total_minutes` käyttämä lähtöparametrikuvio.
4. Selitä, miksi `main` tarkistaa jokaisen funktion paluuarvon välittömästi.
5. Tunnista ehto, joka suojaa kertolaskun allekirjoitetun kokonaisluvun ylivuotolta.
6. Kerro, mikä venäläinen pronomini vastaa **окно**.

## Spoileri: vastaukset ja mallivastaus

1. Syöteparametrit ovat `text`, `minimum`, ja `maximum`. `result` on lähtöparametri; onnistumisen jälkeen funktio tallentaa jäsennellyn `int` sen kautta.
2. Summa itsessään on dataa ja voi olla kelvollinen numeerinen arvo. Eri `Status` palauttaminen tekee onnistumisen tai epäonnistumisen yksiselitteiseksi, kun taas lähtöparametri vastaanottaa summan vain onnistuksessa.
3. `&total_minutes` antaa kutsun yhteyteen kutsujan paikallisen `int total_minutes` muuttujan osoitteen.
4. Null-osoittimen dereferensointi on virheellinen. Ensin tarkistaminen varmistaa, että funktio kirjoittaa vain kelvolliseen lähtöpaikkaan.
5. `write_plan` muotoilee onnistuneen tulostuksen.
6. `main` päättää, että virhe estää työn jatkumisen, ja palauttaa `EXIT_FAILURE` sen jälkeen kun jäsentäjäraportti ilmoittaa epäonnistumisesta.
7. Ehto todistaa, että `sessions * minutes_per_session` mahtuu tyyppiin `int` ennen kertolaskun suorittamista.
8. **окно** on neutri, joten sen pronomini on **оно**.

Minimaalinen tilantarkistuksen malli on:

```c
status = helper(...);
if (status != STATUS_OK) {
    return EXIT_FAILURE;
}
```

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjään: **Russian 004 — sukujen yhteensopivuus yleisissä adjektiiveissa ja ytimekkäät esinekuvaukset.**

## Viitteet

[1]: https://en.cppreference.com/w/c/language/functions "Funktiot — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/return "return-lause — cppreference.com"
[3]: https://en.cppreference.com/w/c/io/fputs "fputs — cppreference.com"
