# C 041 — Tavuittainen XOR-maskaus ja purku

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–040 ja välittömästi edeltävät oppitunnit
**Seuralaisohjelma:** [`examples/c/c-041-byte-wise-xor-masking-and-unmasking.c`](../../examples/c/c-041-byte-wise-xor-masking-and-unmasking.c)
**Deterministinen testi:** [`tests/c-041-byte-wise-xor-masking-and-unmasking.sh`](../../tests/c-041-byte-wise-xor-masking-and-unmasking.sh)
**Tekijä:** Manus AI

## Mitä opit tekemään

Oppitunnin lopussa sinun pitäisi osata soveltaa syklistä monen tavun maskia syötepuskurille ja osoittaa sen symmetrisyys.

## Muistiharjoitus

1. Miksi tavupuskurit on pidettävä eksplisiittisillä `size_t` pituuksilla sen sijaan, että käytettäisiin päätearvoa?
2. Määrittele turvallinen läpikäyntiehtoehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosten paluuarvot on tarkistettava ennen kuin käytät tuloksia.
4. Muista yksi oikea lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuina raakatietoina. Siinä vaaditaan eksplisiittiset `size_t` kapasiteetit, varmistetaan kaikkien ulkoisten funktioiden paluuarvot ja ylläpidetään siirrettävyyttä ISO C17:n mukaisesti. Se välttää natiivilataimia, prosessi-injektiota, shellcodea ja itseään muuttavaa koodia.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void apply_mask(uint8_t *data, size_t len, const uint8_t *key, size_t key_len)
{
    if (data == NULL || key == NULL || key_len == 0) return;
    for (size_t i = 0; i < len; ++i) data[i] ^= key[i % key_len];
}

int main(void)
{
    uint8_t msg[] = {'H', 'E', 'L', 'L', 'O'};
    const uint8_t key[] = {0xAA, 0x55};
    apply_mask(msg, sizeof msg, key, sizeof key);
    apply_mask(msg, sizeof msg, key, sizeof key);
    if (printf("Restored: %c%c%c%c%c\n", msg[0], msg[1], msg[2], msg[3], msg[4]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-041-byte-wise-xor-masking-and-unmasking \
  examples/c/c-041-byte-wise-xor-masking-and-unmasking.c
./c-041-byte-wise-xor-masking-and-unmasking
```

Odotettu tulostus:

```text
Restored: HELLO
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostepuskurien rajat.
2. Etsi jokainen paluuarvon tarkistus ja kapasiteettitarkastus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisina datoina raakapuskureissa.
4. Nimeä yksi virheellisen syötteen tila, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan yksi lisäreunatapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen paluuarvo.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-041-byte-wise-xor-masking-and-unmasking.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, kun niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-041-sanitized \
  examples/c/c-041-byte-wise-xor-masking-and-unmasking.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos pystyt selittämään puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole operatiivinen kryptografinen turvamekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Raakatavupuskurit voivat sisältää `0x00` kelvollisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin kulkevan silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Virheelliset syötteet rikkovat kapasiteetti- tai rakennerajoituksia ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
