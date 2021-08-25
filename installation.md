### Usage Instructions

#### Arch Linux
Just running the script build.sh from the project root directory should build and run the program.
The script will install all the dependencies, build the project and runs it.
```
cd Chess
./build.sh
```

#### Other Linux distros
Install the above mentioned dependencies and cmake.
``` 
cmake .
make
./Chess
```

#### Windows and VS Code
1. Download this project as zip then extract it
2. Install `CMake` and `CMake Tools` extentions in VS Code.
3. Set MSVC as your compiler in VS code.
4. Configure the project using CMake.
5. Build the project after configuring the project.
6. Make sure all the assets are in "../build/debug/assets"
7. A chess.exe file should be present on debug folder if the project was build succesfully.
8. Run chess.exe