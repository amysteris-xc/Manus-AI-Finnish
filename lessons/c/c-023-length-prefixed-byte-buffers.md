# C 023 — Pituusetuliitteelliset tavupuskurit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–017 ja välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-023-length-prefixed-byte-buffers.c`](../../examples/c/c-023-length-prefixed-byte-buffers.c)
**Deterministinen testi:** [`tests/c-023-length-prefixed-byte-buffers.sh`](../../tests/c-023-length-prefixed-byte-buffers.sh)
**Tekijä:** Manus AI

## Mitä osaat oppitunnin jälkeen

Tämän oppitunnin lopussa osaat enkoodata ja dekoodata tavujoukon, jonka edessä on kahden tavun big-endian-järjestyksessä oleva pituusetuliite, sekä suorittaa tiukat kapasiteetti- ja virhetarkistukset.

## Muistiharjoitus

1. Miksi tavupuskuri ei ole sama asia kuin C-merkkijono?
2. Ilmaise rajatun silmukan ehto `count` kelvolliselle alkiomäärälle.
3. Miksi tiedostotoimintojen paluuarvot on tarkistettava?
4. Käännä edellisestä venäjänkielisestä oppitunnista yksi tuttu mallilause.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuna datana. Se käyttää eksplisiittisiä `size_t` pituuksia, tarkistaa jokaisen ulkoisen funktion paluuarvon eikä nojaudu isäntäjärjestelmän muistiasetteluun tai nollatavuun terminä. Koodi on siirrettävissä ja ISO C17 -yhteensopiva; se ei luo suoritettavia tavukuormia, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessiin injektointia tai itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int encode(uint8_t out[], size_t capacity, const uint8_t payload[], size_t length, size_t *used)
{
    if (out == NULL || payload == NULL || used == NULL || length > UINT16_MAX || capacity < 2 || length > capacity - 2) return 0;
    out[0] = (uint8_t)(length >> 8); out[1] = (uint8_t)length;
    for (size_t index = 0; index < length; ++index) out[index + 2] = payload[index];
    *used = length + 2; return 1;
}
int main(void)
{
    const uint8_t payload[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    uint8_t frame[8]; size_t used;
    if (!encode(frame, sizeof frame, payload, sizeof payload, &used)) return EXIT_FAILURE;
    if (printf("Frame bytes: %zu\nLength: %u\n", used, (unsigned)(((unsigned)frame[0] << 8) | frame[1])) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita tarkalleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-023-length-prefixed-byte-buffers \
  examples/c/c-023-length-prefixed-byte-buffers.c
./c-023-length-prefixed-byte-buffers
```

Odotettu tulostus:

```text
Frame bytes: 5
Length: 3
```

## Ohjattu harjoitus

1. Tunnista ohjelman syöte- ja tuloserajat.
2. Nimeä yksi lasku-, kapasiteetti-, alue- tai paluuarvotarkistus.
3. Selvitä, miksi upotetut `0x00` tavut säilyvät tässä kelvollisena datana.
4. Kuvaile yksi väärinmuodostunut tai ylisuuri syöte, joka tulisi hylätä.
5. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen tehtävä

Laajenna ohjelmaa yhdellä lisätarkastetulla arvolla tai testitapauksella. Säilytä eksplisiittiset laskelmat, vältä tarkistamattomia tyypinmuunnoksia ja validoi jokainen ulkoinen tulos ennen sen käyttämistä.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-023-length-prefixed-byte-buffers.sh
```

Sitten, jos ympäristö tukee, käännä ja suorita sanitisaattoreiden kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-023-sanitized \
  examples/c/c-023-length-prefixed-byte-buffers.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajan, kaikki asiaankuuluvat paluuarvotarkistukset, `size_t` laskusopimuksen ja miksi tämä ohjelma ei ole kryptografinen turvallisuusmekanismi.

## Vastausvinkki: vastaukset ja mallivastaus

1. Tavupuskuriilla on eksplisiittinen pituus ja se voi sisältää minkä tahansa tavuarvon, mukaan lukien `0x00`; C-merkkijono päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen etenemisehto on `index < count`.
3. I/O voi olla osittaista tai epäonnistua, joten sen paluuarvo määrää, suoritettiinko pyydetty työ loppuun.
4. Kelvollinen vastaus mainitsee nollapointerin, kapasiteetin, aritmeettisen ylivuodon, osittaisen luku- tai kirjoitustoiminnon tai virheellisen syötteen tarkistuksen.
5. Esimerkki on datankäsittelyharjoitus — se ei tarjoa salausta, autentikointia, turvallista tiivistettä tai muuta turvallisuuskontrollia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen jaksoon.

## Lähteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.comin dokumentaatio"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.comin dokumentaatio"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
