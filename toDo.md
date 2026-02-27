# Todo list

<!-- input handling hybride:
Deels in de game en deels in de wereld
In de input handler twee verschillende states:
Een die checkt tegen opzichte van de vorige loop
en een die wacht tot een press of release geconsumeerd is. -->


<!-- input handling aanpassen: De input state wordt doorgegeven aan de world en die
consumed events zoals release en pressed, maar laat de down staan. -->

<!-- Game ticks van 1/20 per seconde met accumulators -->

posities interpoleren voor smooth graphics

Muis ingedrukt kunnen houden voor deleten en plaatsen
Niet kunnen plaatsen als het blokje te dichtbij is
verschillende types blokjes kunnen plaatsen
Plusje voor richten
9 vierkantjes onderin



Refactoren voor playerClass in plaats van Camera. (Gefixed!!!)


## Blokken deleten en toevoegen.

- world class kijkt naar waar de speler naar kijkt.
- te deleten blok,
- Waar we het blok toevoegen ( eerst printen?)
- Elke world update opnieuw checken?


Ray tracen:
pos en forward.
Voor elke dimensie kijken waar die snijdt?
lengte is (x-pos.x)/forward.x.

Idee:
Bekijk eerst x:
x positief.
ceiling(pos.x). s = 
Ok nieuw idee we kijken naar de afstand naar het midden van het blokje?




## Rest van de ideeen

Rode lijnen rond geselecteerd blokje? JA nu aan werken
Chunks spawnen rond de speler?
Camera mode en speler mode? Dus de handling bepaalt de player locatie en waar die naar kijkt en dan wordt de camera geupdate?





User interface fixen. (IMGui?)
Blokjes toevoegen en deleten.

biome

struct  van verschillende perlin noises?

Uitzoeken wie verantwoordelijk is voor het bijhouden van welk chunks we wel en niet renderen.(world of chunkManager?)
De world checkt welke chunks in range van de player zijn.


Frame rate vast zetten of de game ticks alleen in vaste intervallen.

See through shizzle

