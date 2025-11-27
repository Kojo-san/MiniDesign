# MiniDesign - TP4 LOG2400  

## Compilation
   ```bash
   g++ -std=c++17 *.cpp -o MiniDesign_new.exe
  ```

## Exécution
   ```bash
 ./MiniDesign_new.exe "(5,0) (14,16) (23,0) (0,8) (0,0) (28,8)"
  ```
Le programme accepte une liste de points (x,y) séparés par des espaces.
Ensuite, utilisez les commandes interactives :
* a : afficher la liste des points et nuages
* o1 / o2 : afficher l’orthèse ASCII
* f : fusionner des points en un nuage (texture appliquée automatiquement : o puis #)
* d : déplacer un point
* s : supprimer un point
* c1 / c2 : création de surfaces selon Strategy
* u / r : undo / redo
* q : quitter

## Auteurs
* Gamaliel Kalefe
* Sara Dakir

