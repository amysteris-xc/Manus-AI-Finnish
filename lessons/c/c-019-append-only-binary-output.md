# C 019 — Lisäystilainen binaarinen ulostulo

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–017 ja välittömästi edeltävät oppitunnit
**Esimerkkiohjelma:** [`examples/c/c-019-append-only-binary-output.c`](../../examples/c/c-019-append-only-binary-output.c)
**Deterministinen testi:** [`tests/c-019-append-only-binary-output.sh`](../../tests/c-019-append-only-binary-output.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen osaat lisätä kiinteän tavuerän tiedoston loppuun ja tarkistaa jokaisen kirjoitus- sekä sulkemistoiminnon onnistumisen.

## Kertauskysymykset

1. Miksi tavupuskuri ei ole sama asia kuin C-merkkijono?
2. Määritä rajoitettu silmukkaehto `count` kelvollisille alkioille.
3. Miksi tiedostotoimintojen paluuarvot on tarkistettava?
4. Käännä edellisestä venäjänkielisestä oppitunnista yksi tuttu mallilause.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuna datana. Se käyttää eksplisiittisiä `size_t` pituuksia, tarkistaa jokaisen ulkoisen toiminnon paluuarvon eikä nojaa isäntäalustan muistin asetteluun tai nollatavuun terminointimerkkinä. Se on kannettavaa ISO C17 -koodia eikä luo suoritettavia hyötykuormia, natiivikoodin lataajia, mielivaltaista assembly-koodin suorittamista, prosessiin injektointia tai itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    const uint8_t record[] = {UINT8_C(0x48),UINT8_C(0x45),UINT8_C(0x4C),UINT8_C(0x4C),UINT8_C(0x4F)};
    FILE *out;
    if (argc != 2) return EXIT_FAILURE;
    out = fopen(argv[1], "ab");
    if (out == NULL) return EXIT_FAILURE;
    if (fwrite(record, 1, sizeof record, out) != sizeof record) {
        (void)fclose(out);
        return EXIT_FAILURE;
    }
    {
        int flush_status = fflush(out);
        int close_status = fclose(out);

        if (flush_status != 0 || close_status != 0) return EXIT_FAILURE;
    }
    if (printf("Bytes appended: %zu\n", sizeof record) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita se täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-019-append-only-binary-output \
  examples/c/c-019-append-only-binary-output.c
./c-019-append-only-binary-output log.bin
```

Odotettu tuloste:

```text
Bytes appended: 5
```

## Ohjattu harjoitus

1. Tunnista ohjelman syöte- ja tulostusrajapinnat.
2. Nimeä yksi laskenta-, kapasiteetti-, alue- tai paluuarvoihin liittyvä tarkistus.
3. Selitä, miksi upotetut `0x00` tavut säilyvät kelvollisena datana tässä.
4. Kuvaile yksi virheellinen tai ylisuuri syöte, joka tulisi hylätä.
5. Muista yksi tarkka lause juuri edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä lisätestillä tai -tarkistuksella, joka validoi jonkin arvon tai testitapauksen. Säilytä eksplisiittiset määrät, vältä tarkistamattomia tyyppimuunnoksia ja validoi jokainen ulkoinen tulos ennen sen käyttöä.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-019-append-only-binary-output.sh
```

Sitten, missä tuettu, käännä ja suorita sanitizerien kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-019-sanitized \
  examples/c/c-019-append-only-binary-output.c
```

## Lopullinen tarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajoituksen, jokaisen asiaankuuluvan paluuarvotarkistuksen, `size_t` laskusopimuksen ja miksi tämä ohjelma ei ole kryptografinen turvallisuusmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskurilla on eksplisiittinen pituus ja se voi sisältää minkä tahansa tavuarvon, mukaan lukien `0x00`. C-merkkijono taas päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen etenemisehto on `index < count`.
3. I/O-operaatiot voivat olla osittaisia tai epäonnistua, joten niiden paluuarvo kertoo, suoritettinko pyydetty työ loppuun.
4. Kelvollinen vastaus mainitsee NULL-osoittimen, kapasiteetin, aritmeettisen ylivuodon, lyhyen luku-/kirjoitusoperaation tai virheellisen syötteen tarkistuksen.
5. Esimerkki on datankäsittelyharjoitus — ei salaus-, autentikointi-, hajautus- tai muu turvallisuusmekanismi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
