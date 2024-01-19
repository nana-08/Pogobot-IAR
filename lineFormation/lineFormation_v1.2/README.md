# Line formation v1.2:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes) -> leader
* placement de followers un par un aléatoirement qui calibrent leurs moteurs avec Kalman puis font un wander par tick sur de toutes petites distances. Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader.

## Problèmes
toujours le même problème de déplacement, pas réglé même avec des mouvements par impulsions + kalman. Le bruit est trop fort et variant entre les robots.

## Idée
garder les impulsions, virer kalman, prendre le wanderer de Jules (à la place de marcher tout droit) ---> v1.3