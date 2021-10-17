# NetPong Project

## Mise en place de l'environnement et des d�pandances

La premi�re �tape est d'installer le sous module vcpkg

```bash
> git submodule update --init --recursive
```

Cmake est requis pour construire le projet il est t�l�chargable [ici](https://cmake.org/download/)

il est aussi n�c�ssaire d'install� un compilateur et plusieurs autres outils

*Windows*
Sous windows le plus simple reste d'install� [Visual studio](https://visualstudio.microsoft.com/fr/downloads/) avec les addons d�veloppement multiplateforme en C++

*Linux*
```bash
> sudo apt update
> sudo apt install g++ gdb make ninja-build rsync zip
```

### Lancer bootstrap-vcpkg pour initialiser vcpkg

*Windows*
```cmd
> ./vcpkg/bootstrap-vcpkg.bat -disableMetrics
```

*Linux*
```bash
> ./vcpkg/bootstrap-vcpkg.sh -disableMetrics
```

### Installer les d�pandances

```cmd
./vcpkg/vcpkg.exe --feature-flags=versions install
./vcpkg/vcpkg.exe install
```

### Build et debug

*Sous Visual Studio*



```bash
> cmake -B out -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
```