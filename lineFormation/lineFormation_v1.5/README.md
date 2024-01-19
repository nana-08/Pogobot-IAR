# Line formation v1.5:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes) -> leader
* placement de followers un par un aléatoirement qui font un wander en continu (pas de tick). Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader, à l'exception que cette fois ils envoient les messages dans la même direction qu'ils les ont reçus. + modification du jump set pour qu'il démarre toujours à full puis descende vers 3/4.
