# Manus-AI — suomenkielinen oppimateriaali

Tämä yksityinen arkisto on suomenkielinen painos venäjän kielen ja C-ohjelmoinnin itseopiskelukurssista. Se sisältää **27 venäjän oppituntia ja 27 C-oppituntia**, joiden opastava teksti, tehtävät ja käännökset ovat suomeksi. Venäjänkieliset esimerkit säilyvät kyrillisillä kirjaimilla.

C-ohjelmien lähdekoodi, käännöskomennot, odotetut tulosteet ja deterministiset testit ovat samat kuin kurssin englanninkielisessä lähdearkistossa. Suomenkielinen painos on tarkistettava yksityinen tilannekuva; englanninkielinen arkisto jatkaa oppituntien säännöllistä kirjoittamista. Uudet suomenkieliset oppitunnit voidaan lisätä myöhemmässä, erikseen tarkistetussa päivityksessä.

## Arkiston rakenne

```text
curriculum_state.md     Kurssin eteneminen, esivaatimukset, kertausmuistiinpanot ja seuraava aihe
LESSON_INDEX.md         Valmiiden ja suunniteltujen oppituntien yksirivinen hakemisto
lessons/russian/        Venäjän oppituntien Markdown-tiedostot
lessons/c/              C-oppituntien Markdown-tiedostot
examples/c/             Ajettavat oheisohjelmat C-oppitunneille
tests/                  Deterministiset testit ja testisyötteet
```

## Oppituntien tasovaatimus

Jokainen oppitunti sisältää tavoitteet, palauttamisharjoituksen, selityksen, läpikäydyt esimerkit, ohjatun harjoittelun, itsenäiset tehtävät, spoiler-otsikon jälkeiset vihjeet tai vastaukset sekä lopullisen itsearvioinnin. Venäjän oppitunnit yhdistävät kyrillisen kirjoituksen ja täsmälliset suomenkieliset käännökset. C-oppitunnit sisältävät tarkan käännöskomennon ja odotetun tulosteen, kun se on sovellettavissa.

C-esimerkit on kirjoitettu kannettavalle ISO C17 -tasolle. Ne käännetään vähintään seuraavasti:

```sh
cc -std=c17 -Wall -Wextra -Wpedantic -o program program.c
```

Kun ympäristö tukee niitä, esimerkit suoritetaan myös AddressSanitizerilla ja UndefinedBehaviorSanitizerilla. Oppimateriaali korostaa tarkistettua syöte-/tulostuskäsittelyä (I/O), rajattuja puskureita, turvallisia kokonlukumuunnoksia ja määrittelemättömän käyttäytymisen välttämistä.

## Turvallisuusraja

Kurssi kehittää ymmärrystä tavupohjaisesta ohjelmoinnista, hajautusalgoritmeista ja kryptografiasta. Se ei kuitenkaan luo shellcodea, natiivikoodin lataajia, mielivaltaista assembler-koodin suorittamista, prosessin injektiota tai itseään muokkaavaa koodia. Mahdolliset opetustarkoitukseen tarkoitetut leikkiesimerkit merkitään selvästi **epäturvallisiksi**, eikä niitä koskaan suositella todellisten tietojen suojaamiseen.

## Kurssin nykyinen tila

Tässä painoksessa C-oppitunti 027 on valmis, ja seuraava suunniteltu aihe on venäjän oppitunti 028: mieltymykset, `нравиться`-rakenteen perusteet ja yksinkertaiset pitämistä koskevat ilmaukset. Katso yksityiskohdat tiedostoista [curriculum_state.md](curriculum_state.md) ja [LESSON_INDEX.md](LESSON_INDEX.md).
