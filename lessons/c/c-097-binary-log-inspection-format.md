# C 097 — Binaarilokin tarkastusmuoto

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–096
**Kumppaniohjelma:** [`examples/c/c-097-binary-log-inspection-format.c`](../../examples/c/c-097-binary-log-inspection-format.c)
**Deterministinen testi:** [`tests/c-097-binary-log-inspection-format.sh`](../../tests/c-097-binary-log-inspection-format.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopuksi osaat tarkastaa rajatun opetuksellisen binaarilokimerkinnän ja tulostaa sen kentät turvallisesti.

## Kertauskysymykset

1. Miksi tavupuskurin on sisällettävä eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto osoittaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston paluuarvo, joka on tarkistettava ennen sen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Tarkastustuloksen on perustuttava validoituihin tavuihin ja eksplisiittisiin kenttäleveyksiin.

Seuralaisohjelma käsittelee raakatavut pituudella merkittynä datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan etenemistä. Ohjelma on siirrettävä ISO C17 -koodi ja se jättää tietoisesti pois shellcoden, natiivikoodin lataajat, mielivaltaisen assembly-koodin suorituksen, prosessin injektoinnin, itseään muokkaavan koodin ja todellisen kryptografisen toteutuksen.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int print_hex(const uint8_t *data, size_t len)
{
    if (data == NULL && len != 0) return 0;
    for (size_t i = 0; i < len; ++i)
        if (printf("%02" PRIX8, data[i]) < 0) return 0;
    return 1;
}

int main(void)
{
    const uint8_t data[] = {UINT8_C(0), UINT8_C(97), UINT8_C(255)};
    if (printf("C 097 hex: ") < 0 || !print_hex(data, sizeof data) || printf("\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-097-binary-log-inspection-format \
  examples/c/c-097-binary-log-inspection-format.c
./c-097-binary-log-inspection-format
```

Odotettu tuloste:

```text
C 097 hex: 0061FF
```

## Ohjattu harjoitus

1. Etsi ohjelmasta kaikki osoitin+pituus- tai puskurin+kapasiteetin sopimukset.
2. Paikanna aikaisin tapahtuva hylättävä virheellisen syötteen ehto.
3. Selitä, miksi ohjelma ei pidä `0x00` raakatavupuskurin päätepisteenä.
4. Ilmoita, mikä tuloste kirjoitetaan vasta, kun validointi onnistuu.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aiheena olisi hajautus tai eheystarkastus — kyse on opetusesimerkistä, ei todellisesta suojaustoteutuksesta.

## Itsenäinen harjoitus

Lisää yksi lisädeterministinen kelvollinen syöte ja yksi hylätty rajasyöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tuloste‑paluuarvo‑tarkistukset. Jos muutat mitään allokointilaskentaa, lisää tarkistettu ylivuotoehto ennen laskentaa.

## Varmennus

Suorita deterministinen testi:

```sh
./tests/c-097-binary-log-inspection-format.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-097-sanitized \
  examples/c/c-097-binary-log-inspection-format.c
./c-097-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat määritellä tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen ja erottaa opetusesimerkit tai leikkimieliset hajautustoteutukset todellisesta tuotantotason kryptografiasta.

## Vastausvinkit: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten lopetusmerkkiä etsivä skannaus ei kelpaa pituuden laskemiseen.
2. Eteenpäin kulun ehto on `index < count`.
3. I/O:n, allokoinnin, jäsentämisen ja muotoilun tulokset on tarkistettava ennen kuin niiden tuloksia käytetään.
4. Validoitu ohjelma muuttaa ulostilaa vasta, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte/ulosotto — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen pikaopas — OWASP"
