# Line formation v1.4:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes) -> leader
* placement de followers un par un aléatoirement qui font un wander en continu (pas de tick). Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader. + modification du jump set pour qu'il démarre toujours à full puis descende vers 3/4.

## Problème
Les robots émettent de tous les côtés, donc il y a peu de chance de former une ligne si ils se contentent de wander n'importe où.

## Idées
N'émettre que de l'émetteur opposé au récepteur.