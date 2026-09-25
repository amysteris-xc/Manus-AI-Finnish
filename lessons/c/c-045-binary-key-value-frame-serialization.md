# C 045 — binaarisen avain–arvo-kehyksen serialisointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–044 ja sitä välittömästi edeltävät oppitunnit
**Mukana oleva ohjelma:** [`examples/c/c-045-binary-key-value-frame-serialization.c`](../../examples/c/c-045-binary-key-value-frame-serialization.c)
**Deterministinen testi:** [`tests/c-045-binary-key-value-frame-serialization.sh`](../../tests/c-045-binary-key-value-frame-serialization.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopuksi sinun pitäisi pystyä serialisoimaan tunniste-pituus-arvo (TLV) -kehys tavujonoksi siten, että tehdään tarvittavat rajatarkistukset.

## Kertaus

1. Miksi tavubufferit on annettava eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin sentinel-päättymistä?
2. Määrittele turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosfunktioiden paluuarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi tarkka lause edeltävästä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuna raakatietona. Se edellyttää eksplisiittisiä `size_t` kapasiteetteja, tarkistaa kaikkien ulkoisten funktioiden paluuarvot ja säilyttää siirrettävyyden ISO C17:n puitteissa. Se välttää natiiveja lataajia, prosessien injektiota, shellcodeja ja itseään muuntavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_tlv(uint8_t tag, const uint8_t *val, uint8_t val_len, uint8_t *out, size_t cap, size_t *written)
{
    if (val == NULL && val_len != 0) return 0;
    if (out == NULL || written == NULL || cap < (size_t)(val_len + 2)) return 0;
    out[0] = tag;
    out[1] = val_len;
    for (size_t i = 0; i < val_len; ++i) out[2 + i] = val[i];
    *written = val_len + 2;
    return 1;
}

int main(void)
{
    uint8_t frame[10];
    const uint8_t payload[] = {0xDE, 0xAD};
    size_t used = 0;
    if (!write_tlv(0x01, payload, sizeof payload, frame, sizeof frame, &used)) return EXIT_FAILURE;
    if (printf("TLV written: %zu bytes\n", used) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-045-binary-key-value-frame-serialization \
  examples/c/c-045-binary-key-value-frame-serialization.c
./c-045-binary-key-value-frame-serialization
```

Odotettu tulostus:

```text
TLV written: 4 bytes
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostebufferien rajat.
2. Etsi jokainen paluuarvo- ja kapasiteettitarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisina datana raakatavuissa.
4. Nimeä yksi virheelliseen syötteeseen liittyvä ehto, joka aiheuttaa ennenaikaisen virhepalautuksen.
5. Muista yksi lause edeltävästä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan jokin lisäraja- tai reunaehto tai lisää testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-045-binary-key-value-frame-serialization.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-045-sanitized \
  examples/c/c-045-binary-key-value-frame-serialization.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää bufferisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole toiminnallinen kryptografinen turvaratkaisu.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavubufferit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin käyden silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Vialliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden tulee palauttaa virhekoodi.

## Seuraava oppitunti

Seuraavassa aikataulutetussa oppitunnissa palataan venäjänkieliseen sarjaan.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed width integer types — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Arithmetic operators — cppreference.com"
