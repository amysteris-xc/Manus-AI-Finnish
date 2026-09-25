# C 110 — Aliasing ja `memcpy` -raja

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–109
**Seuralaisohjelma:** [`examples/c/c-110-aliasing-and-memcpy-boundary.c`](../../examples/c/c-110-aliasing-and-memcpy-boundary.c)
**Deterministinen testi:** [`tests/c-110-aliasing-and-memcpy-boundary.sh`](../../tests/c-110-aliasing-and-memcpy-boundary.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata siirtää laskettu määrä tavuja kautta `memcpy` tai `memmove` vain sen jälkeen, kun molemmat alueet on validoitu.

## Muistiharjoitus

1. Miksi raakaa tavutaulukkoa pitää kantaa mukana selkeä `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä palautusarvot I/O-, varaus- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuottamiin tietoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastusarvo ei ole todellinen autentikointi aktiivista hyökkääjää vastaan.

## Ydinajatus

Tavujen kopiointi vaatii lähteen pituuden, kohteen kapasiteetin ja päällekkäisyssäännöt. Älä tee tyyppimuunnoksia ja kierrätä mielivaltaisia objekteja turvattomien osoitincastien kautta.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavuja laskettuina datoina, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja palautetuloksen johon esimerkki nojautuu, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai aitoja kryptografisia toteutuksia.

## Läpi käyty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_span(const uint8_t *source, size_t count, uint8_t *destination, size_t capacity)
{
    if ((source == NULL && count != 0) || destination == NULL || count > capacity) return 0;
    if (count != 0) memmove(destination, source, count);
    return 1;
}

int main(void)
{
    const uint8_t source[] = {UINT8_C(110), UINT8_C(0)};
    uint8_t destination[2] = {0};
    if (!copy_span(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 110 copied byte: %u\n", (unsigned)destination[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-110-aliasing-and-memcpy-boundary \
  examples/c/c-110-aliasing-and-memcpy-boundary.c
./c-110-aliasing-and-memcpy-boundary
```

Odotettu tulos:

```text
C 110 copied byte: 110
```

## Ohjattu harjoitus

1. Tunnista tavujen väli tai tilasopimus kunkin apurajan kohdalla.
2. Etsi aikaisin oleva ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Käytä selkeitä tavumääriä, säilytä alkuperäinen tuloste hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskentaa. Älä tee tästä opettavaisesimerkistä kryptografista, suoritettava-lataajaa tai mielivaltaisen koodin suorittamisjärjestelmää.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-110-aliasing-and-memcpy-boundary.sh
```

Käännä ja aja sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-110-sanitized \
  examples/c/c-110-aliasing-and-memcpy-boundary.c
./c-110-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määrittää syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähdön ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Juonipaljastus: vastaukset ja malli­vastaus

1. Laskettu tavu-API käyttää osoitinta ja nimenomaista pituutta, koska upotetut nollatavut ovat sallittua dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen raja-arvojen katselmointia; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-ketjuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
