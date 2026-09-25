# C 021 — Tarkastettu `uint32_t`-aritmetiikka

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–017 ja välittömästi edeltävät oppitunnit
**Esimerkkiohjelma:** [`examples/c/c-021-checked-uint32-arithmetic.c`](../../examples/c/c-021-checked-uint32-arithmetic.c)
**Deterministinen testi:** [`tests/c-021-checked-uint32-arithmetic.sh`](../../tests/c-021-checked-uint32-arithmetic.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin jälkeen osaat lisätä kaksi `uint32_t`-arvoa vain nimenomaisen ylivuototarkistuksen jälkeen.

## Muistiharjoitus

1. Miksi tavupuskuri ei ole sama asia kuin C-merkkijono?
2. Ilmoita sidotun silmukan ehto `count` kelvolliselle alkiomäärälle.
3. Miksi tiedosto-operaatioiden paluuarvot pitää tarkistaa?
4. Käännä edellisestä venäjänkielisestä oppitunnista yksi tuttu mallilausuma.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuna datana. Se käyttää eksplisiittisiä `size_t`-pituuksia, tarkistaa jokaisen ulkopuolisen tuloksen ja välttää riippuvuutta isäntäjärjestelmän tavujärjestyksestä (endianness) tai nollatavusta terminointina. Se on kannettava, ISO C17 -yhteensopiva eikä luo suoritettavia hyötykuormia, natiivikoodin lataajia, mielivaltaista konekielisuoritusta, prosessin injektiota tai itseään muuttavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int add_u32(uint32_t left, uint32_t right, uint32_t *out)
{
    if (out == NULL || UINT32_MAX - left < right) return 0;
    *out = left + right;
    return 1;
}
int main(void)
{
    uint32_t result;
    if (!add_u32(UINT32_C(4000000000), UINT32_C(12), &result)) return EXIT_FAILURE;
    if (printf("Sum: %" PRIu32 "\nOverflow rejected: %s\n", result, add_u32(UINT32_MAX, UINT32_C(1), &result) ? "no" : "yes") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita se täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-021-checked-uint32-arithmetic \
  examples/c/c-021-checked-uint32-arithmetic.c
./c-021-checked-uint32-arithmetic
```

Odotettu tulos:

```text
Sum: 4000000012
Overflow rejected: yes
```

## Ohjattu harjoitus

1. Määritä ohjelman sisään- ja ulostulon rajat.
2. Nimeä yksi määrä-, kapasiteetti-, alue- tai paluuarvotarkistus.
3. Selitä, miksi upotetut `0x00`-tavut säilyvät tässä kelvollisena datana.
4. Kuvaile yksi virheellinen tai liian suuri syöte, joka pitäisi hylätä.
5. Muista yksi täsmällinen lause välittömästi edeltävästä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä lisätarkastetulla arvolla tai testitapauksella. Säilytä eksplisiittiset määrät, vältä tarkistamattomia tyyppimuunnoksia ja validoi jokainen ulkopuolinen tulos ennen sen käyttämistä.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-021-checked-uint32-arithmetic.sh
```

Sitten, jos tuettu, käännä ja suorita sanitisaattoreilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-021-sanitized \
  examples/c/c-021-checked-uint32-arithmetic.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajan, jokaisen asiaan kuuluvan paluuarvotarkistuksen, `size_t`-määräsopimuksen ja miksi tämä ohjelma ei ole kryptografinen turvallisuusmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskuriilla on eksplisiittinen pituus ja se voi sisältää minkä tahansa tavuarvon, mukaan lukien `0x00`; C-merkkijono päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen eteenpäin-ehto on `index < count`.
3. I/O voi olla osittaista tai epäonnistua, joten sen tulos määrittää, suoritettiinko pyydetty työ loppuun.
4. Kelvollinen vastaus nimeää null-osoittimen, kapasiteetin, aritmeettisen ylivuodon, lyhyen luku-/kirjoituksen tai virheellisen syötteen tarkistuksen.
5. Esimerkki on datankäsittelyharjoitus, ei salausta, autentikointia, turvallista hajautusta tai turvallisuusmekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen osioon.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
