# C 027 — Leikkimielinen sekoitus ja testivektorit

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–017 ja välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-027-toy-mixing-and-test-vectors.c`](../../examples/c/c-027-toy-mixing-and-test-vectors.c)
**Deterministinen testi:** [`tests/c-027-toy-mixing-and-test-vectors.sh`](../../tests/c-027-toy-mixing-and-test-vectors.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopussa sinun pitäisi osata laskea tahallisesti turvaton `uint32_t`-leikkimielinen sekoitustulos ja varmentaa se kiinteällä testivektorilla.

## Muistin palauttamisharjoitus

1. Miksi tavupuskuri ei ole sama asia kuin C-merkkijono?
2. Määritä rajattu silmukkaehto `count` -kelvollisille alkioille.
3. Miksi tiedosto-operaatioiden paluuarvot on tarkistettava?
4. Edellisestä venäjänkielisestä oppitunnista: käännä yksi tuttu mallilause.

## Keskeinen käsite

Tässä esimerkissä tavut käsitellään laskennallisena datana. Se käyttää eksplisiittisiä `size_t`-pituuksia, tarkistaa kaikki ulkoiset tulokset eikä nojaa isäntäjärjestelmän muistiasetteluun tai nollatavuun päättymismerkkinä. Se on siirrettävää ISO C17 -koodia, eikä se luo suoritettavia hyötykuormia, natiivikoodin lataajia, mielivaltaista assembly-koodin suoritusta, prosessin injektiota tai itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational only: this is not a cryptographic hash, MAC, cipher, or security control. */
static uint32_t toy_mix32(const uint8_t bytes[], size_t count)
{
    uint32_t state = UINT32_C(0x12345678);
    for (size_t index = 0; index < count; ++index) state = (state ^ bytes[index]) * UINT32_C(33) + UINT32_C(0x9E3779B9);
    return state;
}
int main(void)
{
    const uint8_t vector[] = {UINT8_C(1),UINT8_C(2),UINT8_C(3)};
    const uint32_t result = toy_mix32(vector, sizeof vector);
    if (printf("Toy mix: 0x%08" PRIX32 "\n", result) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita se täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-027-toy-mixing-and-test-vectors \
  examples/c/c-027-toy-mixing-and-test-vectors.c
./c-027-toy-mixing-and-test-vectors
```

Odotettu tulostus:

```text
Toy mix: 0x9279DE03\n
```

## Ohjattu harjoitus

1. Tunnista ohjelman sisääntulo- ja ulostulorajat.
2. Nimeä yksi laskenta-, kapasiteetti-, arvoalue- tai paluuarvotarkistus.
3. Selitä, miksi upotetut `0x00`-tavut säilyvät tässä kelvollisina datoina.
4. Kuvaile yksi virheellinen tai liian suuri syöte, joka tulisi hylätä.
5. Muista yksi tarkka lause välittömästi edeltäneestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä ylimääräisellä tarkistetulla arvolla tai testitapauksella. Säilytä eksplisiittiset lukumäärät, vältä tarkistamattomia tyyppimuunnoksia ja validoi kaikki ulkoiset tulokset ennen niiden käyttöä.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-027-toy-mixing-and-test-vectors.sh
```

Sitten, jos ympäristö tukee, käännä ja suorita sanitisaattoreilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-027-sanitized \
  examples/c/c-027-toy-mixing-and-test-vectors.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajan, jokaisen asiaankuuluvan paluuarvotarkistuksen, `size_t`-lukusopimuksen ja miksi tämä ohjelma ei ole kryptografinen turvamekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Tavupuskuri sisältää eksplisiittisen pituuden ja voi sisältää minkä tahansa tavuarvon, mukaan lukien `0x00`. C-merkkijono päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen eteenpäin-ehto on `index < count`.
3. I/O voi olla osittaista tai epäonnistua, joten sen paluuarvo määrää, suoritettiinko pyydetty työ loppuun.
4. Kelvollinen vastaus nimeää NULL-osoittimen, kapasiteetin, aritmeettisen ylivuodon, lyhyen luku-/kirjoitusoperaation tai virheellisen syötteen tarkistuksen.
5. Esimerkki on datankäsittelyharjoitus, ei salaus-, autentikointi-, turvallinen tiiviste- tai muu tietoturvakontrolli.

## Seuraava oppitunti

Seuraava oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
