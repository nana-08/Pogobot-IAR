# Line formation v1.5:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes). Lorsqu'il reçoit une réponse d'un robot placé près de lui, il n'émet plus que dans l'axe de la ligne ainsi formée.
* placement de followers un par un aléatoirement qui font un wander en continu (pas de tick). Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader. + modification du jump set pour qu'il démarre toujours à full puis descende vers 3/4.

## Problème
Les robots ont tendance à croiser le signal d'un robot au milieu de la ligne et de s'immobiliser à cet endroit (à cause de la largeur de leur champ IR). Cela forme plus des clusters qu'une véritable ligne (qui n'est déjà pas très droite)

## Idée
Faire en sorte que seuls les robots se trouvant aux extrémités de la ligne émettent, seulement dans la direction de l'axe de la ligne mais dans le sens opposé. De cette façon, il n'y aura plus de signaux IR entre les robots au sein de la ligne