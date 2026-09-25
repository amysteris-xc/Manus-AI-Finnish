# C 194 — Testivektorin pakkaus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–193
**Seurantoprogrammi:** [`examples/c/c-194-test-vector-packaging.c`](../../examples/c/c-194-test-vector-packaging.c)
**Deterministinen testi:** [`tests/c-194-test-vector-packaging.sh`](../../tests/c-194-test-vector-packaging.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi sinun pitäisi osata pakata kiinteä tavusyöte, odotettu tulos ja versionimi toistettavaksi testivektoriksi.

## Palautusharjoittelu

1. Miksi raaka tavupuskuri tarvitsee eksplisiittisen `size_t` pituuden?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta tulee tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Sano, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen ajatus

Vektorin tulee olla yksiselitteinen eikä salainen. Se tukee regresio- ja yhteentoimivuustestausta mutta ei yksin todista turvallisuutta.

Tämä oppitunti käyttää siirrettävää ISO C17 -kieltä. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, johon esimerkki nojaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessin injektiota, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

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
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(194), UINT8_C(255)};
    if (printf("C 194 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-194-test-vector-packaging \
  examples/c/c-194-test-vector-packaging.c
./c-194-test-vector-packaging
```

Odotettu tulos:

```text
C 194 hex: 00C2FF
```

## Ohjattu harjoittelu

1. Tunnista tavujänne tai tilasopimus jokaisessa apurajapinnassa.
2. Löydä aikaisin hylkäävä ehto, joka torjuu pätemättömän osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Sano, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty reunatapaus. Säilytä eksplisiittiset tavumäärät, pidä alkuperäinen lähtö hylkäyksen jälkeen ennallaan, ja lisää tarkistettu aritmeettinen ehto ennen mitään muutettua allokointikoon laskentaa. Älä tee tästä opetus-esimerkistä kryptografista, suoritettavan latausmekanismia tai mielivaltaisen koodin ajomekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-194-test-vector-packaging.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-194-sanitized \
  examples/c/c-194-test-vector-packaging.c
./c-194-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos pystyt kertomaan syötesopimuksen, tunnistamaan hylätyn reunatapauksen, selittämään lähdön ja erottamaan puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli

1. Laskettu tavujen API käyttää osoitinta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri tarkistaa ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajoitusten tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavassa aikataulutetussa oppitunnossa palataan venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
