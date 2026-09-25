# C 072 — Magic-tunniste ja versioneuvottelu

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–071
**Companion program:** [`examples/c/c-072-magic-and-version-negotiation.c`](../../examples/c/c-072-magic-and-version-negotiation.c)
**Deterministic test:** [`tests/c-072-magic-and-version-negotiation.sh`](../../tests/c-072-magic-and-version-negotiation.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopussa osaat tarkistaa tavumuodon magic-tunnisteen ja sallitun version ennen rungon käsittelyä.

## Muistiharjoitus

1. Miksi tavupuskurin on kannettava eksplisiittistä `size_t` pituutta sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Hylkää tuntemattomat magic-tunnisteet ja versiot ennen minkään myöhemmän formaattikentän hyväksymistä.

Companion-ohjelma käsittelee raakatavut laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan etenemistä. Se on kannettava ISO C17 -koodi ja sulkee tarkoituksella pois shellkoodin, natiivikoodin lataajat, mielivaltaisen assembly-koodin suorituksen, prosessin injektion, itseään muokkaavan koodin sekä tuotantotason kryptografian toteutuksen.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int read_u16_be(const uint8_t *data, size_t len, uint16_t *out)
{
    if (data == NULL || out == NULL || len < 2) return 0;
    *out = (uint16_t)(((uint16_t)data[0] << 8) | data[1]);
    return 1;
}

int main(void)
{
    const uint8_t field[] = {UINT8_C(0), UINT8_C(72)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 072 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-072-magic-and-version-negotiation \
  examples/c/c-072-magic-and-version-negotiation.c
./c-072-magic-and-version-negotiation
```

Odotettu tuloste:

```text
C 072 decoded field: 72
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteetti -sopimukset.
2. Etsi varhaisin ehto, joka hylkää virheellisen syötteen.
3. Selitä, miksi ohjelma ei pidä `0x00` raakaa tavupuskurin lopun merkkinä.
4. Mainitse, mikä tuloste kirjoitetaan vasta validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelisi hajautusta tai eheyden varmistusta.

## Itsenäinen harjoitus

Lisää yksi ylimääräinen deterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä nykyiset argumenttitarkistukset, eksplisiittiset pituudet ja ulostulon palautusarvojen tarkistukset. Jos muutat mitään allokointiin liittyvää aritmetiikkaa, lisää ennen laskentaa tarkistettu ylivuotoehto.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-072-magic-and-version-negotiation.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-072-sanitized \
  examples/c/c-072-magic-and-version-negotiation.c
./c-072-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt määrittelemään tavupuskurin sopimuksen, tunnistamaan hylätyn syötteen, selittämään tulosteen ja erottamaan opetus- tai leikkimieliset hajautusesimerkit soveltavasta todellisen maailman kryptografisesta suojauksesta.

## Ratkaisuvinkit ja mallivastaus

1. Raakadatan kohdalla voi esiintyä `0x00`, joten päätemerkkiä etsivä skannaus ei ole pätevä tapa laskea pituutta.
2. Etenemisen ehto on `index < count`.
3. I/O:n, allokoinnin, jäsentämisen ja muotoilun tulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Vahvistettu ohjelma muuttaa ulostulon tilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti palaa venäjänkieliseen linjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulostulo — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun kovettamisen pikaopas — OWASP"
