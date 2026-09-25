# C 075 — Leikkijauhatus (toy hash) — lavinan havainnointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–074
**Seurantaohjelma:** [`examples/c/c-075-toy-hash-avalanche-observation.c`](../../examples/c/c-075-toy-hash-avalanche-observation.c)
**Deterministinen testi:** [`tests/c-075-toy-hash-avalanche-observation.sh`](../../tests/c-075-toy-hash-avalanche-observation.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Oppitunnin lopussa osaat havainnoida, kuinka pieni syötteen muutos voi muuttaa opettavaisen leikkijauhatus-/toy-hash-tuloksen ilman mitään väitettä todellisesta turvallisuudesta.

## Muistiharjoitus

1. Miksi tavupuskurin tulee kantaa eksplisiittinen `size_t` pituustieto sen sijaan, että luottaisi `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Mainitse yksi C-kirjaston palautusarvo, joka on tarkistettava ennen kuin sen tulosta käytetään.
4. Muista yksi täsmällinen lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Muutoksellinen ulostulo ei ole todiste kryptografisesta vahvuudesta; tarvitaan vakiintuneita algoritmeja ja riippumatonta tarkastelua.

Seurantaohjelma käsittelee raakatavut hyödyntäen laskettua pituustietoa. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Ohjelma on kannettavaa ISO C17 -tasoa ja jättää tahallaan pois shellkoodin, natiivikoodin lataajat, mielivaltaisen assembly-koodin suorittamisen, prosessin injektoinnin, itseään muokkaavan koodin ja todelliset kryptografiset toteutukset.

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
    if (printf("C 075 toy hash: %08" PRIX32 "\n", value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-075-toy-hash-avalanche-observation \
  examples/c/c-075-toy-hash-avalanche-observation.c
./c-075-toy-hash-avalanche-observation
```

Odotettu tuloste:

```text
C 075 toy hash: 5B779590
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskuri-plus-kapasiteetti -sopimukset.
2. Etsi aikaisin hylätty virheellistä syötettä koskeva ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` tavupuskurin päätepisteenä.
4. Ilmoita, mikä ulostulo kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotasoista kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyden tarkistusta.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen kelvollinen syöte ja yksi hylätty raja-arvosyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituustiedot ja ulostulon palautustarkistukset. Jos muutat mitään allokointilaskentaa, lisää ylivuodon tarkistus ennen laskutoimitusta.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-075-toy-hash-avalanche-observation.sh
```

Käännä ja suorita sanitisaattoreilla, jos ympäristö tukee niitä:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-075-sanitized \
  examples/c/c-075-toy-hash-avalanche-observation.c
./c-075-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää ulostulon ja erottaa opettavaiset eheys- tai leikkijauhatus-esimerkit soveltuvasta todellisesta kryptografisesta suojauksesta.

## Vastausvinkit ja malli

1. Raakatavut voivat sisältää `0x00`, joten sentinellin etsiminen ei ole kelvollinen pituuden laskentatapa.
2. Eteenpäin kulun ehto on `index < count`.
3. I/O:n, varauksen, jäsentämisen ja formatoinnin palautusarvot on tarkistettava ennen niiden tuottamien tietojen käyttöä.
4. Vahvistettu ohjelma muuttaa tulostilaa vasta, kun kaikki edellytykset on suoritettu onnistuneesti.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
