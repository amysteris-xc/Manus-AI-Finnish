# C 086 — Tarkistussummat vs. autentikointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–085
**Seurantaohjelma:** [`examples/c/c-086-checksums-versus-authentication.c`](../../examples/c/c-086-checksums-versus-authentication.c)
**Deterministinen testi:** [`tests/c-086-checksums-versus-authentication.sh`](../../tests/c-086-checksums-versus-authentication.sh)
**Tekijä:** Manus AI

## Mitä osaat oppitunnin jälkeen

Tämän oppitunnin jälkeen osaat erottaa vahingossa tapahtuvan datan korruptiota havaitsevan tarkistussumman ja autentikoidun eheyden vaatimuksen.

## Muistiharjoitus

1. Miksi tavutaulukon tulee kantaa eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäjänkielisestä oppitunnesta.

## Keskeinen käsite

Tarkistussumma ei ole MAC eikä voi suojata aktiivista hyökkääjää vastaan.

Seurantaohjelma käsittelee raakatavujoukkoja laskettuna datana. Se tarkastaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Se on kannettava ISO C17 -ohjelma ja jättää tarkoituksellisesti pois shellkoodin, natiivikoodin lataajat, mielivaltaisen assembly-suorituksen, prosessin injisoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työnnetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Educational integrity check only: not a MAC or cryptographic hash. */
static uint32_t educational_crc(const uint8_t *data, size_t len)
{
    uint32_t crc = UINT32_C(0xFFFFFFFF);
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ ((crc & 1U) ? UINT32_C(0xEDB88320) : 0U);
    }
    return ~crc;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(1), UINT8_C(2), UINT8_C(3)};
    uint32_t value = educational_crc(data, sizeof data);
    if (printf("C 086 educational check: %08" PRIX32 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-086-checksums-versus-authentication \
  examples/c/c-086-checksums-versus-authentication.c
./c-086-checksums-versus-authentication
```

Odotettu tulostus:

```text
C 086 educational check: 55BC801D
```

## Ohjattu harjoitus

1. Tunnista kaikki ohjelman pointer-plus-length- tai buffer-plus-capacity-sopimukset.
2. Etsi aikaisin hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakabitin puskuriin päättymismerkkinä.
4. Ilmoita, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen kelvollinen syöte ja yksi hylätty rajatapaus. Säilytä nykyiset argumenttitarkistukset, eksplisiittiset pituudet ja tuloste-paluu -tarkistukset. Jos muutat mitään allokointilaskentaa, lisää laskentaa edeltävä tarkistettu ylivuotoehto.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-086-checksums-versus-authentication.sh
```

Käännä ja suorita sanitisaattoreilla, jos alusta tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-086-sanitized \
  examples/c/c-086-checksums-versus-authentication.c
./c-086-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat selittää tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetuksellisen eheyssesimerkin tai leikkimielisen hajautuksen todellisesta tuotantokelpoisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinel-skannaus ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulkuun liittyvä ehto on `index < count`.
3. I/O:n, allokoinnin, jäsentämisen ja muotoilun tulokset on tarkistettava ennen niiden tuottamien arvojen käyttöä.
4. Validointia läpäisevä ohjelma muuttaa ulostulotilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava ajoitettu oppitunti palaa venäjänkieliselle radalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte ja tulostus — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikavihje — OWASP"
