# C 025 — Tavujen suoratoistokäsittely rajatulla kertymällä

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–017 ja välittömästi edeltävät oppitunnit
**Seurantaohjelma:** [`examples/c/c-025-streaming-byte-processing.c`](../../examples/c/c-025-streaming-byte-processing.c)
**Deterministinen testi:** [`tests/c-025-streaming-byte-processing.sh`](../../tests/c-025-streaming-byte-processing.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä tämän jälkeen

Tämän oppitunnin suorittamisen jälkeen osaat käsitellä tavudataa pienissä paloissa siten, että pidät yllä tarkistettua juoksevaa lukumäärää ja summaa.

## Kertauskysymykset

1. Miksi tavupuskuri ei ole sama asia kuin C-merkkijono?
2. Ilmoita rajattu silmukkahakuehto `count` kelvollisille alkioille.
3. Miksi tiedosto-operaatioiden paluuarvot on tarkistettava?
4. Käännä yhdestä edellisestä venäjänkielisestä oppitunnista yksi tuttu mallilause.

## Keskeinen käsite

Tässä esimerkissä tavut käsitellään laskettuina datoina. Se käyttää eksplisiittisiä `size_t` pituuksia, tarkistaa jokaisen ulkoisen tuloksen eikä riipu isäntäympäristön muistiasettelusta tai nollatavusta terminä. Koodi on siirrettävää ISO C17 -yhteensopivaa koodia, eikä se luo suoritettavia hyötykuormia, natiivikoodin lataajia, mielivaltaista konekielen suoritusta, prosessin injektiota tai itseään muokkaavaa koodia.

## Esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int consume(const uint8_t chunk[], size_t count, size_t *total, uint32_t *sum)
{
    if ((chunk == NULL && count != 0) || total == NULL || sum == NULL || SIZE_MAX - *total < count) return 0;
    for (size_t index = 0; index < count; ++index) {
        if (UINT32_MAX - *sum < chunk[index]) return 0;
        *sum += chunk[index];
    }
    *total += count; return 1;
}
int main(void)
{
    const uint8_t first[] = {UINT8_C(1),UINT8_C(2)};
    const uint8_t second[] = {UINT8_C(3),UINT8_C(0)};
    size_t total = 0; uint32_t sum = 0;
    if (!consume(first, sizeof first, &total, &sum) || !consume(second, sizeof second, &total, &sum)) return EXIT_FAILURE;
    if (printf("Bytes: %zu\nSum: %" PRIu32 "\n", total, sum) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita tarkalleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-025-streaming-byte-processing \
  examples/c/c-025-streaming-byte-processing.c
./c-025-streaming-byte-processing
```

Odotettu tulostus:

```text
Bytes: 4
Sum: 6
```

## Ohjattu harjoittelu

1. Määrittele ohjelman syöte- ja tulosrajapinnat.
2. Nimeä yksi laskentaan, kapasiteettiin, alueeseen tai paluuarvoihin liittyvä tarkistus.
3. Selitä, miksi upotetut `0x00` tavut pysyvät kelvollisina datana tässä.
4. Kuvaile yksi virheellinen tai ylisuuri syöte, joka tulisi hylätä.
5. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen tehtävä

Laajenna ohjelmaa yhdellä lisävalidoidulla arvolla tai testitapauksella. Säilytä eksplisiittiset määrät, vältä tarkistamattomia tyyppimuunnoksia ja validoi jokainen ulkoinen tulos ennen sen käyttämistä.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-025-streaming-byte-processing.sh
```

Jos tuetaan, käännä ja suorita myös sanitisaattoreiden kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-025-sanitized \
  examples/c/c-025-streaming-byte-processing.c
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajauksen, jokaisen olennaisen paluuarvotarkistuksen, `size_t` laskusopimuksen ja miksi tämä ohjelma ei ole kryptografinen turvamekanismi.

## Vinkki (spoileri): vastaukset ja mallivastaus

1. Tavupuskuri sisältää eksplisiittisen pituuden ja voi sisältää mitä tahansa tavuja, mukaan lukien `0x00`; C-merkkijono päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen etenemisehto on `index < count`.
3. I/O-operaatio voi olla osittaista tai epäonnistua, joten sen paluuarvo määrää, suorittiko se pyydetyn työn.
4. Kelvollinen vastaus nimeää tarkistuksen, joka huomioi null-osoittimen, kapasiteetin, aritmeettisen ylivuodon, osittaisen luku-/kirjoituksen tai virheellisen syötteen.
5. Esimerkki on datankäsittelyharjoitus, ei salaus, autentikointi, turvallinen tiiviste tai muu turvallisuuskontrolli.

## Seuraava oppitunti

Seuraava oppitunti jatkuu venäjänkielisellä polulla.

## Lähteet

[1]: https://en.cppreference.com/w/c/io/fread "fread-funktio — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite-funktio — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
