## Idées en vrac
* rassemblement des robots -> hypothèse qu'ils peuvent tous communiquer
=> incertitudes liées à la communication => revoir le travail fait sur la détection de bordures qui marchait assez bien

* communication pour compter le nombre de robots -> tous les robots prennent connaissance de leurs voisins (id)

* communication et consensus => attribution des coordonnées
	-> premier arrivé premier servi ? dur niveau communication, incertitudes sur le bruit/réception des messages par les robots voisins
		-> jouer sur la fréquence d'émission de msg? => aléatoire pour éviter une surcharge de msg, dans ce cas ça pourrait marcher
	-> choix sur l'id du robot ? à ce moment-là autant attribuer les rôles aux robots à la main puisque le résultat sera toujours le même => pas super flexible sur le nombre de robots

* dispersion en grille
commencer par un cluster puis robots s'éloignent peu à peu ?

## Algorithme (1) dispersion en ligne par tâtonnement
- chaque robot effectue un tirage aléatoire pour s'arrêter
- SI robot à l'arrêt:
	- SI perçoit un voisin: émet un signal "Je suis immobile" dans les directions parallèles à la position de ce voisin
	- SI pas de voisin: émet un signal "Je suis immobile" dans toutes les directions
	- SI pas de voisin pendant x temps: recommence à bouger
- SI robot en mouvement:
	- SI reçoit le signal "Je suis immobile": s'arrête à côté du robot à l'arrêt et adopte la même orientation que lui (possible si le robot sait par quel capteur le message a été émis (doute là-dessus))

-> commencer par un robot initial préalablement placé et qui reste statique


## Algorithme (Beta) Wanderer
- chaque robot effectue un déplacement aléatoire pendant un temps aléatoire pour explorer l'environnement
- SI robot perçoit un voisin, il chercher à s'en éloigner
- SI robot perçoit un obstacle, il cherche à s'en éloigner