# C 024 — Rajattu binaarikehyksen dekoodaus

**Arvioitu opiskelu­aika:** 25–35 minuuttia
**Esivaatimukset:** C 001–017 ja välittömästi edeltävät oppitunnit
**Seurantaohjelma:** [`examples/c/c-024-bounded-binary-frame-decoding.c`](../../examples/c/c-024-bounded-binary-frame-decoding.c)
**Deterministinen testi:** [`tests/c-024-bounded-binary-frame-decoding.sh`](../../tests/c-024-bounded-binary-frame-decoding.sh)
**Kirjoittaja:** Manus AI

## Mitä osaat tehdä

Tämän oppitunnin lopussa osaat dekoodata kolmen tavun otsikon ainoastaan, kun syötepuskuri sisältää kutakin kenttää varten tarvittavan määrän tavuja.

## Muistin palautusharjoitus

1. Miksi tavupuskuri ei ole sama asia kuin C-merkkijono?
2. Määritä rajatun silmukan ehto `count` kelvollisille alkioille.
3. Miksi tiedostotoimintojen paluuarvot on tarkistettava?
4. Käännä edellisestä venäjänkielisestä oppitunnista yksi tuttu mallilause.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskennallisena datana. Se käyttää eksplisiittisiä `size_t` pituuksia, tarkistaa jokaisen ulkoisen tuloksen eikä nojaa isäntäjärjestelmän muistiasetteluun tai nollatavuun terminaattorina. Se on kannettava ISO C17 -ohjelma eikä luo suoritettavaa koodia, lataa natiivikoodia, suorita mielivaltaista assembly-koodia, tee prosessin injektiota tai muokkaa itseään suorituksen aikana.

## Työstetty esimerkki

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { uint8_t type; uint16_t length; } Header;
static int decode_header(const uint8_t bytes[], size_t count, Header *out)
{
    if (bytes == NULL || out == NULL || count < 3) return 0;
    out->type = bytes[0]; out->length = (uint16_t)(((uint16_t)bytes[1] << 8) | bytes[2]);
    return 1;
}
int main(void)
{
    const uint8_t frame[] = {UINT8_C(2),UINT8_C(0),UINT8_C(3),UINT8_C(0x41),UINT8_C(0),UINT8_C(0x42)};
    Header header;
    if (!decode_header(frame, sizeof frame, &header) || header.length != sizeof frame - 3) return EXIT_FAILURE;
    if (printf("Type: %" PRIu8 "\nPayload length: %" PRIu16 "\n", header.type, header.length) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita se täsmälleen seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-024-bounded-binary-frame-decoding \
  examples/c/c-024-bounded-binary-frame-decoding.c
./c-024-bounded-binary-frame-decoding
```

Odotettu tulostus:

```text
Type: 2
Payload length: 3
```

## Ohjattu harjoitus

1. Määritä ohjelman syöte- ja tulosterajat.
2. Nimeä yksi laskentoon, kapasiteettiin, alueeseen tai paluuarvoihin liittyvä tarkistus.
3. Selitä, miksi upotetut `0x00` tavut säilyvät kelvollisena datana tässä.
4. Kuvaile yksi virheellinen tai ylisuuri syöte, joka tulisi hylätä.
5. Muista yksi tarkka lause välittömästi edeltäneestä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä lisätarkistetulla arvolla tai testitapauksella. Säilytä eksplisiittiset määrät, vältä tarkistamattomia tyypinmuunnoksia ja varmista jokainen ulkoinen tulos ennen sen käyttöä.

## Validointi

Suorita deterministinen testi:

```sh
./tests/c-024-bounded-binary-frame-decoding.sh
```

Sitten, jos se on tuettu, käännä ja suorita sanitisaattoreiden kanssa:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-024-sanitized \
  examples/c/c-024-bounded-binary-frame-decoding.c
```

## Lopullinen itsearvio

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajan, jokaisen asiaankuuluvan paluuarvotarkistuksen, `size_t` lukumääräsopimuksen ja miksi tämä ohjelma ei ole kryptografinen turvallisuusmekanismi.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskuriilla on eksplisiittinen pituus ja se voi sisältää minkä tahansa tavuarvon, myös `0x00`; C-merkkijono sen sijaan päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen etenemisehto on `index < count`.
3. I/O voi olla osittaista tai epäonnistua, joten sen paluuarvo määrää, suoritettiinko pyydetty operaatio loppuun.
4. Kelvollinen vastaus mainitsee NULL-osoittimen, kapasiteetin, aritmeettisen ylivuodon, osittaisen luku-/kirjoitusoperaation tai virheellisen syötteen tarkistuksen.
5. Esimerkki on datankäsittelyharjoitus, ei salausta, todennusta, turvallista hajautusta tai turvallisuusmekanismia.

## Seuraava oppitunti

Seuraava aikataulutettu oppitunti palaa venäjänkieliselle linjalle.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
