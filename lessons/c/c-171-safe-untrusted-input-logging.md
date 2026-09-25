# C 171 — Safe Untrusted Input Logging

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–170
**Seurantaohjelma:** [`examples/c/c-171-safe-untrusted-input-logging.c`](../../examples/c/c-171-safe-untrusted-input-logging.c)
**Deterministinen testi:** [`tests/c-171-safe-untrusted-input-logging.sh`](../../tests/c-171-safe-untrusted-input-logging.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi pystyä kirjaamaan rajoitetun yhteenvedon epäluotettavista tavuista ilman, että niitä käsitellään C-merkkijonona.

## Kertaus

1. Miksi raakaa tavutaulukkoa täytyy kantaa mukanaan eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentämis-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamia arvoja luotetaan?
4. Perustele, miksi ei-kriptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Diagnostiikan tulee rajoittaa pituutta, käyttää turvallista heksanumeromuotoilua ja välttää arkaluontoisen tai tarpeettoman syötteen paljastamista.

Tämä oppitunti käyttää kannatettavaa ISO C17 -kieltä. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki luottaa, ja säilyttää lähtötilan siihen asti, kunnes validointi onnistuu. Se ei luo shellcodea, natiivilataajia, mielivaltaista assemblyn suorittamista, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

## Käytännön esimerkki

```c
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_hex_prefix(const uint8_t *data, size_t count, size_t maximum)
{
    size_t shown = count < maximum ? count : maximum;
    if (data == NULL && count != 0) return 0;
    for (size_t i = 0; i < shown; ++i)
        if (printf("%02" PRIX8, data[i]) < 0) return 0;
    return 1;
}

int main(void)
{
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(171), UINT8_C(255)};
    if (printf("C 171 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-171-safe-untrusted-input-logging \
  examples/c/c-171-safe-untrusted-input-logging.c
./c-171-safe-untrusted-input-logging
```

Odotettu tuloste:

```text
C 171 hex: 00ABFF
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus kunkin apufunktion rajapinnassa.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoite yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmeettinen ehto ennen kuin muutat allokoinnin kokoon liittyvää laskelmaa. Älä tee tästä opetusesimerkistä kryptografista, suoritustason lataajaa tai mekanismia mielivaltaisen koodin suorittamiseen.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-171-safe-untrusted-input-logging.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-171-sanitized \
  examples/c/c-171-safe-untrusted-input-logging.c
./c-171-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtötuloksen ja erottaa defensiivisen tavunkäsittelyn todellisesta käytännön kryptografisesta suojauksesta.

## Vastausvinkki: vastaukset ja mallivastaus

1. Lasketun tavua-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
