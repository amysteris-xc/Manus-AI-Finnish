# C 124 — Ownership Transfer Contracts

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–123
**Kumppaniohjelma:** [`examples/c/c-124-ownership-transfer-contracts.c`](../../examples/c/c-124-ownership-transfer-contracts.c)
**Deterministinen testi:** [`tests/c-124-ownership-transfer-contracts.sh`](../../tests/c-124-ownership-transfer-contracts.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä tämän oppitunnin jälkeen

Tämän oppitunnin lopussa sinun pitäisi osata dokumentoida yksinkertainen omistajuuden siirtopäätös ilman että puskurista vuotaa tietoa tai sitä vapautetaan kahteen kertaan.

## Palautusharjoitus

1. Miksi raakabufferilla on oltava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää eteenpäin etenevän indeksin `count`-alkioisen puskurin sisällä?
3. Mitkä I/O-, allokointi- tai jäsentämis-APIen paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele miksi ei-kryptografinen tarkistusarvo ei ole autentikaatioa aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Omistajuuden on oltava eksplisiittinen jokaisella paluutien polulla. Kutsujan pitää tietää, omistaako se puskurin, lainaa sitä vai ei saa mitään puskuria.

Tämä oppitunti käyttää kannateltavaa ISO C17:ää. Se käsittelee tavujoukkoa laskettavana datana, tarkistaa jokaisen osoittimen, laskurin, kapasiteetin, muunnoksen ja esimerkin luottamuksen arvoon vaikuttavan palautusarvon, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataimia, mielivaltaista asm-suoritusta, prosessin injektiota, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int checked_count_bytes(size_t count, size_t item_size, size_t *out)
{
    if (out == NULL || item_size == 0 || count > SIZE_MAX / item_size) return 0;
    *out = count * item_size;
    return 1;
}

int main(void)
{
    size_t bytes = 0;
    if (!checked_count_bytes((size_t)124, sizeof(uint8_t), &bytes)) return EXIT_FAILURE;
    if (printf("C 124 checked bytes: %zu\n", bytes) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-124-ownership-transfer-contracts \
  examples/c/c-124-ownership-transfer-contracts.c
./c-124-ownership-transfer-contracts
```

Odotettu tuloste:

```text
C 124 checked bytes: 124
```

## Ohjattu harjoitus

1. Tunnista tavujänne tai tilasopimus jokaisessa apurajan kohdassa.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoittimen, laskurin, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Määrittele milloin lähtöarvo tai -asema saa muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdessä täsmällisessä lauseessa.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapauksen. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen lähtö hylkäyksen sattuessa, ja lisää tarkistettu aritmeettinen ehto ennen minkään muuttuneen allokaation koon laskemista. Älä muuta tätä opetusesimerkkiä kryptografiseksi, suoritinlataimeksi tai mielivaltaisen koodin suoritusalustaksi.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-124-ownership-transfer-contracts.sh
```

Käännä ja ajoita sanitisaattoreilla, jos tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-124-sanitized \
  examples/c/c-124-ownership-transfer-contracts.c
./c-124-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat kuvata syötteen sopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähdön ja erottaa puolustavan tavukäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavua API käyttää osoitteen lisäksi eksplisiittistä pituutta, koska upotetut nollatavut ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apuri validoi ennen kuin muuttaa lähtöparametria, kursoria tai tilakenttää.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkistusta; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
