# Line formation v1.1:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes) -> leader
* placement de followers un par un aléatoirement qui font un wander par tick sur de toutes petites distances. Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader.

## Problèmes
toujours le même problème de déplacement, pas réglé même avec des mouvements par impulsions

## Idée
garder les impulsions mais utiliser kalman ---> v1.2