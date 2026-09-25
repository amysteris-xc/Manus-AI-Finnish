# C 111 — Kiinteän tietueen vertailijan sopimus

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–110
**Seuraava ohjelma:** [`examples/c/c-111-fixed-record-comparator-contract.c`](../../examples/c/c-111-fixed-record-comparator-contract.c)
**Deterministinen testi:** [`tests/c-111-fixed-record-comparator-contract.sh`](../../tests/c-111-fixed-record-comparator-contract.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat verrata kiinteän leveyden tavu­tietueita käyttämällä eksplisiittistä tietueen leveyttä ja lukumäärää.

## Muistiharjoitus

1. Miksi raakabytes‑puskurilla on oltava eksplisiittinen `size_t` pituus?
2. Mikä silmukkaehto pitää etenevän indeksin sisällä `count`-alkioisessa puskurissa?
3. Mitkä I/O-, varaus- tai jäsentämis‑API:en paluuarvot on tarkistettava ennen kuin niiden tuottamiin arvoihin luotetaan?
4. Perustele, miksi ei-kryptografinen tarkistus­arvo ei ole autentikointia aktiivista hyökkääjää vastaan.

## Keskeinen käsite

Vertailija tarvitsee vakaan leveyden, validoidut osoittimet ja dokumentoidun järjestyksen. Älä lue tietueen rajapinnan yli.

Tämä oppitunti käyttää kannatettavaa ISO C17:ää. Se käsittelee tavuja laskettavana datana, tarkistaa jokaisen osoittimen, lukumäärän, kapasiteetin, muunnoksen ja esimerkin vaatiman paluu­tuloksen, ja säilyttää lähtötilan kunnes validointi onnistuu. Se ei luo shellcodea, natiivikoodin lataajia, mielivaltaista assemblerin suoritusta, prosessiin injektointia, itseään muokkaavaa koodia tai todellisia kryptografisia toteutuksia.

## Työstetty esimerkki

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int find_byte(const uint8_t *data, size_t count, uint8_t target, size_t *out_index)
{
    if (data == NULL || out_index == NULL) return 0;
    for (size_t i = 0; i < count; ++i) {
        if (data[i] == target) { *out_index = i; return 1; }
    }
    return 0;
}

int main(void)
{
    const uint8_t table[] = {UINT8_C(1), UINT8_C(111), UINT8_C(250)};
    size_t index = 0;
    if (!find_byte(table, sizeof table, UINT8_C(111), &index)) return EXIT_FAILURE;
    if (printf("C 111 table index: %zu\n", index) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-111-fixed-record-comparator-contract \
  examples/c/c-111-fixed-record-comparator-contract.c
./c-111-fixed-record-comparator-contract
```

Odotettu tuloste:

```text
C 111 table index: 1
```

## Ohjattu harjoitus

1. Tunnista tavujen alue tai tilasopimus jokaisessa apufunktion rajapinnassa.
2. Etsi aikaisin mahdollinen ehto, joka hylkää virheellisen osoitteen, lukumäärän, kapasiteetin tai arvon.
3. Selitä, miksi `0x00` on tässä esimerkissä tavallinen data eikä loppumerkki.
4. Ilmoita, milloin lähtöarvo tai -asema voi muuttua.
5. Selitä oppitunnin ilmoitettu turvallisuusrajoitus yhdellä täsmällisellä lauseella.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty tapaus ja yksi hylätty rajatapaus. Pidä eksplisiittiset tavumäärät, säilytä alkuperäinen tuloste hylkäyksessä, ja lisää tarkistettu aritmeettinen ehto ennen mitä tahansa muutettua varauskoon laskentaa. Älä tee tästä opetusesimerkistä kryptografista, suoritinlataajaa tai mielivaltaisen koodin suorituksen mekanismia.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-111-fixed-record-comparator-contract.sh
```

Käännä ja suorita sanitisaattoreilla, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-111-sanitized \
  examples/c/c-111-fixed-record-comparator-contract.c
./c-111-sanitized
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos voit määritellä syöttösopimuksen, tunnistaa hylätyn rajatapauksen, selittää lähtöarvon ja erottaa defensiivisen tavunkäsittelyn todellisesta kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Laskettu tavuryhmä‑API käyttää osoitetta ja eksplisiittistä pituutta, koska upotetut nollatavuk­si ovat kelvollista dataa.
2. Eteenpäin kulkeva läpikäynti tarkistaa `index < count` ennen kuin lukee `data[index]`.
3. Apufunktio validoi ennen lähtöparametrin, kursorin tai tilakentän muuttamista.
4. Oikeellisuus vaatii sekä deterministisiä testejä että ilmoitettujen rajojen tarkastelua; mikään pieni esimerkki ei todista tuotantotason turvallisuutta.

## Seuraava oppitunti

Seuraavaksi ohjelmassa palataan venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed-Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
