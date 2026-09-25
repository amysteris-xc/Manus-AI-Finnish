# C 083 — Siirrettävä tiedostohygienia ja siivous

**Arvioitu oppimisaika:** 25–35 minuuttia
**Esitiedot:** C 001–082
**Mukana oleva ohjelma:** [`examples/c/c-083-portable-file-hygiene-and-cleanup.c`](../../examples/c/c-083-portable-file-hygiene-and-cleanup.c)
**Deterministinen testi:** [`tests/c-083-portable-file-hygiene-and-cleanup.sh`](../../tests/c-083-portable-file-hygiene-and-cleanup.sh)
**Tekijä:** Manus AI

## Mitä opit

Oppitunnin lopussa osaat luoda ja sulkea pienen väliaikaisen binääritiedoston ja tarkistaa jokaisen siirrettävän C-tiedosto-operaation.

## Muistiharjoitus

1. Miksi tavupuskurin pitää kantaa eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto osoittaa, että indeksi on `count`-alkioisen puskurin sisällä?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edellisestä venäläisestä oppitunnista.

## Keskeinen käsite

Sulje jokainen onnistuneesti avattu tiedostovirta ja ilmoita virheistä ennen kuin luotat dataan.

Mukana oleva ohjelma käsittelee raakabyttejä laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja palautusarvot ennen tilan muuttamista. Se on siirrettävä ISO C17 -ohjelma, ja siihen on tarkoituksellisesti jätetty pois shellcode, natiivikoodin lataajat, mielivaltainen Assembly-koodin suoritus, prosessiin kohdistuva injektio, itseään muokkaava koodi ja varsinainen kryptografinen toteutus.

## Käytännön esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_and_read_demo(const char *path, uint8_t *out)
{
    const uint8_t value = UINT8_C(84);
    FILE *stream = fopen(path, "wb");
    if (stream == NULL) return 0;
    if (fwrite(&value, sizeof value, 1, stream) != 1 || fclose(stream) != 0) return 0;
    stream = fopen(path, "rb");
    if (stream == NULL) return 0;
    if (fread(out, sizeof *out, 1, stream) != 1 || fclose(stream) != 0) return 0;
    return 1;
}

int main(void)
{
    uint8_t value = 0;
    if (!write_and_read_demo("lesson.bin", &value)) return EXIT_FAILURE;
    if (printf("C 083 verified file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-083-portable-file-hygiene-and-cleanup \
  examples/c/c-083-portable-file-hygiene-and-cleanup.c
./c-083-portable-file-hygiene-and-cleanup
```

Odotettu tuloste:

```text
C 083 verified file byte: 84
```

## Ohjattu harjoitus

1. Tunnista ohjelmassa kaikki osoitin+pituus- tai puskuri+kapasiteetti -sopimukset.
2. Etsi ohjelmasta varhaisin hylätty virheellisen syötteen tarkistus.
3. Selitä, miksi ohjelma ei pidä `0x00` raakabufferin (tavupuskurin) lopun merkkinä.
4. Kerro, mikä tuloste kirjoitetaan vain sen jälkeen, kun validointi on onnistunut.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantokelpoista kryptografiaa, vaikka aihe käsittelee hajautusta tai eheyden tarkistusta — erottele opetusesimerkit ja leikkimieliset hajautukset todellisesta tuotantokäyttöön sopivasta kryptografiasta.

## Itsenäinen harjoitus

Lisää yksi deterministinen kelvollinen syöte ja yksi hylätty rajatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulosteiden palautusarvojen tarkistukset. Jos muutat muistiallokointiin liittyvää laskentaa, lisää tarkistettu ylivuodotapaus ennen laskutoimitusta.

## Varmistus

Suorita deterministinen testi:

```sh
./tests/c-083-portable-file-hygiene-and-cleanup.sh
```

Käännä ja suorita sanitisaattoreiden kanssa, jos tuki on saatavilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-083-sanitized \
  examples/c/c-083-portable-file-hygiene-and-cleanup.c
./c-083-sanitized
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat kertoa tavupuskurin sopimuksen, tunnistaa hylätyn syötteen, selittää tulosteen sekä erottaa opetus- ja leikkiesimerkkien eheyden/hajautuksen oikeasta tuotantokelpoisesta kryptografisesta suojauksesta.

## Paljastus: vastaukset ja mallivastaus

1. Raakabytet voivat sisältää `0x00`, joten päätteen etsintään perustuva pituuden laskenta ei ole luotettava.
2. Eteenpäin läpikäynnin ehto on `index < count`.
3. I/O-, allokointi-, jäsentämis- ja muotoilutulokset on tarkistettava ennen kuin niiden tuottamia arvoja käytetään.
4. Validioitu ohjelma muuttaa tulostetta tai tilaa vasta kaikkien edellytysten onnistuttua.

## Seuraava oppitunti

Seuraava oppitunti jatkuu venäjän kielellä.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C-tiedostojen syöte ja tulostus — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-pohjaisen työkaluketjun koventamisen muistilista — OWASP"
