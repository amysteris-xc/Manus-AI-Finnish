# C 191 — No-Custom-Crypto Policy

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Osaamisvaatimus:** C 001–190
**Seuraa ohjelma:** [`examples/c/c-191-no-custom-crypto-policy.c`](../../examples/c/c-191-no-custom-crypto-policy.c)
**Deterministinen testi:** [`tests/c-191-no-custom-crypto-policy.sh`](../../tests/c-191-no-custom-crypto-policy.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin jälkeen osaat määritellä ja toimeenpanna ohjelmatason säännön, jonka mukaan opetusluontoinen sekoituskoodi ei ole valittavissa todelliseen suojaamiseen.

## Muistiharjoitus

1. Miksi raaka tavutaulukko tarvitsee selvästi ilmaistun `size_t` pituuden?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsentämis-API:iden palautusarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Politiinarvohko estää leikkikoodin nostamisen tuotantoon. Oikea kryptografia vaatii ylläpidettyjä kirjastoja, dokumentoituja API:ita, testivektoreita ja tarkastelua.

Tässä oppitunnissa käytetään kannettavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkastaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja palautustuloksen, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(191)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 191 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-191-no-custom-crypto-policy \
  examples/c/c-191-no-custom-crypto-policy.c
./c-191-no-custom-crypto-policy
```

Odotettu tuloste:

```text
C 191 validated byte: 191
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi varhaisin ehto, joka hylkää virheellisen osoittimen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asento voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksessä ja lisää tarkastettu aritmetiikkaehto ennen minkään muuttuvan varauksen kokolaskentaa. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-191-no-custom-crypto-policy.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-191-sanitized \
  examples/c/c-191-no-custom-crypto-policy.c
./c-191-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit ilmaista tuloehdon, tunnistaa hylätyn rajatapauksen, selittää lähdön ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavuan raja-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-kielelliseen linjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
