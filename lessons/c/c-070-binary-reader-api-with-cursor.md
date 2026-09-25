# C 070 — Binäärilukijan API kursoria käyttäen

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–069
**Mukana oleva ohjelma:** [`examples/c/c-070-binary-reader-api-with-cursor.c`](../../examples/c/c-070-binary-reader-api-with-cursor.c)
**Deterministinen testi:** [`tests/c-070-binary-reader-api-with-cursor.sh`](../../tests/c-070-binary-reader-api-with-cursor.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Tämän oppitunnin lopussa osaat rakentaa pienen lukija-API:n, joka siirtää rajattua kursoria vasta onnistuneen lukutoiminnon jälkeen.

## Muistiharjoitus

1. Miksi tavutaulukolla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka pitää tarkistaa ennen sen tuloksen käyttöä.
4. Muista yksi tarkka lause edellisestä venäjänkielisestä oppitunnesta.

## Keskeinen käsite

Kursorin liike on sidottu onnistuneeseen rajatarkistukseen.

Mukana oleva ohjelma käsittelee raakatavuja laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan siirtämistä. Se on siirrettävä ISO C17 -ohjelma ja tarkoituksella poissulkee shellcoden, natiivikoodin lataajat, mielivaltaisen assemblerin suorittamisen, prosessin injektoinnin, itseään muokkaavan koodin sekä todellisen kryptografian toteutuksen.

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
    const uint8_t field[] = {UINT8_C(0), UINT8_C(70)};
    uint16_t value = 0;
    if (!read_u16_be(field, sizeof field, &value)) return EXIT_FAILURE;
    if (printf("C 070 decoded field: %" PRIu16 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-070-binary-reader-api-with-cursor \
  examples/c/c-070-binary-reader-api-with-cursor.c
./c-070-binary-reader-api-with-cursor
```

Odotettu tulostus:

```text
C 070 decoded field: 70
```

## Ohjattu harjoitus

1. Tunnista kaikki ohjelmassa esiintyvät osoitin-plus-pituus- tai puskurin-plus-kapasiteetti-sopimukset.
2. Löydä aikaisin tapahtuva hylätty virheellistä syötettä koskeva ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavutiedoston lopetuksena.
4. Ilmoita mikä tuloste kirjoitetaan vasta kun validointi on onnistunut.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi lisätty deterministinen kelvollinen syöte ja yksi hylätty raja-arvoinen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteen palautusarvojen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää ennen laskentaa tarkistettu ylivuotoehto.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-070-binary-reader-api-with-cursor.sh
```

Käännä ja suorita sanitisaattoreilla, jos tuki on saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-070-sanitized \
  examples/c/c-070-binary-reader-api-with-cursor.c
./c-070-sanitized
```

## Loppuarviointi

Olet valmis seuraavaan oppitunnin, jos osaat kertoa tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetukselliset eheys- tai leikkihajautusesimerkit oikeasta tuotantokryptografiasta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut saattavat sisältää `0x00`, joten sentinelliskannaus ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O:n, allokoinnin, jäsentämisen ja muotoilun tulokset on tarkistettava ennen niiden käytöstä.
4. Vahvistettu ohjelma muuttaa tulostilaa vasta kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö ja -ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
