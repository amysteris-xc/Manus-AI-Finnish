# C 033 — Monikenttäisen binaaritietueen validointi

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esivaatimukset:** C 001–032 ja välittömästi edeltävät oppitunnit
**Lisäohjelma:** [`examples/c/c-033-multi-field-binary-record-validation.c`](../../examples/c/c-033-multi-field-binary-record-validation.c)
**Deterministinen testi:** [`tests/c-033-multi-field-binary-record-validation.sh`](../../tests/c-033-multi-field-binary-record-validation.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa sinun pitäisi osata validoida kiinteän 12 tavun structin, joka sisältää magic-tavut, version ja tarkistekentän.

## Muistiharjoitus

1. Miksi tavubuffereiden on käytettävä eksplisiittisiä `size_t` pituuksia sen sijaan, että käytettäisiin sentinel-terminaatiota?
2. Ilmoita turvallinen läpikäyntiehto `count` alkioille.
3. Selitä, miksi I/O- ja muunnosoperaatioiden paluuarvot on varmistettava ennen niiden tulosten käyttöä.
4. Muista yksi tarkka ilmaisu edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkissä tavut käsitellään laskettuna raakatietona. Se pakottaa eksplisiittiset `size_t` kapasiteetit, tarkistaa kaikkien ulkoisten funktioiden palautusarvot ja säilyttää siirrettävyyden ISO C17:n mukaisesti. Se välttää natiivilataajat, prosessin injektion, shellcoden ja itseään muokkaavan koodin.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int validate_record(const uint8_t *rec, size_t len)
{
    if (rec == NULL || len != 12) return 0;
    if (rec[0] != 'M' || rec[1] != 'A' || rec[2] != 'G' || rec[3] != 'C') return 0;
    uint16_t version = (uint16_t)(((uint16_t)rec[4] << 8) | rec[5]);
    if (version != 1) return 0;
    return 1;
}

int main(void)
{
    const uint8_t record[12] = {'M', 'A', 'G', 'C', 0, 1, 0, 0, 0, 0, 0, 0};
    if (!validate_record(record, sizeof record)) return EXIT_FAILURE;
    if (printf("Record valid: yes\n") < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-033-multi-field-binary-record-validation \
  examples/c/c-033-multi-field-binary-record-validation.c
./c-033-multi-field-binary-record-validation
```

Odotettu tuloste:

```text
Record valid: yes
```

## Ohjattu harjoitus

1. Tunnista syöte- ja tulostebuffereiden rajat.
2. Etsi jokainen palautusarvo ja kapasiteettitarkastus.
3. Selitä, miksi upotetut `0x00` arvot pysyvät kelvollisena datana raakapuskureissa.
4. Nimeä yksi virheellinen syöte-olosuhde, joka aiheuttaa varhaisen virhepalautuksen.
5. Muista yksi lause edellisestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa validoimaan lisätilaustapaus tai testisyöte. Säilytä eksplisiittiset pituudet ja tarkista jokainen palautustila.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-033-multi-field-binary-record-validation.sh
```

Käännä ja suorita sanitizerien kanssa, jos tuettu:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-033-sanitized \
  examples/c/c-033-multi-field-binary-record-validation.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää puskurisopimuksen, virhepalautusarvot ja miksi tämän oppitunnin koodi ei ole käytännöllinen kryptografinen turvamekanismi.

## Paljastus: vastaukset ja mallivastaus

1. Raakabuffereissa voi esiintyä `0x00` laillisena datana; `strlen` eivät voi määrittää niiden kokoa.
2. Eteenpäin silmukan raja: `index < count`.
3. Tarkistamattomat tulokset voivat johtaa alustamattoman muistin käyttöön tai hiljaiseen epäonnistumiseen.
4. Vialliset syötteet rikkovat kapasiteetti- tai rakenne-ehdot ja niiden on palautettava virhekoodi.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/language/operator_arithmetic "Aritmeettiset operaattorit — cppreference.com"
