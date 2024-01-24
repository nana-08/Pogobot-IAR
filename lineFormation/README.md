## Line formation v1
Cette première (et sans doute dernière) version de line formation permet de, à partir d'un leader préalablement placé, former une ligne de n pogobots qui se placent derrière lui. 
### RESUME DES VERSIONS: chaque version découle de la précédente
- 1.0: déplacement en ligne droite
- 1.1: déplacement en ligne droite par impulsion tous les ticks
- 1.2: déplacement en ligne droite avec calibration par Kalman
- 1.3: déplacement aléatoire des followers "wander"
- 1.4: retrait du tick + contrôle des demi-tours dans le wander
- 1.5: contrôle des messages reçus en "diagonale": on ignore les messages reçus par 2 capteurs
- 1.6: émission de messages seulement par les pogo aux extrémités de la ligne
- 1.6 no wander: 1.6 mais déplacement en ligne droite