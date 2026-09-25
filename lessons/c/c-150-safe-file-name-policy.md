# C 150 — Safe File Name Policy

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–149
**Seuraava ohjelma:** [`examples/c/c-150-safe-file-name-policy.c`](../../examples/c/c-150-safe-file-name-policy.c)
**Deterministinen testi:** [`tests/c-150-safe-file-name-policy.sh`](../../tests/c-150-safe-file-name-policy.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen pystyt validoimaan pienen sallittujen testitiedostotunnisteen ennen sen käyttöä paikallisessa demonstraatiossa.

## Muistin harjoittelu

1. Miksi raakapuskurin (raw byte buffer) täytyy kantaa eksplisiittistä `size_t` pituutta?
2. Mikä silmukan ehtolauseke pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokointi- tai jäsentämisrajapintojen paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Selitä, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Peruskäsite

Tiedostonimen politiikan tulee hylätä polkuerottimet, ohjausmerkit ja odottamattomat pituudet. Tämä oppitunti ei ole hiekkalaatikon kiertämis- tai järjestelmäkäyttötekniikka.

Tämä oppitunti käyttää kannateltavaa ISO C17 -kieltä. Se käsittelee tavuja lasketuksi dataksi, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja paluuarvon, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblyn suoritusta, prosessi-injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(150)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 150 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-150-safe-file-name-policy \
  examples/c/c-150-safe-file-name-policy.c
./c-150-safe-file-name-policy
```

Odotettu tuloste:

```text
C 150 validated byte: 150
```

## Ohjattu harjoitus

1. Tunnista tavujen alue (byte span) tai tilalupaus (state contract) kunkin apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -paikka voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokointikoon laskentaa. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritettavien lataajaksi tai mielivaltaisen koodin suoritustavaksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-150-safe-file-name-policy.sh
```

Käännä ja suorita sanitisaattoreilla siellä missä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-150-sanitized \
  examples/c/c-150-safe-file-name-policy.c
./c-150-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle uralle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
