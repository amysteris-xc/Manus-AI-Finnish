# C 156 — Mutation Corpus Safety

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–155
**Seuralaisohjelma:** [`examples/c/c-156-mutation-corpus-safety.c`](../../examples/c/c-156-mutation-corpus-safety.c)
**Deterministinen testi:** [`tests/c-156-mutation-corpus-safety.sh`](../../tests/c-156-mutation-corpus-safety.sh)
**Tekijä:** Manus AI

## Mitä osaat tunteen jälkeen

Tämän oppitunnin jälkeen osaat muuntaa pienen muistissa olevan tavujoukon siten, että tunnettu enimmäispituus säilyy.

## Muistikoe

1. Miksi raakabyte-puskuriin on liitettävä eksplisiittinen `size_t` pituus?
2. Mikä silmukan ehto pitää eteenpäin kasvavan indeksin `count`-alkioisessa puskurissa?
3. Mitkä I/O-, allokaatio- tai jäsennys-API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Miksi ei-kryptografinen tarkistusarvo ei ole autentikointi aktiivista hyökkääjää vastaan?

## Keskeinen käsite

Mutation-testidataa tulee pitää rajatussa puskurissa. Jokainen generoitu tapaus pysyy parserin tavanomaisen validoinnin alaisena.

Tämä oppitunti käyttää kannettavaa ISO C17:ää. Se käsittelee tavut laskettuina datoina, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja palautusarvon, joihin esimerkki luottaa, ja säilyttää tulostilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista asm-suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

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
    const uint8_t input[] = {UINT8_C(156)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 156 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-156-mutation-corpus-safety \
  examples/c/c-156-mutation-corpus-safety.c
./c-156-mutation-corpus-safety
```

Odotettu tuloste:

```text
C 156 validated byte: 156
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus jokaisen apurajapinnan kohdalla.
2. Etsi aikaisin esiintyvä ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Mainitse, milloin lähtöarvo tai -paikka saattaa muuttua.
5. Selitä oppitunnissa mainittu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksen yhteydessä ja lisää tarkistettu aritmeettinen ehto ennen kaikkia muutettuja allokaatiosuuruuden laskelmia. Älä muunna tätä opetusesimerkkiä kryptografiseksi, suoritettavan lataajaksi tai mielivaltaisen koodin suoritusmekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-156-mutation-corpus-safety.sh
```

Käännä ja aja sanitisaattoreilla siellä missä ne ovat tuettuja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-156-sanitized \
  examples/c/c-156-mutation-corpus-safety.c
./c-156-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä syötesopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Laskettu tavu-API käyttää osoitinta ja eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliseen polkuun.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
