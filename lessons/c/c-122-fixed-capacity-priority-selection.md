# C 122 — Kiinteäkapasiteettinen prioriteettivalinta

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–121
**Seuraaohjelma:** [`examples/c/c-122-fixed-capacity-priority-selection.c`](../../examples/c/c-122-fixed-capacity-priority-selection.c)
**Deterministinen testi:** [`tests/c-122-fixed-capacity-priority-selection.sh`](../../tests/c-122-fixed-capacity-priority-selection.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata valita korkeimman prioriteetin tavutietue rajatusta kiinteäkapasiteettisesta joukosta.

## Muistiharjoitus

1. Miksi raaka tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä palautusarvot I/O-, allokointi- tai jäsentämis-API:sta pitää tarkistaa, ennen kuin niiden tuloksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Ydinajatus

Rajatussa läpikäynnissä tarvitaan laskuri, joka ei ole suurempi kuin kapasiteetti, sekä selkeä käytäntö tasapisteille ja tyhjälle syötteelle.

Tämä oppitunti käyttää kannettavaa ISO C17 -kieltä. Se käsittelee tavuja laskettuna datana, tarkistaa kaikki osoittimet, lukumäärät, kapasiteetit, muunnokset ja palautustulokset, joihin esimerkki luottaa, ja säilyttää ulostulotilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataimia, mielivaltaista assembly-suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Esimerkkityö

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
    const uint8_t table[] = {UINT8_C(1), UINT8_C(122), UINT8_C(250)};
    size_t index = 0;
    if (!find_byte(table, sizeof table, UINT8_C(122), &index)) return EXIT_FAILURE;
    if (printf("C 122 table index: %zu\n", index) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-122-fixed-capacity-priority-selection \
  examples/c/c-122-fixed-capacity-priority-selection.c
./c-122-fixed-capacity-priority-selection
```

Odotettu tulostus:

```text
C 122 table index: 1
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin ulostuloarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan allokaatiokoon laskemista. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suorituksenlataajaksi tai mielivaltaisen koodin suorituksen mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-122-fixed-capacity-priority-selection.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-122-sanitized \
  examples/c/c-122-fixed-capacity-priority-selection.c
./c-122-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syöttösopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa defensiivisen tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Vastausvinkki: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen läpikäyntiä; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäläiselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
