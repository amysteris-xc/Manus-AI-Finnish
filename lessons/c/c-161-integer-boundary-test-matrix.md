# C 161 — Kokonaislukujen rajatestausmatriisi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–160
**Seuraava ohjelma:** [`examples/c/c-161-integer-boundary-test-matrix.c`](../../examples/c/c-161-integer-boundary-test-matrix.c)
**Deterministinen testi:** [`tests/c-161-integer-boundary-test-matrix.sh`](../../tests/c-161-integer-boundary-test-matrix.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata testata valittuja minimikumaksimiarvoja ja juuri niiden ulkopuolisia arvoja ennen koon kaventamista tai laskemista.

## Muistin harjoitus

1. Miksi raakabittipuskurilla pitää olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkiehtoa pitää eteenpäin kasvavan indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämisrajapintojen paluuarvot täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkastusalue ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Rajatestien tulee nimetä testattava tyyppi ja konversio. Älä luota implementaation sattumanvaraiseen kiertymiseen.

Tämä oppitunti käyttää siirrettävää ISO C17:ää. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, konversion ja palautustuloksen, joihin esimerkki nojautuu, ja säilyttää ulostulotilan valideeraukseen asti. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one(const uint8_t *data, size_t count, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (count != 1 || data[0] == 0) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(161)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 161 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-161-integer-boundary-test-matrix \
  examples/c/c-161-integer-boundary-test-matrix.c
./c-161-integer-boundary-test-matrix
```

Odotettu tuloste:

```text
C 161 validated byte: 161
```

## Ohjattu harjoitus

1. Tunnista tavupituusväli tai tilasopimus jokaisessa apuriidan rajapinnassa.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoitteen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa ennemmin kuin lopetusmerkki.
4. Ilmoita milloin ulostuloarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajaustapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen mitään muutettua allokointikokolaskelmaa. Älä tee tästä opetusmielessä olevasta esimerkistä kryptografista, suoritettavan lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-161-integer-boundary-test-matrix.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-161-sanitized \
  examples/c/c-161-integer-boundary-test-matrix.c
./c-161-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit kuvata sisäänottosopimuksen, tunnistaa hylätyn rajaustapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavarajapinnan yhteydessä käytetään osoitetta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkevassa läpikäynnissä tarkistetaan `index < count` ennen kuin luetaan `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava suunniteltu oppitunti palaa venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
