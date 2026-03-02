# Todo list

<!-- input handling hybride:
Deels in de game en deels in de wereld
In de input handler twee verschillende states:
Een die checkt tegen opzichte van de vorige loop
en een die wacht tot een press of release geconsumeerd is. -->


<!-- input handling aanpassen: De input state wordt doorgegeven aan de world en die
consumed events zoals release en pressed, maar laat de down staan. -->

<!-- Game ticks van 1/20 per seconde met accumulators -->

<!-- posities interpoleren voor smooth graphics -->


<!-- Hmm ff nadenken over muis indrukken.
We hebben meer frames per game tick, dus het kan zijn dat we een een key hebben ingedrukt, 
maar volgens mij nu opgelost. -->
<!-- Muis ingedrukt kunnen houden voor deleten en plaatsen -->
<!-- Niet kunnen plaatsen als het blokje te dichtbij is -->

<!-- Range van plaatsen eentje verder dan deleten!
Ray tracing for block targets bepalen?
snel met een block id en een positie en richting de afstand tot het snijpunt bepalen. -->

<!-- Refactor code -->
<!-- Ronddraaien in pauze mode aanpassen -->

<!-- Structs met UIData pointers toevoegen. -->

<!-- Imgui instellen -->


<!-- In ImGui belangrijke stats toevoegen -->
<!-- In Camera de muisdraai snelheid toevoegen.(Nevermind, staat in de player class.) -->
<!-- Bij Pause mode alles pauzeren! -->

Default variabele waardes van de constructor naar de header verplaatsen.
Crashes als ik buiten de chunks ben en een cube in een chunk probeer te verwijderen.

zwarte lijn blijk blokjes weg als ik vanuit een hoek kijk.
UI library maken
Lijnen rond geselecteerd blokje


verschillende types blokjes kunnen plaatsen
Plusje voor richten
9 vierkantjes onderin


Add imgui debug files.(No idea if necessary, but they do exist or something)



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
Raytracing aanpassen voor ofwel klein stapje, of elke keer door de dichtsbijzijnde surface ipv drie verschillende loops.





User interface fixen. (IMGui?)
Blokjes toevoegen en deleten.

biome

struct  van verschillende perlin noises?

Uitzoeken wie verantwoordelijk is voor het bijhouden van welk chunks we wel en niet renderen.(world of chunkManager?)
De world checkt welke chunks in range van de player zijn.


Frame rate vast zetten of de game ticks alleen in vaste intervallen.

See through shizzle

