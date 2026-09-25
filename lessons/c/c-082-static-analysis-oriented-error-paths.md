# C 082 — Staattisen analyysin virhepolut

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–081
**Seuralaisohjelma:** [`examples/c/c-082-static-analysis-oriented-error-paths.c`](../../examples/c/c-082-static-analysis-oriented-error-paths.c)
**Deterministinen testi:** [`tests/c-082-static-analysis-oriented-error-paths.sh`](../../tests/c-082-static-analysis-oriented-error-paths.sh)
**Tekijä:** Manus AI

## Mitä osaat oppimisen jälkeen

Tämän oppitunnin lopussa osaat tehdä jäsenninvirhepolut eksplisiittisiksi niin, että kääntäjän diagnostiikka ja ihmisen tarkastus pystyvät seuraamaan niitä.

## Muistiharjoitus

1. Miksi tavutaulukolla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskuriin kuuluva?
3. Nimeä yksi C-kirjaston palautusarvo, joka pitää tarkistaa ennen kuin sen tulosta käytetään.
4. Muista yksi täsmällinen lause edellisestä venäläisestä oppitunnista.

## Keskeinen käsite

Luettavat virhepolut tekevät korrektiominaisuuksien tarkastelusta ja testaamisesta helpompaa.

Seuralaisohjelma käsittelee raakatavujoukkoja laskettuina tietoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan siirtoa. Ohjelma on kannettava ISO C17 -koodia ja jättää tahallaan pois shellkoodin, natiivikoodin lataimet, mielivaltaisen assemblyn ajon, prosessin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografian toteutuksen—se on opetus- ja analyysiesimerkki, ei tuotantotason suojaus.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { STATUS_OK = 0, STATUS_ARGUMENT = 1, STATUS_FORMAT = 2 } Status;

static Status validate_one_byte(const uint8_t *data, size_t len, uint8_t *out)
{
    if (data == NULL || out == NULL) return STATUS_ARGUMENT;
    if (len != 1) return STATUS_FORMAT;
    *out = data[0];
    return STATUS_OK;
}

int main(void)
{
    const uint8_t input[] = {UINT8_C(82)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 082 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-082-static-analysis-oriented-error-paths \
  examples/c/c-082-static-analysis-oriented-error-paths.c
./c-082-static-analysis-oriented-error-paths
```

Odotettu tuloste:

```text
C 082 status-checked byte: 82
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- ja puskurin-plus-kapasiteetti-sopimukset.
2. Löydä aikaisin hylätty virheellinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin lopetuksena.
4. Ilmoita, mikä tuloste kirjoitetaan ainoastaan sen jälkeen, kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi deterministinen hyväksytty syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuksen palautustarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuototarkastus ennen laskentaa.

## Vahvistus

Suorita deterministinen testi:

```sh
./tests/c-082-static-analysis-oriented-error-paths.sh
```

Käännä ja aja sanitisaattoreilla siellä, missä tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-082-sanitized \
  examples/c/c-082-static-analysis-oriented-error-paths.c
./c-082-sanitized
```

## Lopputarkastus

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetusluonteiset eheys- tai hajautusesimerkit oikean, tuotantotason kryptografian vaatimuksista.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellikartoitus ei ole kelvollinen pituuden laskutapa.
2. Eteenpäin kulkeva läpikäyntiehtoyhtälö on `index < count`.
3. I/O-, allokointi-, jäsentäminen- ja muotoilutulokset pitää tarkistaa, ennen kuin niiden tuottamia arvoja käytetään.
4. Validattu ohjelma muuttaa ulostulotilaa vasta kaikkien edellytysten onnistuttua.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle opintopolulle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C:n tiedostojen syöte ja tulostus — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "Työkaluketjun koventamisen pikaopas C:lle — OWASP"
