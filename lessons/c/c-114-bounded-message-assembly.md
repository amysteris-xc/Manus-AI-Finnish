# C 114 — Rajoitettu viestin kokoaminen

**Arvioitu oppimisaika:** 25–35 minuuttia
**Esitiedot:** C 001–113
**Seuraajaohjelma:** [`examples/c/c-114-bounded-message-assembly.c`](../../examples/c/c-114-bounded-message-assembly.c)
**Deterministinen testi:** [`tests/c-114-bounded-message-assembly.sh`](../../tests/c-114-bounded-message-assembly.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat koota pienen viestin lasketuista osista samalla kun varmistat tulostilan kapasiteetin.

## Muistiharjoitus

1. Miksi raakabufferin täytyy kantaa selkeä `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin `count`-alkioisen bufferin sisällä?
3. Mitkä paluuarvot I/O-, allokaatio- tai jäsentämisrajapinnoista pitää tarkistaa ennen kuin niiden tuloksiin luotetaan?
4. Perustele miksi ei-kryptografinen tarkastusumoarvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Jokaisen lisäyksen täytyy todistaa jäljellä oleva kapasiteetti ennen kirjoitusasennon muuttamista. Sisäiset nollabytet ovat tavanomaista dataa.

Tämä oppitunti käyttää kannateltavaa ISO C17 -kieltä. Se käsittelee bittejä laskettuna datana, tarkistaa jokaisen osoittimen, määrän, kapasiteetin, muunnoksen ja paluuarvon, mihin esimerkki nojaa, ja säilyttää tulostilan tilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivia lataajaa, mielivaltaista käskykannan suorittamista, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t *data; size_t capacity; size_t position; } Writer;

static int writer_put(Writer *writer, uint8_t value)
{
    if (writer == NULL || writer->data == NULL || writer->position >= writer->capacity) return 0;
    writer->data[writer->position] = value;
    writer->position++;
    return 1;
}

int main(void)
{
    uint8_t bytes[2] = {0};
    Writer writer = {bytes, sizeof bytes, 0};
    if (!writer_put(&writer, UINT8_C(114))) return EXIT_FAILURE;
    if (printf("C 114 writer byte: %u\n", (unsigned)bytes[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-114-bounded-message-assembly \
  examples/c/c-114-bounded-message-assembly.c
./c-114-bounded-message-assembly
```

Odotettu tulos:

```text
C 114 writer byte: 114
```

## Ohjattu harjoitus

1. Tunnista tavupituus tai tilasopimus kunkin apurajapinnan kohdalla.
2. Etsi aikaisin tapahtuva ehto, joka hylkää virheellisen osoitteen, määrän, kapasiteetin tai arvon.
3. Selitä miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Määrittele milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen tehtävä

Lisää yksi deterministinen hyväksyttävä tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavulaskelmat, säilytä alkuperäinen tuloste hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokaatiosuuruuden laskemista. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritettavien lataajaksi tai mielivaltaisen koodin ajoa mahdollistavaksi mekanismiksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-114-bounded-message-assembly.sh
```

Käännä ja suorita sanitisaattoreilla tuetuissa ympäristöissä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-114-sanitized \
  examples/c/c-114-bounded-message-assembly.c
./c-114-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos voit kertoa syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää tuloksen ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Lasketun tavurajapinnan kanssa tarvitaan osoitin ja eksplisiittinen pituus, koska sisäiset nollabytet ovat kelvollista dataa.
2. Eteenpäin etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin se muuttaa ulostuloparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
