OS=$(awk -F'=' '/ID_LIKE/{print $2}' /etc/os-release)


# Downloading Dependencies

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

elif [ "$OS" = "ubuntu debian" ]; then 
  echo "debian"
  dependencies=("libsdl2-dev" "cmake" "libsdl2-image-dev" "libsdl2-ttf-dev" "libsdl2-mixer-2.0-0")
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


if [ "$1" = "build" ]; then 
  echo "Build complete. Executable at $(pwd)"

elif [ -f Chess ]; then 
  echo "Running program"
  ./Chess;
else 
  echo "Something seems to be wrong"
fi

cd ..
