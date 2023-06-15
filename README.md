# MPC-MPG
* Moderní počítačová grafika
* Projekt: Fraktálový svět

## Finální projekt (body 24/24):
* [Hlavní skript](xpastu02.cpp)
* [Vlastní knihovna](imageLoad.h)
* [Zadání projektu](SP2023.pdf)

## Úvod
Tento projekt modeluje 3D Fraktálový svět, který splňuje většinu požadovaných kategorií.

## Seznam vypracovaných úkolů:
1) Modelování objektů (3 b) - splněno
- 30 dračích křivek v různých iteracích => jiné útvary (je zde využito rekurzivní volání funkce)
- 5 krychlí
2) Animace (1 b) - splněno
- dračí křivky se točí i hýbají, to stejné krychle na středu plátna, bezierovy pláty, atd.
- pro zapnuti/vypnuti buď v menu nebo 't'
- pro zrychlení lze držet 't'
3) Osvětlení (1 b) – splněno
- barevné osvětlení se neustále mění a zapne/vypne se klávesou 'o' nebo z menu
- změna osvětlení jde zastavit vypnutím času 't' nebo v menu a opačně
4) Volný pohyb v horizontální rovině (1 b) - splněno
- šipky pohyb z pohledu kamery jako v Minecraftu
- WASD také pohyb, ale jen v ose x nebo z, nezávisí na pohledu kamery
5) Menu (2 b) - splěno
- Reset pohybu a pozice nebo 'r'
- Timer nebo 't'
- Letani (nahoru/dolu) nebo 'Ctrl'
- Osvetleni nebo 'o'
- Textura (generovaná) nebo 'g'
- Ukončení programu nebo 'x'
presum = 8b
------------------------------------------------------
6) Výpis textu (2 b) - splněno
- výpis je vlevém dolním rohu
- vypisuje se zde pohyb, pauza, reset, osvětlení, creative/survival, naražení do krychle, atd.
- výpis v levém horním rohu, reaguje na změnu okna
- vypisuje se zde pozice vyhozeného míče
7) Ruční svítilna (2 b) – X
8) Blender model (2 b) - X
9) Létání (2 b) – X
10) Stoupání, klesání (1 b) - splněno
- 'Page up/down' funguje vždy
- nebo je potřeba zapnout creativ mode přes Ctrl nebo Menu a
nahoru se lítá přes mezerník a dolů přes shift+mezerník
presum = 8 + 3 = 11b
------------------------------------------------------
11) Hod předmětu (2 b) - splněno
- přes klávesu 'f' se vyhodí míček z pohledu kamery, který skáče
- míček se kutálí/skáče jen nějakou dobu nebo jde vypnout stejnou klávesou 'f'
- míček lze vyhodnit odkudkoliv i ze vzduchu
12) Simulace kroků (2 b) - splněno
- při pohybu přes šipky i WASD je nadskakování, tedy kroky
- (vylepšení, double jump, pokud dřív kamera nespadne)
- pokud neni zastaven čas, tak kamera po chvili klesne na puvodni uroven
13) Tlačítka (2 b) – splněno
- obrazovka je rozdělena na 4 části a vypisuje do které části bylo kliknuto,
výpis je v dolním levém rohu
14) Průhlednost (1 b) – splněno
- poloprůhledná různobarevná krychle na středu a bezierovy pláty
15) Projekční paprsek (1 b) – X
presum = 11 + 7 = 18 b
------------------------------------------------------
16) Neprůchozí objekt (2 b) – splněno
- u statických krychlí nejde jít dovnitř a vrací to kameru na předchozí pozici
- při viletění na krychlí a přepnutí na survival kameraman začne skákat
17) Texturování (2 b) – jen generován kódem -> 1b
- u Beziérových plátů, dá se zapnout/vypnout přes menu nebo 'g'
18) Bézierovy pláty (2 b) – splněno
- několik plátů jako kopečky, různobarevné, poloprůhlené
- utváří jakoby pohybující se týpí u ohniště, které reprezentuje pohybující se kostky
19) Estetika scény - 0b
20) Vlastní rozšíření – fraktály, double jump, skákací koule se třením 1b
Suma celkem = 18+6 = 24 b

## Ukázky:



