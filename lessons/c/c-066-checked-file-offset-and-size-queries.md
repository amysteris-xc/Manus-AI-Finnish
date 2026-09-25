# C 066 — Tarkistetut tiedoston siirto- ja koko­kyselyt

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Esitiedot:** C 001–065
**Seurantaohjelma:** [`examples/c/c-066-checked-file-offset-and-size-queries.c`](../../examples/c/c-066-checked-file-offset-and-size-queries.c)
**Deterministinen testi:** [`tests/c-066-checked-file-offset-and-size-queries.sh`](../../tests/c-066-checked-file-offset-and-size-queries.sh)
**Tekijä:** Manus AI

## Mitä osaat tämän jälkeen

Tämän oppitunnin lopuksi sinun pitäisi osata hankkia tiedoston pieni koko käyttämällä tarkistettuja seek- ja tell-operaatioita ilman oletusta niiden onnistumisesta.

## Muistin virkistys

1. Miksi tavupuskuriin on liitettävä eksplisiittinen `size_t` pituus sen sijaan, että luotettaisiin `\0`?
2. Mikä ehto osoittaa, että indeksi kuuluu `count`-alkioiseen puskuriin?
3. Nimeä yksi C-kirjaston palautusarvo, joka on tarkistettava ennen tuloksen käyttämistä.
4. Muista yksi täsmällinen lause edelliseltä venäjänkieliseltä oppitunnilta.

## Keskeinen käsite

Tiedoston sijainti-API:t voivat epäonnistua; jokainen tila ja muunnos on tarkistettava ennen tuloksen käyttöä.

Seurantaohjelma käsittelee raakatavut laskettuna datana. Se tarkistaa argumentit, kapasiteetit ja paluuarvot ennen tilan muuttamista. Ohjelma on kannettava ISO C17 -koodia ja siihen ei tarkoituksellisesti sisällytetä shellcodea, natiivi-suorittimelle ladattavia lataajia, mielivaltaista assembly-koodin suorittamista, prosessin injektiota, itseään muokkaavaa koodia tai todellista kryptografista toteutusta — kyseessä on opettava esimerkki, ei tuotantokäyttöinen salausratkaisu.

## Työstetty esimerkki

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_and_read_demo(const char *path, uint8_t *out)
{
    const uint8_t value = UINT8_C(67);
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
    if (printf("C 066 verified file byte: %u\n", (unsigned)value) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-066-checked-file-offset-and-size-queries \
  examples/c/c-066-checked-file-offset-and-size-queries.c
./c-066-checked-file-offset-and-size-queries
```

Odotettu tuloste:

```text
C 066 verified file byte: 67
```

## Ohjattu harjoittelu

1. Tunnista kaikki ohjelman pointer-plus-length- ja buffer-plus-capacity-sopimukset.
2. Etsi varhaisin hylätty virheellistä syötettä koskeva ehto.
3. Selitä, miksi ohjelma ei käsittele `0x00` raw-tavupuskurin lopetuksena.
4. Ilmoita, mikä tuloste kirjoitetaan vain validoinnin onnistuttua.
5. Kuvaile, miksi tämä oppitunti ei tarjoa tuotantotason kryptografiaa, vaikka aihe käsittelee hajautusta tai eheystarkistuksia.

## Itsenäinen harjoitus

Lisää yksi lisädeterministinen kelvollinen syöte ja yksi hylätty reunatapauksen syöte. Säilytä olemassa olevat argumenttitarkistukset, eksplisiittiset pituudet ja tulostuksen paluuarvojen tarkistukset. Jos muutat mitään allokointilaskentaa, lisää laskutoimituksen eteen tarkistettu ylivuototarkistus.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-066-checked-file-offset-and-size-queries.sh
```

Käännä ja suorita sanitisaattoreiden kanssa siellä, missä niitä tuetaan:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-066-sanitized \
  examples/c/c-066-checked-file-offset-and-size-queries.c
./c-066-sanitized
```

## Lopputarkistus

Olet valmis seuraavaan oppituntiin, jos pystyt kuvaamaan tavupuskurin sopimuksen, tunnistamaan hylätyn syötteen, selittämään tulosteen ja erottamaan opettavaisen eheys- tai leikkikenttähajautuksen oikeasta tuotantokäytön kryptografisesta suojauksesta.

## Ratkaisuosio: vastaukset ja mallivastaus

1. Raakatavut voivat sisältää `0x00`, joten sentinellikatselu ei ole pätevä pituuden laskentatapa.
2. Eteenpäin kulkemisen ehto on `index < count`.
3. I/O-, varaus-, jäsentämis- ja muotoilutulokset on tarkistettava ennen kuin niiden tuottamia arvoja käytetään.
4. Vahvistettu ohjelma muuttaa ulostulotilaa vain sen jälkeen, kun kaikki edellytykset ovat onnistuneet.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Lähteet

[1]: https://en.cppreference.com/w/c/types/integer "Fixed Width Integer Types — cppreference.com"
[2]: https://en.cppreference.com/w/c/io "C File Input/Output — cppreference.com"
[3]: https://cheatsheetseries.owasp.org/cheatsheets/C-Based_Toolchain_Hardening_Cheat_Sheet.html "C-Based Toolchain Hardening Cheat Sheet — OWASP"
