# C 138 — Hex-koodauksen ympäri ja takaisin

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitaitovaatimus:** C 001–137
**Seurantaohjelma:** [`examples/c/c-138-hex-encode-round-trip.c`](../../examples/c/c-138-hex-encode-round-trip.c)
**Deterministinen testi:** [`tests/c-138-hex-encode-round-trip.sh`](../../tests/c-138-hex-encode-round-trip.sh)
**Tekijä:** Manus AI

## Mitä osaat kurssin jälkeen

Tämän oppitunnin jälkeen osaat esittää tavutaulukon heksadesimaalisena merkkijonona ja varmistaa, että koodattu esitys on rajattu oikein.

## Muistin virkistys

1. Miksi raaka tavutaulukko täytyy kantaa mukana eksplisiittisellä `size_t` pituudella?
2. Mikä silmaehdo pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä paluuarvot I/O-, allokointi- tai jäsentämis-API:sta täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Hex-koodaus kasvattaa jokaista tavua ennustettavasti. Lähtökapasiteetin pitää ottaa huomioon kaksi merkkiä per tavu plus valinnainen terminointimerkki.

Tämä oppitunti käyttää kannettavaa ISO C17 -standardia. Se käsittelee tavuja laskettuna datana, tarkistaa jokaisen osoitteen, määrän, kapasiteetin, muunnoksen ja palautusarvon, joihin esimerkki luottaa, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista aseman toteutusta, prosessiin injektointia, itseään muokkaavaa koodia tai oikeita kryptografisia toteutuksia.

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
    const uint8_t bytes[] = {UINT8_C(0), UINT8_C(138), UINT8_C(255)};
    if (printf("C 138 hex: ") < 0 || !print_hex_prefix(bytes, sizeof bytes, sizeof bytes) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-138-hex-encode-round-trip \
  examples/c/c-138-hex-encode-round-trip.c
./c-138-hex-encode-round-trip
```

Odotettu tuloste:

```text
C 138 hex: 008AFF
```

## Ohjattu harjoitus

1. Tunnista tavujakso tai tilasopimus kunkin apufunktion rajapinnalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data, ei loppumerkki.
4. Ilmoita milloin lähtöarvo tai -positio saa muuttua.
5. Selitä oppitunnin mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen yhteydessä, ja lisää tarkistettu aritmetiikkaehto ennen minkään muutetun allokoinnin koko-laskennan suorittamista. Älä taita tätä opetusesimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Varmennus

Suorita deterministinen testi:

```sh
./tests/c-138-hex-encode-round-trip.sh
```

Käännä ja suorita sanitisaattoreiden kanssa siellä missä ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-138-sanitized \
  examples/c/c-138-hex-encode-round-trip.c
./c-138-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syöterasoituksen, tunnistaa hylätyn rajatapauksen, selittää lähdön ja erottaa defensiivisen tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavurajapinta käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen kuin muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa Russian-kielelle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
