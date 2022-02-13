# NetPong Project

## Mise en place de l'environnement et des dépandances

La première étape est d'installer le sous module vcpkg

```bash
git submodule update --init --recursive
```

il est aussi nécéssaire d'installé un compilateur et plusieurs autres outils

*Windows*

Sous windows le plus simple reste d'installé [Visual studio](https://visualstudio.microsoft.com/fr/downloads/) avec les addons développement multiplateforme en C++

*Linux*

Cmake est requis pour construire le projet il est téléchargable [ici](https://cmake.org/download/)

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

### Installer les dépandances

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

```powershell
(dir -include *.cpp -recurse | select-string "^(\s*)//" -notMatch | select-string "^(\s*)$" -notMatch).Count
(dir -include *.hpp -recurse | select-string "^(\s*)//" -notMatch | select-string "^(\s*)$" -notMatch).Count
```