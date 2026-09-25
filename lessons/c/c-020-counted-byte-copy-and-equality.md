# C 020 — Lasketun tavun kopiointi ja vertailu

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–017 ja välittömät edeltävät oppitunnit
**Kumppaniohjelma:** [`examples/c/c-020-counted-byte-copy-and-equality.c`](../../examples/c/c-020-counted-byte-copy-and-equality.c)
**Deterministinen testi:** [`tests/c-020-counted-byte-copy-and-equality.sh`](../../tests/c-020-counted-byte-copy-and-equality.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnon jälkeen osaat kopioida ja vertailla tavubuffereita käyttäen eksplisiittisiä pituusparametreja siten, että nollaarvoiset tavut eivät toimi merkkijonon päättymisen indikaattoreina.

## Kertauskysymykset

1. Miksi tavubufferi ei ole sama asia kuin C-merkkijono?
2. Mikä on rajoitettu silmukkaehto `count` kelvollisille alkioille?
3. Miksi tiedostotoimintojen paluuarvot täytyy tarkistaa?
4. Käännä edelliseltä venäjänkieliseltä oppitunnilta yksi tuttu mallilause.

## Keskeinen käsite

Esimerkissä käsitellään tavuja laskettuna datana. Se käyttää eksplisiittisiä `size_t` pituuksia, tarkistaa kaikki ulkoiset paluuarvot ja välttää riippuvuuksia isäntäjärjestelmän muistiasettelusta tai nollatavusta terminä. Kyseessä on siirrettävä ISO C17 -koodi; se ei luo suoritettavia payloadeja, natiivikoodin lataajia, mielivaltaista assembly-koodin suorittamista, prosessi-injektiota tai itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int copy_bytes(uint8_t out[], size_t out_count, const uint8_t in[], size_t count)
{
    if ((out == NULL || in == NULL) && count != 0) return 0;
    if (count > out_count) return 0;
    for (size_t index = 0; index < count; ++index) out[index] = in[index];
    return 1;
}
static int equal_bytes(const uint8_t a[], const uint8_t b[], size_t count)
{
    if ((a == NULL || b == NULL) && count != 0) return 0;
    for (size_t index = 0; index < count; ++index) if (a[index] != b[index]) return 0;
    return 1;
}
int main(void)
{
    const uint8_t source[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    uint8_t copy[3] = {0};
    if (!copy_bytes(copy, sizeof copy, source, sizeof source) || !equal_bytes(source, copy, sizeof source)) return EXIT_FAILURE;
    if (printf("Copied bytes: %zu\nEqual: yes\n", sizeof source) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-020-counted-byte-copy-and-equality \
  examples/c/c-020-counted-byte-copy-and-equality.c
./c-020-counted-byte-copy-and-equality
```

Odotettu tulostus:

```text
Copied bytes: 3
Equal: yes
```

## Ohjattu harjoittelu

1. Tunnista ohjelman syöte- ja tulorajat.
2. Nimeä yksi laskentoon, kapasiteettiin, arvoalueeseen tai paluuarvoon liittyvä tarkistus.
3. Selitä, miksi tähän upotetut `0x00` tavut säilyvät kelvollisina datana.
4. Kuvaile yksi virheellinen tai liian suuri syöte, joka tulisi hylätä.
5. Muista yksi täsmällinen lause välittömästi edeltävältä venäjänkieliseltä oppitunnilta.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä lisätarkistettavalla arvolla tai testitapauksella. Säilytä eksplisiittiset lukumäärät, vältä tarkistamattomia tyyppimuunnoksia ja validoi jokainen ulkoinen tulos ennen sen käyttöä.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-020-counted-byte-copy-and-equality.sh
```

Sitten, jos tuettu, käännä ja suorita sanitisaattoreiden kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-020-sanitized \
  examples/c/c-020-counted-byte-copy-and-equality.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt selittämään eksplisiittisen rajauksen, jokaisen asiaankuuluvan paluuarvotarkistuksen, `size_t` lukusitoumuksen ja miksi tämä ohjelma ei ole kryptografinen turvamekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Tavubufferilla on eksplisiittinen pituus ja se voi sisältää minkä tahansa tavuarvon, mukaan lukien `0x00`; C-merkkijono päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen eteenpäin-ehto on `index < count`.
3. Syöte-/tulostustoiminta voi olla osittaista tai epäonnistua, joten sen paluuarvo kertoo, suoritettiinko pyydetty työ loppuun.
4. Kelvollinen vastaus nimeää esimerkiksi null-osoittimen, kapasiteetin, aritmeettisen ylivuodon, lyhyen luku-/kirjoitusoperaation tai virheellisen syötteen tarkistuksen.
5. Esimerkki on datankäsittelyharjoitus — ei salaus, todennus, turvallinen tiiviste tai suojausmekanismi.

## Seuraava oppitunti

Seuraavaksi ajoitettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
