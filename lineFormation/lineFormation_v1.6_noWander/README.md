# Line formation v1.6:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes). Lorsqu'il reçoit une réponse d'un robot placé près de lui, il n'émet plus que dans l'axe de la ligne ainsi formée.
* placement de followers un par un aléatoirement qui font un wander en continu (pas de tick). Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader. + modification du jump set pour qu'il démarre toujours à full puis descende vers 3/4.
* seuls les robots à l'extrémité de la ligne émettent un message, dans l'axe de la ligne
