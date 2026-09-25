# C 109 — Kannettava kokonaislukujen muotoilu

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–108
**Seuraava ohjelma:** [`examples/c/c-109-portable-integer-formatting.c`](../../examples/c/c-109-portable-integer-formatting.c)
**Deterministinen testi:** [`tests/c-109-portable-integer-formatting.sh`](../../tests/c-109-portable-integer-formatting.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa osaat muotoilla kiinteän levyisiä kokonaislukuarvoja käyttäen vastaavia `<inttypes.h>` -makroja.

## Kertaus

1. Miksi raakabufferin yhteydessä täytyy aina kantaa mukana eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehdon muoto pitää etenevän indeksin sisällä `count`-alkioisen bufferin rajoissa?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämis-API:sta on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Selitä, miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Kannettava tulostus käyttää sopivia `PRI...` -makroja tai turvallista promootiota dokumentoidun formaatin kanssa — ei oletuksia typedef-aliaksista.

Tämä oppitunti käyttää kannattavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa kaikki osoittimet, määrät, kapasiteetit, muunnokset ja paluuarvot, joihin esimerkki nojaa, ja säilyttää tulostustilan kunnes validointi onnistuu. Se EI luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin ajoa, prosessiin injektointia, itseään muuttavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(109), UINT8_C(255)};
    if (printf("C 109 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-109-portable-integer-formatting \
  examples/c/c-109-portable-integer-formatting.c
./c-109-portable-integer-formatting
```

Odotettu tuloste:

```text
C 109 hex: 006DFF
```

## Ohjattu harjoitus

1. Tunnista tavuspanne tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi varhaisin ehto, joka hylkää epäkelvon osoittimen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määritä milloin tulosarvo tai paikka saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Säilytä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä ja lisää tarkistettu aritmetiikkaehto ennen minkään muutetun allokaation koon laskemista. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavaa lataavaksi tai mielivaltaista koodin ajoa mahdollistavaksi mekanismiksi.

## Validointi

Aja deterministinen testi:

```sh
./tests/c-109-portable-integer-formatting.sh
```

Käännä ja aja sanitisaattoreiden kanssa, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-109-sanitized \
  examples/c/c-109-portable-integer-formatting.c
./c-109-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tulosteen ja erottaa defensiivisen tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Lasketun tavun API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apurutiini validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelun; pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavassa aikataulutetussa oppitunnissa palataan Russian-raidalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
