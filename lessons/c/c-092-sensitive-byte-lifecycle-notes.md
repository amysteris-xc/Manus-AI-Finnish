# C 092 — Herkkien tavujen elinkaarimuistiinpanot

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–091
**Seuraohjelma:** [`examples/c/c-092-sensitive-byte-lifecycle-notes.c`](../../examples/c/c-092-sensitive-byte-lifecycle-notes.c)
**Deterministinen testi:** [`tests/c-092-sensitive-byte-lifecycle-notes.sh`](../../tests/c-092-sensitive-byte-lifecycle-notes.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin jälkeen sinun pitäisi pystyä erottamaan tavalliset tavupuskurit ja herkkä data sekä dokumentoimaan kannettavan pyyhinnän rajat.

## Muistiharjoitus

1. Miksi tavupuskurilla täytyy olla eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen sen tuloksen käyttämistä.
4. Muistatko yhden tarkan ilmauksen edellisestä venäjänkielisestä oppitunnista?

## Keskeinen käsite

Kannettava C ei voi yksin taata turvallista pyyhkimistä; oikeiden salaisten tietojen käsittelyssä noudata alustan ja kirjastojen vakiintunutta ohjeistusta.

Seuraohjelma käsittelee raakatavuja laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan etenemistä. Ohjelma on kannattavaa ISO C17:ää ja jättää tahallaan pois shellcoden, natiivikoodin lataimet, mielivaltaisen assemblyn suorituksen, prosessiin injektoinnin, itseään muuttavan koodin ja todellisen kryptografisen toteutuksen.

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
    const uint8_t input[] = {UINT8_C(92)};
    uint8_t value = 0;
    if (validate_one_byte(input, sizeof input, &value) != STATUS_OK) return EXIT_FAILURE;
    if (printf("C 092 status-checked byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-092-sensitive-byte-lifecycle-notes \
  examples/c/c-092-sensitive-byte-lifecycle-notes.c
./c-092-sensitive-byte-lifecycle-notes
```

Odotettu tuloste:

```text
C 092 status-checked byte: 92
```

## Ohjattu harjoittelu

1. Tunnista kaikki ohjelmassa esiintyvät osoitin-plus-pituus- tai puskuriplus-kapasiteetti-sopimukset.
2. Löydä aikaisin tapahtuva hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei kohtele `0x00` raakapunttipuskurin lopetusmerkkinä.
4. Ilmoita mikä tuloste kirjoitetaan vasta sen jälkeen, kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aihe käsittelisi hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen kelvollinen syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tuloste-palautus -tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskua.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-092-sensitive-byte-lifecycle-notes.sh
```

Käännä ja suorita sanitisaattoreilla, jos niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-092-sanitized \
  examples/c/c-092-sensitive-byte-lifecycle-notes.c
./c-092-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos osaat kuvata tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetusesimerkit todellisesta käytännön kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakatavut saattavat sisältää `0x00`, joten sentinel-haku ei ole kelvollinen pituuslaskenta.
2. Eteenpäin kulkeva läpikäyntiehtosi on `index < count`.
3. Syöte/ulosotto, muistivaraukset, jäsentäminen ja muotoilun tulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Vahvistettu ohjelma muuttaa ulostilaa vasta kaikkien edellytysten täytyttyä.

## Seuraava oppitunti

Seuraavassa aikataulutetussa oppitunnissa palataan venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/tuotos — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluputken koventamisen pikaopas — OWASP"
