# C 189 — Third-Party Error Mapping

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–188
**Seuraava ohjelma:** [`examples/c/c-189-third-party-error-mapping.c`](../../examples/c/c-189-third-party-error-mapping.c)
**Deterministinen testi:** [`tests/c-189-third-party-error-mapping.sh`](../../tests/c-189-third-party-error-mapping.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat kartoittaa pienen kolmannen osapuolen tyyppisen tuloksen paikallisiin nimettyihin tiloihin menetyksettä siten, että epäonnistumisen merkitys säilyy.

## Palautusharjoitus

1. Miksi raakabyttein sisältävän puskurin täytyy kantaa selkeä `size_t` pituus?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, varaus- tai jäsennys-API:iden paluuarvot on tarkistettava ennen niiden tuottamien arvojen luottamista?
4. Miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Virheiden kartoituksen tulee säilyttää toiminnallisesti merkittävät erot samalla kun se välttää arkaluonteisten tietojen vuotamista. Tuotannossa katso aina kirjaston oma dokumentaatio.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee bittejä laskettuina datoina, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, konversion ja palautetuloksen joita esimerkki käyttää, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessin injektointia, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(189)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 189 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-189-third-party-error-mapping \
  examples/c/c-189-third-party-error-mapping.c
./c-189-third-party-error-mapping
```

Odotettu tulostus:

```text
C 189 validated byte: 189
```

## Ohjattu harjoittelu

1. Tunnista tavujen alue tai tilasopimus jokaisessa apurajapinnassa.
2. Etsi aikaisin tapahtuava ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -sijainti saattaa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tulostus hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuvan varauskoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritettavaa lataajaa tai mielivaltaisen koodin suorituksen mekanismia.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-189-third-party-error-mapping.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-189-sanitized \
  examples/c/c-189-third-party-error-mapping.c
./c-189-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syöttösopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli-vastaus

1. Laskettu tavurajapinta käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus edellyttää sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-kielelle suunnattuun osuuteen.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
