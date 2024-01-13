# Line formation v1.1:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes) -> leader
* placement de followers un par un aléatoirement qui calibrent leurs moteurs avec Kalman puis font un wander par tick sur de toutes petites distances. Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader.