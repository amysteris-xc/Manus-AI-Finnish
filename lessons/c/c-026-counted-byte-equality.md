# C 026 — Tavujen vertailu annetulla pituudella

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esitiedot:** C 001–017 ja välittömästi edeltävät oppitunnit
**Seuraava ohjelma:** [`examples/c/c-026-counted-byte-equality.c`](../../examples/c/c-026-counted-byte-equality.c)
**Deterministinen testi:** [`tests/c-026-counted-byte-equality.sh`](../../tests/c-026-counted-byte-equality.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä oppitunnin jälkeen

Oppitunnin jälkeen osaat verrata kahta tavupuskuria käyttäen nimenomaista pituutta ja ymmärrät, että upotetut nollatavut ovat kelvollista dataa tässä kontekstissa.

## Kertausharjoitus

1. Miksi tavupuskuri ei ole sama kuin C-merkkijono?
2. Ilmaise rajattu silmukkaehto `count` kelvollisille alkioille.
3. Miksi tiedosto-operaatioiden paluuarvot on tarkistettava?
4. Käännä edellisestä venäjänkielisestä oppitunnista yksi tuttu mallilause.

## Keskeinen käsite

Esimerkissä tavut käsitellään pituudella rajattuna datana. Ohjelma käyttää nimenomaisia `size_t` pituuksia, tarkistaa jokaisen ulkoisen operaation paluuarvon eikä nojaudu isäntämuistin järjestykseen tai oletuksiin nollatavun merkityksestä. Se on kannettava ISO C17 -ohjelma eikä luo suoritettavaa hyötykuormaa, lataa natiivikoodia, suorita mielivaltaista assembly-koodia, tee prosessien injektiota tai itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int equal(const uint8_t left[], const uint8_t right[], size_t count)
{
    if ((left == NULL || right == NULL) && count != 0) return 0;
    for (size_t index = 0; index < count; ++index) if (left[index] != right[index]) return 0;
    return 1;
}
int main(void)
{
    const uint8_t a[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    const uint8_t b[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    const uint8_t c[] = {UINT8_C(0x41),UINT8_C(0),UINT8_C(0x43)};
    if (printf("a equals b: %s\na equals c: %s\n", equal(a,b,sizeof a) ? "yes" : "no", equal(a,c,sizeof a) ? "yes" : "no") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita se täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-026-counted-byte-equality \
  examples/c/c-026-counted-byte-equality.c
./c-026-counted-byte-equality
```

Odotettu tuloste:

```text
a equals b: yes
a equals c: no
```

## Ohjattu harjoitus

1. Tunnista ohjelman syöte- ja tulostusrajapinnat.
2. Nimeä yksi lukumäärä-, kapasiteetti-, alue- tai paluuarvotarkistus.
3. Selitä, miksi upotetut `0x00` tavut pysyvät tässä kelvollisena datana.
4. Kuvaile yksi virheellinen tai ylisuuri syöte, joka tulisi hylätä.
5. Muista yksi tarkka lause välittömästi edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä lisätarkistuksella tai testitapauksella. Säilytä eksplisiittiset määrät, vältä tarkistamattomia tyyppimuunnoksia ja validoi kaikki ulkoiset paluuarvot ennen niiden käyttöä.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-026-counted-byte-equality.sh
```

Jos ympäristö tukee, käännä ja suorita myös sanitisaattoreilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-026-sanitized \
  examples/c/c-026-counted-byte-equality.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajan, jokaisen asiaankuuluvan paluuarvotarkistuksen, `size_t` lukusopimuksen ja miksi tämä ohjelma ei ole kryptografinen turvamekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskurilla on nimenomainen pituus ja se voi sisältää minkä tahansa tavuarvon, mukaan lukien `0x00`; C-merkkijono puolestaan päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen etenemisehto on `index < count`.
3. Syöte- ja tulostotoiminnot voivat olla osittaisia tai epäonnistua, joten niiden paluuarvo kertoo, suoritettiinko pyydetty työ loppuun.
4. Kelvollinen vastaus mainitsee esimerkiksi null-osoittimen, kapasiteetin, aritmeettisen ylivuodon, osittaisen luku-/kirjoitusoperaation tai virheellisen syötteen tarkistuksen.
5. Tämä esimerkki on datankäsittelyharjoitus; se ei ole salaus-, autentikointi-, turvallinen hajautus- tai muu turvallisuuskontrolli.

## Seuraava oppitunti

Seuraavassa oppitunnissa palataan venäjänkieliseen osuuteen.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Fixed width integer types — cppreference.com"
