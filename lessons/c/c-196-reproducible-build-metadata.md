# C 196 — Toistettavien käännösmetatietojen tallentaminen

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–195
**Seurantaohjelma:** [`examples/c/c-196-reproducible-build-metadata.c`](../../examples/c/c-196-reproducible-build-metadata.c)
**Deterministinen testi:** [`tests/c-196-reproducible-build-metadata.sh`](../../tests/c-196-reproducible-build-metadata.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopussa sinun pitäisi pystyä tallentamaan salattomia käännösinputteja, kuten kääntäjän liput ja lähdekoodin versio, pieneen demonstraatioon.

## Muistiharjoitus

1. Miksi raaka tavumuisti pitää kantaa mukana eksplisiittisellä `size_t` pituudella?
2. Mikä silmukan ehto pitää etenevän indeksin `count` alkioisessa puskurissa?
3. Mitkä palautusarvot I/O-, varaus- tai jäsentämis-API:sta täytyy tarkistaa ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistusluku ei ole autentikointi aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Toistettavuus vaatii dokumentoidut inputit. Vältä aikaleimojen, absoluuttisten polkujen, tunnistetietojen tai järjestelmäkohtaisen satunnaisen datan upottamista artefakteihin.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavut laskettuna datana, tarkistaa jokaisen osoitteen, määrän, kapasiteetin, muunnoksen ja palautusarvon, johon esimerkki nojaa, ja säilyttää ulostilaa kunnes validointi onnistuu. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessi-injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Käytännön esimerkki

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
    const uint8_t input[] = {UINT8_C(196)};
    uint8_t value = 0;
    if (validate_one(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 196 validated byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja aja:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-196-reproducible-build-metadata \
  examples/c/c-196-reproducible-build-metadata.c
./c-196-reproducible-build-metadata
```

Odotettu tuloste:

```text
C 196 validated byte: 196
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus kunkin apurin rajapinnassa.
2. Löydä aikaisin hylkäävä ehto virheelliselle osoitteelle, määrälle, kapasiteetille tai arvolle.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallista dataa eikä loppumerkki.
4. Ilmoita, milloin ulosteen arvo tai sijainti voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen ulostulo hylkäyksessä ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua varauskoon laskentaa. Älä tee tästä opetus-esimerkistä kryptografiaa, suoritettavaa lataajaa tai mielivaltaisen koodin suoritusmekanismia.

## Varmistus

Aja deterministinen testi:

```sh
./tests/c-196-reproducible-build-metadata.sh
```

Käännä ja aja sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-196-sanitized \
  examples/c/c-196-reproducible-build-metadata.c
./c-196-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat ilmaista input-sopimuksen, tunnistaa hylätyn rajatapauksen, selittää ulostulon ja erottaa puolustavan tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja malli­sanaus

1. Lasketun tavun API käyttää osoitetta plus eksplisiittistä pituutta, koska upotetut nolla‑tavut ovat kelvollista dataa.
2. Etenevä läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa ulosteen parametria, kursoria tai tilakenttää.
4. Oikeellisuus edellyttää sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa Russian-kurssille.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
