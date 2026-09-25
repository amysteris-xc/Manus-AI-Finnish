# C 018 — Rajoitettu binaaritietueiden kirjoittaminen

**Arvioitu opiskeluaika:** 25–35 minuuttia
**Edellytykset:** C 001–017 ja välittömästi edeltävät oppitunnit
**Esimerkkiohjelma:** [`examples/c/c-018-bounded-binary-record-writing.c`](../../examples/c/c-018-bounded-binary-record-writing.c)
**Deterministinen testi:** [`tests/c-018-bounded-binary-record-writing.sh`](../../tests/c-018-bounded-binary-record-writing.sh)
**Tekijä:** Manus AI

## Mitä osaat tehdä

Oppitunnin lopuksi osaat kirjoittaa kaksi erikseen määriteltyä kahdeksantavuista tietuetta siten, että käytät täydellistä kirjoituskäsittelyä, ulostulon pakotustarkastuksia ja tarkastettuja sulkemistuloksia.

## Kertauskysymykset

1. Miksi tavupuskuri ei ole sama asia kuin C-merkkijono?
2. Määrittele rajoitetun silmukan ehto `count` kelvollisille alkioille.
3. Miksi tiedostotoimintojen paluuarvot on tarkistettava?
4. Käännä yksi tuttu lause edellisestä venäjänkielisestä oppitunnista.

## Keskeinen käsite

Esimerkki käsittelee tavuja laskettuina datoina. Se käyttää eksplisiittisiä `size_t` pituuksia, tarkistaa jokaisen ulkoisen toimintakutsun paluuarvon eikä nojaudu isäntämuistin rakenteeseen tai nollatavuun merkkijonon terminointina. Se on kannettava ISO C17 -koodi eikä luo suoritettavia hyötykuormia, natiivikoodin lataajia, mielivaltaista assemblerin suorittamista, prosessiin injektointia tai itseään muuttavaa koodia.

## Esimerkkiaskel

```c
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_all(FILE *out, const uint8_t *data, size_t count)
{
    size_t done = 0;
    while (done < count) {
        size_t step = fwrite(data + done, 1, count - done, out);
        if (step == 0) return 0;
        done += step;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    const uint8_t records[][8] = {{0,0,0,42,0,5,1,0},{0,0,1,35,0,160,2,1}};
    FILE *out;
    if (argc != 2) return EXIT_FAILURE;
    out = fopen(argv[1], "wb");
    if (out == NULL) return EXIT_FAILURE;
    for (size_t index = 0; index < sizeof records / sizeof records[0]; ++index)
        if (!write_all(out, records[index], sizeof records[index])) { (void)fclose(out); return EXIT_FAILURE; }
    {
        int flush_status = fflush(out);
        int close_status = fclose(out);

        if (flush_status != 0 || close_status != 0) return EXIT_FAILURE;
    }
    if (printf("Records written: %zu\n", sizeof records / sizeof records[0]) < 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
```

Käännä ja suorita täsmälleen näin:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -o c-018-bounded-binary-record-writing \
  examples/c/c-018-bounded-binary-record-writing.c
./c-018-bounded-binary-record-writing records.bin
```

Odotettu tulostus:

```text
Records written: 2
```

## Ohjattu harjoitus

1. Määritä ohjelman syöte- ja tuloserajat.
2. Nimeä yksi lukumäärään, kapasiteettiin, alueeseen tai paluuarvoon liittyvä tarkistus.
3. Selitä, miksi upotetut `0x00` tavut ovat kelvollisia datoja tässä esimerkissä.
4. Kuvaile yksi viallinen tai liian suuri syöte, joka tulisi hylätä.
5. Muista yksi täsmällinen lausuma välittömästi edeltävästä venäjänkielisestä oppitunnista.

## Itsenäinen harjoitus

Laajenna ohjelmaa yhdellä lisätarkastetulla arvolla tai testitapauksella. Säilytä eksplisiittiset lukumäärät, vältä tarkistamattomia tyyppimuunnoksia ja validoi jokainen ulkoinen tulos ennen sen käyttämistä.

## Varmennus

Suorita deterministinen testi:

```sh
./tests/c-018-bounded-binary-record-writing.sh
```

Kun tuki on saatavilla, käännä ja suorita myös sanitisaattoreilla:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g \
  -o c-018-sanitized \
  examples/c/c-018-bounded-binary-record-writing.c
```

## Lopullinen itsearviointi

Olet valmis seuraavaan oppituntiin, jos osaat selittää eksplisiittisen rajan, jokaisen asiaankuuluvan paluuarvon tarkistuksen, `size_t` lukusopimuksen sekä miksi tämä ohjelma ei tarjoa kryptografista suojausmekanismia.

## Spoileri: vastaukset ja mallivastaus

1. Tavupuskurilla on eksplisiittinen pituus ja se voi sisältää minkä tahansa tavuarvon, mukaan lukien `0x00`; C-merkkijono päättyy ensimmäiseen nollatavuunsa.
2. Turvallinen etenemisehto on `index < count`.
3. Syöte- ja tulostotoiminnot voivat olla osittaisia tai epäonnistua, joten niiden paluuarvo kertoo, suoritettiinko pyydetty työ loppuun.
4. Kelvollinen vastaus mainitsee null-osoittimen, kapasiteetin, aritmeettisen ylivuodon, liian lyhyen luku-/kirjoitusoperaation tai viallisten syötteiden tarkistamisen.
5. Esimerkki on datankäsittelyharjoitus, ei salausta, todennusta, turvallista tiivistettä tai turvallisuuskontrollia.

## Seuraava oppitunti

Seuraavaksi aikataulutettu oppitunti jatkaa venäjänkielisellä polulla.

## Viitteet

[1]: https://en.cppreference.com/w/c/io/fread "fread — cppreference.com"
[2]: https://en.cppreference.com/w/c/io/fwrite "fwrite — cppreference.com"
[3]: https://en.cppreference.com/w/c/types/integer "Kiinteän leveyden kokonaislukutyypit — cppreference.com"
