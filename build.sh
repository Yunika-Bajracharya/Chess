#!/bin/bash
OS=$(awk -F'=' '/ID_LIKE/{print $2}' /etc/os-release)

RUN="TRUE"
CHECK_DEPENDENCIES="TRUE"

# Parses the arguments
while test $# -gt 0; do
  case "$1" in
    -h|--help)
      echo "A script for setting up and building the project"
      echo "By default it builds and runs the project"
      echo " "
      echo "Usage: ./build.sh [options]"
      echo " "
      echo "options:"
      echo "-h, --help                show brief help"
      echo "-n --no-check             tries to build the project without checking dependencies"
      echo "-b, --build               Build the project without running it"
      exit 0
      ;;
    -n|--no-check)
      CHECK_DEPENDENCIES="FALSE"
      shift
      ;;
    -b|--build)
      RUN="FALSE"
      shift
      ;;
    *)
      break
      ;;
  esac
done

# Downloading Dependencies
if [[ "$CHECK_DEPENDENCIES" == "TRUE" ]]; then
  if [ "$OS" = "arch" ]; then
   dependencies=("sdl2" "sdl2_image" "sdl2_ttf" "sdl2_mixer" "cmake")
   for i in "${dependencies[@]}"; do
     if pacman -Qi $i > /dev/null; then
       echo "The package $i is installed"
     else 
       echo "The package $i is not installed"
       echo "Installing the package $i"
       sudo pacman -S $i
     fi
   done

  elif [ "$OS" = '"ubuntu debian"' ] || [ "$OS" = "debian" ]; then 
   echo "debian"
   dependencies=("g++" "libsdl2-dev" "libsdl2-image-dev" "libsdl2-ttf-dev" "libsdl2-mixer-dev")
    for i in "${dependencies[@]}"; do
     if dpkg -l $i > /dev/null; then
       echo "The package $i is installed"
     else 
       echo "The package $i is not installed"
       echo "Installing the package $i"
       sudo apt-get install $i
     fi
    done

  else 
   echo "Unrecognized distro"
   echo "Install the following dependencies"
   echo "dependencies: sdl2, sdl2_image, sdl2_ttf, sdl2_mixer, cmake"
   echo "and run:"
   echo "$ ./build.sh -n"
   echo "Enter: $ ./build.sh --help for more info"
  fi
fi

# Running for ubuntu

if [ "$OS" = '"ubuntu debian"' ] || [ "$OS" = "debian" ]; then 
  g++ src/* -lSDL2 -lGL -lSDL2_image -lSDL2_mixer -lSDL2_ttf -o Chess
  # running is the variable is not false
  if [[ "$RUN" == "FALSE" ]]; then
    echo "Build complete. Executable at $(pwd)"
  elif [ -f Chess ]; then
    echo "Running program"
    ./Chess;
  else
    echo "Something seems to be wrong"
  fi
  exit 0
fi

# Running cmake
cmake -S . -B build

# if we failed to create build directory
if [ ! -d "build" ]; then
  exit 1
fi

# Copying all the assets
if [ ! -d "build/assets" ]; then
  cp -r assets build/assets
  echo "Copying assets folder" 
fi

## Changing to the directory
cd build
echo "Changed directory to: $(pwd)" 

# Building
make 

# running is the variable is not false
if [[ "$RUN" == "FALSE" ]]; then 
  echo "Build complete. Executable at $(pwd)"
elif [ -f Chess ]; then 
  echo "Running program"
  ./Chess;
else 
  echo "Something seems to be wrong"
fi

cd ..
