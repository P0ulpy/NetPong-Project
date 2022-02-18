# Neon PongBall Stadium 90°

  Neon PongBall Stadium 90° est un projet de création de jeu en réseau, réalisé dans le cadre du cours de Programmation Moteur à Ynov Campus. Nous avons été dirigé par Fabrice Granger, à ce jour directeur technique à Big Bad Wolf Studio et professeur dans notre école. Ce jeu a été développé dans le langage C++ avec la librairie SFML, le but premier étant de produire un code structuré et optimisé. À l'origine, nous devions reproduire un jeu de type "Pong", en réseau; au vu du rythme d'avancement de notre projet, nous avons décidé de rajouter quelques règles...

  L'équipe !
  - Florian Aurousseau, 11ème Dan en réseau et en crunch C++ #JeRendraiFierLeMaitre
  - Valentin Barrère, Ingénieur-Souverain en vaisseaux spatiaux #JackyCamboui
  - Julen Leremboure, Invocateur des Saintes PongBalls et du diabolique PolygonTerrain #CollisionPlonge
  
  Une page itch.io sera disponible très prochainement !

  Le code source est disponible ! Les instructions d'installation sont en-dessous.

## Les règles

  Neon PongBall Stadium 90° est un jeu en un contre un, où les joueurs sont enfermés dans une arène à bord de leur vaisseau. Leur but : éliminer l'adversaire avec leur canon à Pong-Ball. Un seul coup bien placé suffit pour détruire le vaisseau adverse. Cependant, les tirs directs sont inutiles, les vaisseaux n'étant pas conçus pour se détruire en eux. Pour espérer atteindre l'adversaire, les joueurs devront surcharger leurs projectiles d'électricité en les envoyant ricocher sur les murs de l'arène. Avec un bon angle, vous aurez peut-être une chance de le toucher...
 
## Structure et système du projet

Une partie décrivant briévement le système verra le jour très prochainement !

## Instructions d'installation

### Préparation du projet

La première étape est d'installer le sous-module vcpkg

```bash
git submodule update --init --recursive
```

Il est aussi nécéssaire d'installer un compilateur et plusieurs autres outils...

*Windows*

Sous windows, le plus simple reste d'installer [Visual studio](https://visualstudio.microsoft.com/fr/downloads/) avec les addons développement multiplateforme en C++

*Linux*

Cmake est requis pour construire le projet. Il est téléchargable [ici](https://cmake.org/download/)

```bash
sudo apt update
sudo apt install g++ gdb make ninja-build rsync zip libx11-dev libxrandr-dev libxi-dev libudev-dev libgl1-mesa-dev
```

### Lancer bootstrap-vcpkg pour initialiser vcpkg

*Windows*
```cmd
./vcpkg/bootstrap-vcpkg.bat -disableMetrics
```

*Linux*
```bash
./vcpkg/bootstrap-vcpkg.sh -disableMetrics
```

### Installer les dépandences

```cmd
./vcpkg/vcpkg.exe --feature-flags=versions install

# si une réinstallation est nécessaire 
./vcpkg/vcpkg.exe install
```

### Build et debug

*Sous Visual Studio*

/////////////////

*avec cmake directement*

```bash
cmake --build out/debug --target NetPong-Project
```

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - NMake Makefiles" ./NetPong-Project
```

```powershell
(dir -include *.cpp -recurse | select-string "^(\s*)//" -notMatch | select-string "^(\s*)$" -notMatch).Count
(dir -include *.hpp -recurse | select-string "^(\s*)//" -notMatch | select-string "^(\s*)$" -notMatch).Count
```
