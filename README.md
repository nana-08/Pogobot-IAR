# IAR 2023: Projet Pogobots - Dispersion sur une grille

Ce dépôt est consacré au projet de robotique en essaim Dispersion sur une grille dans le cadre de l'UE IAR. Comme le sujet l'indique, l'objectif principal est de disperser les robots sur une grille de manière distribuée. Avant de s'atteler à ce problème, nous l'avons réduit à un problème plus simple à implémenter: la dispersion en ligne.

## Plan d'objectifs
### 1ère étape: Etude des capacités des Pogobots 🤖
* Etude des déplacements des robots, notamment en ligne droite
* Etude de la communication entre les robots

### 2ème étape: Dispersion en ligne 📏
* Simulation sur Unity/Python
* Implémentation d'un algorithme de dispersion en ligne:
    - v1: avec un leader placé au préalable => lineFormation_v1
    - v2: pas de leader préalablement placé
    - v3: déplacement en gardant la forme de ligne

### 3ème étape: Implémentation d'un algorithme de dispersion en grille 📐
- Découler du comportement des lignes
- anchor_message fonctionnera ? (=> non, les robots sont trop bruités pour que ça fonctionne)
