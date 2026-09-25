# C 190 — Turvallinen oletuskonfiguraatio

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Edellytykset:** C 001–189
**Seuraava esimerkkiohjelma:** [`examples/c/c-190-secure-default-configuration.c`](../../examples/c/c-190-secure-default-configuration.c)
**Deterministinen testi:** [`tests/c-190-secure-default-configuration.sh`](../../tests/c-190-secure-default-configuration.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata esittää konservatiivinen, ei-salaista tietoa sisältävä konfiguraatio, jossa on eksplisiittiset sallitut rajat ja poissa käytöstä olevat valinnaiset ominaisuudet.

## Palautusharjoitus

1. Miksi raakabufferin täytyy kantaa eksplisiittistä `size_t` pituutta?
2. Mikä silmukan ehto pitää etenevän indeksin `count`-alkioisessa bufferissa?
3. Mitkä I/O-, muisti­varaus- tai jäsentämis­rajapintojen paluuarvot on tarkistettava ennen niiden tuottamien arvojen luottamista?
4. Perustele, miksi ei-kryptografinen tarkastus­arvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Oletusarvojen tulee olla eksplisiittisiä, vähimmäisoikeuksia noudattavia ja helposti tarkastettavissa. Tämä esimerkki ei konfiguroi oikeaa palvelua tai turvatuotetta.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluu­tuloksen, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataimia, mielivaltaista assemblyn suoritusta, prosessiin injektointia, itsensä muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(190)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 190 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-190-secure-default-configuration \
  examples/c/c-190-secure-default-configuration.c
./c-190-secure-default-configuration
```

Odotettu tuloste:

```text
C 190 validated byte: 190
```

## Ohjattu harjoitus

1. Tunnista tavuttoman alueen tai tilasopimus jokaisen apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi tässä esimerkissä `0x00` on tavallista dataa eikä loppumerkki.
4. Ilmoita milloin lähtöarvo tai -asema saa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä tarkalla lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen varauskoon laskemista. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suorittamisen mekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-190-secure-default-configuration.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-190-sanitized \
  examples/c/c-190-secure-default-configuration.c
./c-190-sanitized
```

## Lopullinen itse­tarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollabytet ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että mainittujen rajojen tarkastelua; mikään pieni esimerkkitapaus ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava suunniteltu oppitunti palaa русская траекторияlle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
