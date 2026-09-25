# C 163 — Omistajuuskartan tarkastus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edellytykset:** C 001–162
**Seuraava ohjelma:** [`examples/c/c-163-ownership-map-review.c`](../../examples/c/c-163-ownership-map-review.c)
**Deterministinen testi:** [`tests/c-163-ownership-map-review.sh`](../../tests/c-163-ownership-map-review.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi pystyä piirtämään ja valvomaan omistajuuskarttaa pienen puskurin datavirralle.

## Muistiharjoitus

1. Miksi raakabytetason puskurilla täytyy olla selkeä `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsentämisrajapintojen palautusarvot on tarkistettava ennen kuin niiden tuotoksiin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistus-arvo ei ole todennus aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Jokaisella puskurilla on kerrallaan yksi omistaja. Tarkastuksen tulee kertoa, kuka varaa, kuka saa lainata ja kuka vapauttaa sen.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja esimerkin varaan luottaman palautetuloksen sekä säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivia lataajaa, mielivaltaista assemblerin ajamista, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Läpi käyty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int checked_count_bytes(size_t count, size_t item_size, size_t *out)
{
    if (out == NULL || item_size == 0 || count > SIZE_MAX / item_size) return 0;
    *out = count * item_size;
    return 1;
}

int main(void)
{
    size_t bytes = 0;
    if (!checked_count_bytes((size_t)163, sizeof(uint8_t), &bytes)) return EXIT_FAILURE;
    if (printf("C 163 checked bytes: %zu\n", bytes) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-163-ownership-map-review \
  examples/c/c-163-ownership-map-review.c
./c-163-ownership-map-review
```

Odotettu tuloste:

```text
C 163 checked bytes: 163
```

## Ohjattu harjoitus

1. Tunnista tavun alue tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää kelvottoman osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` tässä esimerkissä on tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähdetulos tai positio voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavaa lataavaksi tai mielivaltaisen koodin suorittamista mahdollistavaksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-163-ownership-map-review.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-163-sanitized \
  examples/c/c-163-ownership-map-review.c
./c-163-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat määrittää syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuraja tarkastaa ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus edellyttää sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-kielelle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
