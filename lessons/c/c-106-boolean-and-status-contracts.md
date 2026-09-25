# C 106 — Boolean ja tilasopimukset

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–105
**Seuraava ohjelma:** [`examples/c/c-106-boolean-and-status-contracts.c`](../../examples/c/c-106-boolean-and-status-contracts.c)
**Deterministinen testi:** [`tests/c-106-boolean-and-status-contracts.sh`](../../tests/c-106-boolean-and-status-contracts.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi osata valita selkeä palautussopimus apufunktiolle, joka validoi tavutuloa.

## Muistiharjoitus

1. Miksi raakabufferilla täytyy olla eksplisiittinen `size_t` pituus?
2. Mikä silmukkeiden ehto pitää eteenpäin kulkevan indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O:n, varauksen tai jäsentämisen API:en paluuarvot täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kriptografinen tarkistusluku ei ole todennus aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Käytä Bool-arvoa vain yhteen onnistuminen/epäonnistuminen-kysymykseen; käytä nimettyjä tilaarvoja, kun kutsujien täytyy erottaa epäonnistumisen syyt.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja esimerkin luottamuksen arvoina olevat paluuarvot, ja säilyttää ulostulon tilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessien injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(106)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 106 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-106-boolean-and-status-contracts \
  examples/c/c-106-boolean-and-status-contracts.c
./c-106-boolean-and-status-contracts
```

Odotettu tulostus:

```text
C 106 validated byte: 106
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apurajan kohdalla.
2. Etsi aikaisin ehtotilanne, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tavallinen data eikä lopetusmerkki tässä esimerkissä.
4. Ilmoita milloin ulostuloarvo tai —asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä, ja lisää tarkistettua aritmetiikkaa ennen mitä tahansa muutettua varauksen kokolaskelmaa. Älä tee tästä opetusesimerkistä kryptografista, suoritettavaa lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-106-boolean-and-status-contracts.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-106-sanitized \
  examples/c/c-106-boolean-and-status-contracts.c
./c-106-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit kertoa sisääntulosopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavupääte-API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
