# C 074 — FNV-1a Opetuksellinen hajautus ja testivektori

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–073
**Kumppaniohjelma:** [`examples/c/c-074-fnv1a-toy-hash-and-test-vector.c`](../../examples/c/c-074-fnv1a-toy-hash-and-test-vector.c)
**Deterministinen testi:** [`tests/c-074-fnv1a-toy-hash-and-test-vector.sh`](../../tests/c-074-fnv1a-toy-hash-and-test-vector.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat laskea opetuksellisen ei-kryptografisen FNV-1a-tavuhajautuksen ja varmistaa kiinteän testivektorin.

## Muistiharjoitus

1. Miksi tavupuskuriin pitää liittää eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

FNV-1a esitetään ainoastaan opetustarkoitukseen; älä käytä sitä salasanoihin, allekirjoituksiin tai vihamieliseen syötteeseen.

Kumppaniohjelma käsittelee raakatavut laskettuina tietona. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan muuttamista. Se on kannettava ISO C17 -ohjelma ja sulkee tahallisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembler-koodin suorittamisen, prosessiin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Deliberately non-cryptographic teaching hash; never use for real security. */
static uint32_t toy_fnv1a(const uint8_t *data, size_t len)
{
    uint32_t hash = UINT32_C(2166136261);
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i) {
        hash ^= data[i];
        hash *= UINT32_C(16777619);
    }
    return hash;
}

int main(void)
{
    const uint8_t data[] = {'L', '0', '1'};
    uint32_t value = toy_fnv1a(data, sizeof data);
    if (printf("C 074 toy hash: %08" PRIX32 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-074-fnv1a-toy-hash-and-test-vector \
  examples/c/c-074-fnv1a-toy-hash-and-test-vector.c
./c-074-fnv1a-toy-hash-and-test-vector
```

Odotettu tulostus:

```text
C 074 toy hash: 5B779590
```

## Ohjattu harjoitus

1. Etsi kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteetti -sopimukset ohjelmasta.
2. Paikanna aikaisin hylätty virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin päättymisenä.
4. Määritä, mikä tuloste kirjoitetaan vasta validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aiheena käsitellään hajauttamista tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi rajatapauksen hylätty syöte. Säilytä olemassa olevat argumenttitarkastukset, eksplisiittiset pituudet ja tulosteen paluuarvojen tarkistukset. Jos muutat allokointiin liittyvää aritmetiikkaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-074-fnv1a-toy-hash-and-test-vector.sh
```

Käännä ja suorita sanitisaattoreilla, kun se on tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-074-sanitized \
  examples/c/c-074-fnv1a-toy-hash-and-test-vector.c
./c-074-sanitized
```

## Viimeinen itsearviointi

Olet valmis siirtymään seuraavaan oppituntiin, jos osaat kuvata tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetukselliset eheys- tai leikkihajautusesimerkit soveltuvasta todellisen maailman kryptografisesta suojauksesta.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellihaku ei ole kelvollinen pituuden laskenta.
2. Etenemisen läpikäyntiehto on `index < count`.
3. I/O:n, varauksen, jäsentämisen ja muotoilun tulokset on tarkistettava ennen niiden tulosten käyttämistä.
4. Validioitu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset ovat täyttyneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliseen sarjaan.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen tiivistelmä — OWASP"
