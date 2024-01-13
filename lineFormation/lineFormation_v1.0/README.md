# Line formation v1.0:
## Principe
* placement d'un robot leader immobile qui envoie des signaux dans toutes les directions tous les ticks, (i.e toutes les deux secondes) -> leader
* placement de followers un par un aléatoirement qui font un wander. Ils interrogent leur environnement toutes les secondes pour savoir si il y a un signal. Si ils trouvent le leader/la ligne, ils s'immobilisent et se comportent comme le leader.

## Problèmes
* le déplacement: les robots se déplacent plus ou moins bien, en général moins bien
* les capteurs IR: les signaux sont émis et captés dans un champ et pas de manière très précise

## Idées pour le déplacement
* utiliser Kalman: à défaut de rendre les déplacements parfaits il pourrait vachement les améliorer. Problèmes:
    - il serait mieux de le tester dans l'arène
    - il faudrait d'une manière ou d'une autre récupérer les valeurs de moteurs à mettre après filtre de kalman de manière automatique;
* faire en sorte que les robots se déplacent par impulsion quand ils wander, donc imposer un tick durant lequel ils parcourent une toute petite distance. Ça permettrait de contrôler beaucoup mieux la trajectoire sans trop se prendre la tête, je pense que c'est une bonne solution au moins à court terme ---> **v1.1**