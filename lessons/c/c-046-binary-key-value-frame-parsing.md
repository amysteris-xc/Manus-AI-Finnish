# C 046 — Binäärisen avain-arvo-kehyksen jäsentäminen

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–045 ja sitä välittömästi edeltävät oppitunnit
**Seuralaisohjelma:** [`examples/c/c-046-binary-key-value-frame-parsing.c`](../../examples/c/c-046-binary-key-value-frame-parsing.c)
**Deterministinen testi:** [`tests/c-046-binary-key-value-frame-parsing.sh`](../../tests/c-046-binary-key-value-frame-parsing.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän oppitunnin jälkeen

Tämän oppitunnin jälkeen sinun pitäisi osata jäsentää tunniste-pituus-arvo (tag-length-value, TLV) -kehys tavutaulukosta tiukkojen rajaehtojen tarkistuksella.

## Kertauskäytännöt

1. Miksi tavupuskurien on käytettävä eksplisiittisiä `size_t` pituuksia sentinel-päättymisen sijaan?
2. Muotoile turvallinen läpikäyntiehto `count` elementeille.
3. Selitä, miksi I/O- ja muunnosfunktioiden palautusarvot on tarkistettava ennen tulosten käyttöä.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuina raakadatana. Se vaatii eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää kannettavuuden ISO C17 -standardin mukaisesti. Se välttää natiivilataajia, prosessin injektiota, shellcodea ja itseään muokkaavaa koodia.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int read_tlv(const uint8_t *frame, size_t len, uint8_t *tag, const uint8_t **val, size_t *val_len)
{
    if (frame == NULL || tag == NULL || val == NULL || val_len == NULL || len < 2) return 0;
    *tag = frame[0];
    size_t l = frame[1];
    if (len - 2 < l) return 0;
    *val = frame + 2;
    *val_len = l;
    return 1;
}

int main(void)
{
    const uint8_t raw[] = {0x05, 0x02, 0xAA, 0xBB};
    uint8_t tag = 0;
    const uint8_t *val = NULL;
    size_t val_len = 0;
    if (!read_tlv(raw, sizeof raw, &tag, &val, &val_len)) return EXIT_FAILURE;
    if (printf("Tag: %" PRIu8 "\nVal length: %zu\n", tag, val_len) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-046-binary-key-value-frame-parsing \
  examples/c/c-046-binary-key-value-frame-parsing.c
./c-046-binary-key-value-frame-parsing
```

Odotettu tulostus:

```text
Tag: 5
Val length: 2
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen palautusarvon ja kapasiteetin tarkistus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisena datana raakapuskurissa.
4. Nimeä yksi virheellisten syötteiden ehto, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan jokin lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-046-binary-key-value-frame-parsing.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-046-sanitized \
  examples/c/c-046-binary-key-value-frame-parsing.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole operatiivinen kryptografinen turvamekanismi.

## Vihje: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` laillisena datana; `strlen` ei voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen virheeseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakenne-ehdot ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäläiselle polulle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
