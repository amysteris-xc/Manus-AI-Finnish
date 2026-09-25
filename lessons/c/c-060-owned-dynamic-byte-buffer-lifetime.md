# C 060 — Omistetun dynaamisen tavupuskurin elinkaari

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–059
**Esimerkkiohjelma:** [`examples/c/c-060-owned-dynamic-byte-buffer-lifetime.c`](../../examples/c/c-060-owned-dynamic-byte-buffer-lifetime.c)
**Deterministinen testi:** [`tests/c-060-owned-dynamic-byte-buffer-lifetime.sh`](../../tests/c-060-owned-dynamic-byte-buffer-lifetime.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Oppitunnin lopussa osaat varata, alustaa ja vapauttaa pienen omistetun tavupuskurin siten, että koon määritykset huomioivat ylivuodon.

## Muistiharjoitus

1. Miksi tavupuskurin täytyy kantaa eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto todistaa, että indeksi on `count`-alkioisessa puskurissa?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen kuin sen tulosta käytetään.
4. Muista yksi tarkka lause edellisestä venäläisestä oppitunnista.

## Keskeinen käsite

Omistajuus on eksplisiittistä: onnistuneella varauksella on yksi vapautuspolku, ja varauksen koot tarkistetaan ennen muistin pyytämistä.

Seurantaohjelma käsittelee raakatavut laskettuina datoina. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Se on kannettava ISO C17 -yhteensopiva ja jättää tahallisesti pois shellcoden, natiivikoodin lataimet, mielivaltaisen assembly-koodin suorituksen, prosessiin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_bounded(const uint8_t *src, size_t len, uint8_t *dst, size_t cap)
{
    if ((src == NULL && len != 0) || dst == NULL || len > cap) return 0;
    if (len != 0) memmove(dst, src, len);
    return 1;
}

int main(void)
{
    const uint8_t source[] = {UINT8_C(0), UINT8_C(60), UINT8_C(255)};
    uint8_t destination[3] = {0};
    if (!copy_bounded(source, sizeof source, destination, sizeof destination)) return EXIT_FAILURE;
    if (printf("C 060 copied bytes: %u %u %u\n", (unsigned)destination[0], (unsigned)destination[1], (unsigned)destination[2]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-060-owned-dynamic-byte-buffer-lifetime \
  examples/c/c-060-owned-dynamic-byte-buffer-lifetime.c
./c-060-owned-dynamic-byte-buffer-lifetime
```

Odotettu tulostus:

```text
C 060 copied bytes: 0 60 255
```

## Ohjattu harjoitus

1. Tunnista ohjelmasta kaikki osoitin-plus-pituus- tai puskurin-plus-kapasiteettisopimukset.
2. Etsi varhaisin hylätty viallinen syöte -ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raakatavupuskurin lopetusmerkkinä.
4. Mainitse mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyttä.

## Itsenäinen harjoitus

Lisää yksi lisä deterministinen validi syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkastukset, eksplisiittiset pituudet ja tulosteen-paluuarvon tarkistukset. Jos muutat varauksen aritmetiikkaa, lisää tarkistettu ylivuototarkistus ennen laskentaa.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-060-owned-dynamic-byte-buffer-lifetime.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-060-sanitized \
  examples/c/c-060-owned-dynamic-byte-buffer-lifetime.c
./c-060-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetustarkoitukseen tarkoitetut eheys- tai leikkahajaus-esimerkit oikean maailman kryptografisesta suojauksesta.

## Vastausvinkki: vastaukset ja malli

1. Raakatavut voivat sisältää `0x00`, joten sentinellin skannaus ei ole kelvollinen pituuden laskenta.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O-, varaus-, jäsentämis- ja muotoilutulokset on tarkistettava ennen niiden arvojen käyttämistä.
4. Validoinnin läpäisseessä ohjelmassa tulostustila muuttuu vain, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraavassa ajoitetussa oppitunnissa palataan venäläiseen opetuspolkuun.

## Viitteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöttö/ulostulo — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
