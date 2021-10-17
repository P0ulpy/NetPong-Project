# NetPong Project

## Mise en place de l'environnement et des dépandances

La première étape est d'installer le sous module vcpkg

```bash
git submodule update --init --recursive
```

Cmake est requis pour construire le projet il est téléchargable [ici](https://cmake.org/download/)

il est aussi nécéssaire d'installé un compilateur et plusieurs autres outils

*Windows*
Sous windows le plus simple reste d'installé [Visual studio](https://visualstudio.microsoft.com/fr/downloads/) avec les addons développement multiplateforme en C++

*Linux*
```bash
sudo apt update
sudo apt install g++ gdb make ninja-build rsync zip
```

### Lancer bootstrap-vcpkg pour initialiser vcpkg

*Windows*
```cmd
./vcpkg/bootstrap-vcpkg.bat -disableMetrics
```

*Linux*
```bash
> ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
```

### Installer les dépandances

```cmd
./vcpkg/vcpkg.exe --feature-flags=versions install
./vcpkg/vcpkg.exe install
```

### Build et debug

*Sous Visual Studio*



```bash
cmake -B out -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
```
