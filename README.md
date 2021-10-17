# NetPong Project

## Mise en place de l'environnement et des d�pandances

Cmake est requis pour construire le projet il est t�l�chargable [ici](https://cmake.org/download/)

il est aussi n�c�ssaire d'install� un compilateur et plusieurs autres outils

*Windows*
Sous windows le plus simple reste d'install� [Visual studio](https://visualstudio.microsoft.com/fr/downloads/) avec les addons d�veloppement multiplateforme en C++

*Linux*
```bash
sudo apt update
sudo apt install g++ gdb make ninja-build rsync zip
```

### Lancer bootstrap-vcpkg pour initialiser vcpkg

*Windows*
```bash
./vcpkg/bootstrap-vcpkg.bat
```

*Linux*
```bash
./vcpkg/bootstrap-vcpkg.sh
```

### Rajout� le toolchain file de vcpkg

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake
```