# quad-tree-visualisation
Small C project visualising quad trees using Raylib.

## Prerequisites
Raylib installation is needed. Below are examples for Ubuntu Linux with `apt` and MacOS with `brew` (those two are also commented out in build scripts, so you can just uncomment them before first build script run).

For more info on Raylib installation visit [Raylib Wiki](https://github.com/raysan5/raylib/wiki/)

### Ubuntu Linux
```sh
sudo add-apt-repository ppa:texus/raylib
sudo apt install libraylib5-dev -y
```

### MacOS

Install Raylib on MacOS with brew
```sh
brew install raylib
```

## Build

Run build script `build.sh` (Linux) or `build.osx.sh` (MacOS).

## Run

Build scripts put executables into `build` directory. To run from project root:
```sh
build/quadtree
```

