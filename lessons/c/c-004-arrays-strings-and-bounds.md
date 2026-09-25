# C 004 — Kiinteän koon taulukot, C-merkkijonot ja indeksi-rajat

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** [C 001 — Build, Types, and Output](c-001-build-types-output.md), [C 002 — Expressions, Decisions, Loops, and Checked Input](c-002-control-flow-and-simple-input.md), ja [C 003 — Functions, Output Parameters, and Explicit Error Returns](c-003-functions-and-error-returns.md)
**Seurantaohjelma:** [`examples/c/c-004-arrays-strings-and-bounds.c`](../../examples/c/c-004-arrays-strings-and-bounds.c)
**Deterministinen testi:** [`tests/c-004-arrays-strings-and-bounds.sh`](../../tests/c-004-arrays-strings-and-bounds.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin lopussa osaat määritellä kiinteän kokoisen taulukon, laskea sen alkiot käyttämällä `sizeof` kun se on vielä taulukko, selittää eron yleisen `char` -taulukon ja nollalla päättyvän C-merkkijonon välillä, varata tilan `\0`:lle, selvittää merkkijonon pituuden vasta kun terminointi on tiedossa, ja validoida käyttäjän antama indeksi ennen taulukkoalkion lukemista.

## Kertauslämmittely

Ilman että avaat aiempia C-oppitunteja, vastaa näihin kysymyksiin.

1. Miksi C 002 sijoittaa tekstin rajattuun puskuriiin ennen sen muuntamista `strtol`?
2. Mitä `end == text` tarkoittaa `strtol` -kutsun jälkeen?
3. Mitä `&total_minutes` välittää C 003:n laskentafunktiolle?
4. Miksi C 003 tarkistaa tilan ennen tulosarvon käyttämistä?
5. Venäjän oppitunnosta Russian 004: täydennä: **новый — новая — ___**.

Spoiler-osio sisältää mallivastaukset. Yritä muistaa ennen kuin tarkistat vanhan oppitunnin vastaukset.

## 1. Taulukoilla on kiinteä määrä alkioita

Taulukko sisältää saman tyyppisiä alkioita kiinteässä määrässä. Taulukkoindeksit alkavat kohdasta `0`, joten taulukolla, jossa on `N` alkiota, on kelvolliset indeksit `0` ... `N - 1`.

```c
char label[17];
```

Tämä varaa 17 `char` -alkiota, numeroituina `label[0]` ... `label[16]`. Taulukon ulkopuolella oleva indeksi `label[17]`, tai mikä tahansa negatiivinen indeksi, on taulukon ulkopuolella ja käyttäytyminen on määrittelemätöntä. C ei tee ajonaikaista rajatarkistusta tavallisessa taulukkoaccessissa; ohjelman on itse varmennettava raja ennen indeksointia.

Jos kyseessä on oikea taulukko samalla näkyvyysalueella, tämä lauseke laskee sen alkioiden lukumäärän:

```c
size_t capacity = sizeof label / sizeof label[0];
```

Koska `label` sisältää tyypin `char` alkioita, `sizeof label[0]` on `1`, joten `sizeof label` on tässäkin sen alkioiden lukumäärä. Jakomuoto on hyvä yleinen tapa, koska se toimii myös suurempia alkioita sisältävien taulukoiden kanssa, kuten `int scores[10]`.

> Taulukkosääntö: Jos indeksi tulee syötteestä tai laskennasta, varmista `index < element_count` ennen kuin käytät `array[index]`.

## 2. C-merkkijono on enemmän kuin `char` -taulukko

`char` -taulukko on yksinkertaisesti merkkikokoluokan olioiden taulukko. Se on **C-merkkijono** vain, kun se sisältää tekstinsä jälkeen nollamerkin, joka kirjoitetaan muodossa `\0`. Ensimmäinen `\0` osoittaa merkkijonon lopun.

```c
char word[] = "cat";
```

Tämä alustaja luo neljä alkiota:

| Indeksi | Tallennettu arvo |
|---|---|
| `0` | `'c'` |
| `1` | `'a'` |
| `2` | `'t'` |
| `3` | `\0` |

Näkyvä sana sisältää kolme merkkiä, mutta taulukko vaatii neljä alkiota. Merkkijonofunktiot, kuten `strlen`, etsivät ensimmäistä nollamerkkiä ja palauttavat sitä edeltävien merkkien määrän.[2] Funktion `strlen` kutsuminen taulukolle, josta ei tiedetä, että se on nollaterminoitu, johtaa määrittelemättömään käyttäytymiseen.[2]

Tavu- eli byte-puskuri ei ole automaattisesti C-merkkijono. Myöhemmissä oppitunneissa tämä ero on ratkaiseva: tavupuskuri voi sisältää nollaarvoja datana, kun taas C-merkkijono käsittelee ensimmäisen nollatavu-arvon merkkijonon päättymisenä. Älä käytä merkkijonofunktioita mielivaltaiselle tavupuskurille.

## 3. Varaa päättymismerkki ja hylkää katkaistut syötteet

Seurantaohjelma hyväksyy tunnuksen, jossa on 1–15 näkyvää merkkiä. Se määrittelee 17 alkiota sisältävän syöttötaulukon:

```c
enum {
    MAX_LABEL_LENGTH = 15,
    LABEL_CAPACITY = MAX_LABEL_LENGTH + 2
};

char label[LABEL_CAPACITY];
```

Miksi 17 alkiota? Täydellinen terminaalin kautta syötetty rivi voi sisältää:

| Tilankäyttö | Maksimi määrä |
|---|---:|
| Näkyvät tunnuksen merkit | 15 |
| Rivinvaihto, jonka `fgets` lukee | 1 |
| Nollapääte, jonka `fgets` kirjoittaa | 1 |
| Taulukon kokonaisalkiot | 17 |

`fgets` lukee enintään `count - 1` merkkiä ja kirjoittaa nollapäätteen onnistuneen syötön jälkeen.[1] Ohjelma välittää `sizeof label` sen sijaan, että toistaisi manuaalisesti `17`. Onnistuneen lukemisen jälkeen se tarkistaa, oliko rivinvaihto luettu. Jos rivinvaihtoa ei löytynyt, rivi ei mahtunut ohjelman hyväksymään muotoon, joten ohjelma kuluttaa lopun ja raportoi epäonnistumisen sen sijaan, että käsittelisi katkaistua etuliitettä täydellisenä syötteenä.

```c
if (strchr(label, '\n') == NULL) {
    /* Discard the rest of the input line, then reject it. */
}

label[strcspn(label, "\n")] = '\0';
```

Tämä korvaus poistaa rivinvaihdon vasta sen jälkeen, kun ohjelma on varmistanut, että rivinvaihto todella luettiin. Taulukko pysyy nollaterminoituina, joten `strlen(label)` on sen jälkeen turvallinen.

Älä pidä `strncpy` yleisenä korjauksena ylisuurelle syötteelle. Jos kopiointiraja saavutetaan ennen lähteen terminaattoria, `strncpy` ei lisää nollapäätettä.[3] Tässä oppitunnissa ylisuuri rivi hylätään syötteen rajalla sen sijaan, että kopioitaisiin lyhennetty etuliite ja muutettaisiin käyttäjän dataa hiljaisesti.

## 4. `sizeof` ja `strlen` vastaavat eri kysymyksiin

Nämä operaatiot käsittelevät eri kysymyksiä ja käyttävät eri tyyppejä.

| Lauseke | Merkitys | Tyyppi |
|---|---|---|
| `sizeof label` | Tämän taulukon tallennuskapasiteetti tavuina | `size_t` |
| `sizeof label / sizeof label[0]` | Tämän taulukon alkioiden lukumäärä | `size_t` |
| `strlen(label)` | Näkyvä merkkijonon pituus ennen ensimmäistä `\0` | `size_t` |

Esimerkiksi jos `char label[17]` sisältää `"cat"`, niin `sizeof label` on `17`, mutta `strlen(label)` on `3`. Olisi virheellistä tulkita kaikki 17 paikkaa tekstinä; vain kolme ensimmäistä merkkiä ennen `\0` kuuluvat C-merkkijonoon.

Yleinen ansa syntyy, kun taulukko lähetetään funktiolle. Parametrilistassa kuten `const char *label`, funktio vastaanottaa osoittimen, ei alkuperäistä taulukkorakennetta. Se ei voi käyttää `sizeof label` palauttaakseen kutsujan taulukon kapasiteettia. Lähetä pituus tai kapasiteetti eksplisiittisesti, kun funktio tarvitsee sitä.

## 5. Vahvista looginen raja, älä vain varauksen rajaa

Seurantaohjelma laskee pituuden turvallisen merkkijonon lukemisen jälkeen:

```c
size_t length = strlen(label);
```

Jos tunnus on `"cat"`, fyysisellä taulukolla on 17 alkiota, mutta loogiset merkki-indeksit ovat vain `0`, `1` ja `2`. Indeksi `3` sisältää päättyvän `\0`; sitä seuraavat indeksit eivät kuulu syötteen merkkijonoon. Siksi ohjelman on tarkistettava suhteessa `length`, ei vain taulukon kapasiteettiin.

```c
if (index >= length) {
    return STATUS_INVALID_INDEX;
}

character = label[index];
```

Indeksin tyyppi on `size_t`, allekirjoittamaton tyyppi, jota käytetään kokojen ja lukumäärien ilmaisemiseen. Parsinta hylkää negatiivisen syötteen ennen muunnosta `long` -> `size_t`, ja sen jälkeen hylätään mikä tahansa indeksi, joka on suurempi tai yhtäsuuri kuin todellinen merkkijonon pituus. Tämä järjestys estää turvattoman muunnoksen ja rajojen ylittämisen.

## 6. Esimerkkiharjoitus: tarkastele yhden tunnuksen merkkiä turvallisesti

Seurantaohjelma lukee lyhyen tunnuksen ja indeksin. Ohjelma tulostaa pyydetyn merkin vain sen jälkeen, kun se on varmistanut kaikki seuraavat:

1. Tunnusrivi mahtuu kiinteään syöttötaulukkoon ja sisältää rivinvaihdon.
2. Rivinvaihdon poistamisen jälkeen tunnus ei ole tyhjä.
3. Indeksiteksti muunnetaan kokonaan ei-negatiiviseksi luvuksi.
4. Indeksi on pienempi kuin tunnuksen näkyvä pituus.

```c
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATUS_OK = 0,
    STATUS_INVALID_ARGUMENT = 1,
    STATUS_INPUT_FAILURE = 2,
    STATUS_LINE_TOO_LONG = 3,
    STATUS_EMPTY_LABEL = 4,
    STATUS_INVALID_INDEX = 5,
    STATUS_OUTPUT_FAILURE = 6
} Status;

enum {
    MAX_LABEL_LENGTH = 15,
    LABEL_CAPACITY = MAX_LABEL_LENGTH + 2,
    INDEX_LINE_CAPACITY = 32
};

static int discard_remainder_of_line(void)
{
    int character;

    do {
        character = getchar();
    } while (character != '\n' && character != EOF);

    return character == EOF && ferror(stdin) ? 0 : 1;
}

static Status read_line(char *buffer, size_t capacity)
{
    if (buffer == NULL || capacity < 2 || capacity > INT_MAX) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (fgets(buffer, (int)capacity, stdin) == NULL) {
        return STATUS_INPUT_FAILURE;
    }

    if (strchr(buffer, '\n') == NULL) {
        if (!discard_remainder_of_line()) {
            return STATUS_INPUT_FAILURE;
        }
        return STATUS_LINE_TOO_LONG;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    return STATUS_OK;
}

static Status parse_index(const char *text, size_t limit, size_t *result)
{
    char *end = NULL;
    long value;

    if (text == NULL || result == NULL || limit == 0 ||
        limit > (size_t)LONG_MAX) {
        return STATUS_INVALID_ARGUMENT;
    }

    errno = 0;
    value = strtol(text, &end, 10);

    if (end == text || errno == ERANGE || value < 0) {
        return STATUS_INVALID_INDEX;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }

    if (*end != '\0' || value >= (long)limit) {
        return STATUS_INVALID_INDEX;
    }

    *result = (size_t)value;
    return STATUS_OK;
}

static Status write_indexed_character(const char *label, size_t length,
                                      size_t index)
{
    if (label == NULL || length == 0 || index >= length) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (printf("Label: %s\n", label) < 0 ||
        printf("Stored characters: %zu\n", length) < 0 ||
        printf("Character at index %zu: %c\n", index, label[index]) < 0) {
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
    char label[LABEL_CAPACITY];
    char index_line[INDEX_LINE_CAPACITY];
    size_t label_length;
    size_t index;
    Status status;

    if (fputs("Label (1-15 characters): ", stdout) == EOF) {
        return EXIT_FAILURE;
    }

    status = read_line(label, sizeof label);
    if (status == STATUS_LINE_TOO_LONG) {
        (void)write_error("Label is too long. Use at most 15 characters.\n");
        return EXIT_FAILURE;
    }
    if (status != STATUS_OK) {
        (void)write_error("Could not read the label.\n");
        return EXIT_FAILURE;
    }

    label_length = strlen(label);
    if (label_length == 0) {
        (void)write_error("Label must not be empty.\n");
        return EXIT_FAILURE;
    }

    if (printf("Index (0-%zu): ", label_length - 1) < 0) {
        return EXIT_FAILURE;
    }

    status = read_line(index_line, sizeof index_line);
    if (status != STATUS_OK) {
        (void)write_error("Could not read the index.\n");
        return EXIT_FAILURE;
    }

    status = parse_index(index_line, label_length, &index);
    if (status != STATUS_OK) {
        (void)write_error("Index must select an existing label character.\n");
        return EXIT_FAILURE;
    }

    status = write_indexed_character(label, label_length, index);
    if (status != STATUS_OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

Käännä ja suorita liitetty seurantaohjelman tiedosto täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-004-arrays-strings-and-bounds \
  examples/c/c-004-arrays-strings-and-bounds.c
printf 'cat\n1\n' | ./c-004-arrays-strings-and-bounds
```

Odotettu tuloste:

```text
Label (1-15 characters): Index (0-2): Label: cat
Stored characters: 3
Character at index 1: a
```

Kokeile sallitun alueen ulkopuolista indeksiä:

```sh
printf 'cat\n3\n' | ./c-004-arrays-strings-and-bounds
```

Ohjelman tulisi päättyä epäonnistuneena ja kirjoittaa tämä diagnoosi standardivirheeseen:

```text
Index must select an existing label character.
```

## 7. Ohjattu harjoitus

Vastaa ennen spoiler-osion lukemista.

1. Kuinka monta taulukkoalkiota on `char word[] = "cat";`, ja miksi?
2. Mikä on suurin kelvollinen indeksi kohdassa `char scores[10];`?
3. Miksi `strlen` on turvaton `char` -taulukolla, joka ei välttämättä sisällä `\0`?
4. Jos `char label[17]` sisältää `"cat"`, mitä ovat `sizeof label` ja `strlen(label)`?
5. Miksi esimerkki hylkää indeksin `3` tunnukselle `cat`, vaikka taulukon kapasiteetti on suurempi kuin 3?
6. Miksi `read_line` hylkää rivin, kun `strchr(buffer, '\n')` ei löydä rivinvaihtoa?
7. Miksi funktion tulisi saada merkkijonon pituus tai kapasiteetti eksplisiittisesti sen sijaan, että se laskisi `sizeof` osoitintyyppisestä parametrista?

## 8. Itsenäinen harjoitus

Luo `exercise.c` seurantaohjelman viereen. Ilmoita kiinteän kokoisen merkkitaulukon koko niin, että se hyväksyy yhdensanaisen kurssitunnuksen, jossa on enintään 12 näkyvää merkkiä. Käytä tämän oppitunnin tarkistettua rivinlukumallia ja tulosta tunnuksen ensimmäinen ja viimeinen merkki.

Ohjelmasi tulee:

1. Varaa tilaa rivinvaihdolle ja `\0` sekä näkyville merkeille.
2. Hylkää tyhjä tunnus ja liian pitkä rivi sen sijaan, että leikkaisit sen hiljaisesti.
3. Kutsu `strlen` vasta onnistuneen syötön jälkeen, kun nollapääte on varmennettu.
4. Hylkää mikä tahansa pyydetty indeksi, jolle `index >= strlen(tag)`.
5. Käytä `size_t` kapasiteetti-, pituus- ja indeksi-muuttujille.

Testaa yhden merkin tunnusta, 12-merkkistä tunnusta, tyhjää riviä, 13-merkkistä tunnusta ja indeksiä, joka on yhtä suuri kuin merkkijonon pituus.

## 9. Tarkista sanitisaattoreilla

Suorita repositorion deterministinen testi:

```sh
./tests/c-004-arrays-strings-and-bounds.sh
```

Käännä ja suorita sitten AddressSanitizer- ja UndefinedBehaviorSanitizer-työkalujen kanssa, jos ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-004-sanitized \
  examples/c/c-004-arrays-strings-and-bounds.c
printf 'cat\n1\n' | ./c-004-sanitized
printf 'cat\n3\n' | ./c-004-sanitized
```

Kelvollinen syöte tulostaa indeksoidun merkin. Virheellinen syöte palauttaa epä-nollan arvon odotetulla diagnoosilla eikä tuota sanitisaattoriraporttia. Sanitisaattorit voivat paljastaa muistivirheitä testauksen aikana, mutta ne eivät korvaa lähdekoodissa tehtäviä eksplisiittisiä terminointi- ja rajatarkistuksia.

## 10. Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit tehdä seuraavat ilman takaisintarkistusta.

1. Määritä kelvollinen indeksiarvojen väli taulukolle, jossa on `N` alkioita.
2. Selitä ero `char` -taulukon ja C-merkkijonon välillä.
3. Selitä, miksi näkyvä 15-merkkinen rivi tarvitsee lisätilaa rivinvaihdolle ja `\0`.
4. Määritä ero `sizeof label` ja `strlen(label)`.
5. Selitä, miksi indeksi-validointi käyttää `index < label_length` eikä pelkästään `index < sizeof label`.
6. Muistatko venäjän neutri-adjektiivin muodon lauseessa **новое окно**.

## Vastausosio: vastaukset ja mallivastaukset

1. C 002 käyttää rajattua puskuria, jotta ohjelma voi tarkastella koko syötettä ennen muunnosta ja hylätä liian pitkän tai virheellisen rivin.
2. `end == text` tarkoittaa, että `strtol` ei muuntanut yhtään numeroa.
3. `&total_minutes` välittää osoitteen kutsujan `int` tallennustilaan.
4. Epäonnistunut tila tarkoittaa, että ulostulo ei välttämättä ole kelvollinen; kutsujan on lopetettava tai käsiteltävä virhe ennen arvon käyttöä.
5. **новое**.
6. `char word[] = "cat";` sisältää neljä alkiota: kolme näkyvää merkkiä ja yhden päättävän `\0`.
7. Suurin kelvollinen indeksi kohdassa `char scores[10];` on `9`.
8. `strlen` etsii nollapäättymää; jos päättymää ei ole todellisessa taulukossa, se lukee taulukon ulkopuolelta ja käyttäytyminen on määrittelemätöntä.
9. 17-alkioisessa taulukossa, joka pitää `"cat"`, `sizeof label` on `17` ja `strlen(label)` on `3`.
10. Indeksi `3` on nollapääte, ei näkyvä tunnuksen merkki. Suurempi indeksi ei kuulu loogiseen merkkijonoon.
11. Jos rivinvaihtoa ei luettu, ohjelma ei voi turvallisesti käsitellä puskuria hyväksyttynä rivinä; se hylkää syötteen kuluttaen rivin lopun.
12. Funktioparametrissa taulukko on kutistunut osoittimeksi, joten `sizeof` kuvaa osoitinta eikä kutsujan alkuperäistä taulukkoa.
13. **новое окно** tarkoittaa "new window".

## Seuraava oppitunti

Seuraava ajastettu oppitunti palaa venäjään: **Russian 005 — yleisesti käytetyt preesensin verbit ja lyhyet arkirutiinilausahdukset.**

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fgets "fgets — cppreference.com"
[2]: https://en.cppreference.com/w/c/string/byte/strlen "strlen, strnlen_s — cppreference.com"
[3]: https://en.cppreference.com/w/c/string/byte/strncpy "strncpy, strncpy_s — cppreference.com"
