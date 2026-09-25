# C 022 — Tavuhistogrammit rajoitetuilla laskureilla

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–017 ja välittömästi edeltävät oppitunnit
**Lisäohjelma:** [`examples/c/c-022-byte-histograms-with-bounds.c`](../../examples/c/c-022-byte-histograms-with-bounds.c)
**Deterministinen testi:** [`tests/c-022-byte-histograms-with-bounds.sh`](../../tests/c-022-byte-histograms-with-bounds.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat laskea valittujen tavujen esiintymistiheyden kiinteässä puskurissa käyttäen `size_t` indeksejä ja 256 alkion histogrammia.

## Muistiharjoitus

1. Miksi tavupuskuri ei ole sama kuin C-merkkijono?
2. Ilmoita silmukan rajausehto `count` kelvollisille alkioille.
3. Miksi tiedosto-operaatioiden palautusarvot on tarkistettava?
4. Käännä mallilause, joka esiintyi yhdellä edeltävistä venäjänkielisistä oppitunneista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettavana datana. Se käyttää eksplisiittisiä `size_t` pituuksia, tarkistaa jokaisen ulkoisen funktion palautusarvon ja välttää luottamasta isäntäalustan muistiasetteluun tai oletukseen nollatavusta. Ohjelma on kannettava ISO C17 -yhteensopiva eikä se luo suoritettavia latauksia, natiivikoodin lataajia, mielivaltaista assembly-suoritusta, prosessiin injektointia tai itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const uint8_t input[] = {UINT8_C(0),UINT8_C(0x41),UINT8_C(0xFF),UINT8_C(0x41),UINT8_C(0)};
    size_t counts[256] = {0};
    for (size_t index = 0; index < sizeof input / sizeof input[0]; ++index) ++counts[input[index]];
    if (printf("00: %zu\n41: %zu\nFF: %zu\n", counts[0], counts[UINT8_C(0x41)], counts[UINT8_C(0xFF)]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita tarkalleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-022-byte-histograms-with-bounds \
  examples/c/c-022-byte-histograms-with-bounds.c
./c-022-byte-histograms-with-bounds
```

Odotettu tuloste:

```text
00: 2
41: 2
FF: 1
```

## Ohjattu harjoitus

1. Määritä ohjelman sisääntulo- ja ulostulorajat.
2. Nimeä yksi laskentaan, kapasiteettiin, arvoalueeseen tai palautusarvoon liittyvä tarkistus.
3. Selitä, miksi upotetut `0x00` tavut säilyvät tässä kelvollisena datana.
4. Kuvaile yksi virheellinen tai liian suuri syöte, joka pitäisi hylätä.
5. Muista yksi täsmällinen lause välittömästi edeltävästä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä lisätyllä tarkistettavalla arvolla tai testitapauksella. Säilytä eksplisiittiset laskennat, vältä tarkistamattomia tyyppimuunnoksia ja validoi jokainen ulkoinen tulos ennen sen käyttöä.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-022-byte-histograms-with-bounds.sh
```

Sitten, jos ympäristö tukee sitä, käännä ja suorita sanitisaattoreiden kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-022-sanitized \
  examples/c/c-022-byte-histograms-with-bounds.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos voit selittää eksplisiittisen rajan, jokaisen asiaankuuluvan palautusarvon tarkistuksen, `size_t` laskentasopimuksen ja miksi tämä ohjelma ei ole kryptografinen suojausmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskurilla on eksplisiittinen pituus ja se voi sisältää mitä tahansa tavuarvoja, mukaan lukien `0x00`; C-merkkijono puolestaan päättyy ensimmäiseen nollatavuun.
2. Turvallinen eteenpäin-ehto on `index < count`.
3. Syöte- ja tulostotoiminnot voivat olla osittaisia tai epäonnistua; niiden palautusarvo kertoo, suoritettiinko pyydetty työ.
4. Kelvollinen vastaus mainitsee null-osoittimen, kapasiteetin, aritmeettisen ylivuodon, osittaisen luku-/kirjoitusoperaation tai virheellisen syötteen tarkistuksen.
5. Esimerkki on datankäsittelyharjoitus, ei salaus-, todennus-, turvallinen tiiviste- tai valvontamekanismi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
