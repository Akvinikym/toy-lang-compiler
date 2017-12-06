# iLang Compiler Project

## Overview

iLang is a simple imperative language for which this compiler is created.

## Requirements

1. LLVM 4.0.1 - install from source is preferable, check [llvm installation guide](./LLVM_INSTALL.md) for instructions
2. C++14 compatible compiler (gcc or clang)
3. CMake 3.6+

## Installation

CMake is used as a build system. Here is the installation process:

### Linux/OS X

1. Create directory where all build files will be placed.

   ```mkdir build && cd build```

2. Generate build files, if you are missing some dependencies, cmake will fail.

   ```cmake ../src```

3. Build executable

   ```make```

4. Check that everything works by running

   ```./voc```

## Running

Right now compiler generates object code file for given input. You can provide it a source file
```./voc -o dest source```

Result will be object file, which can be linked with C/C++ program to obtain executable.
In the future, we will provide working driver to generate executable, but at the moment you have to do following steps:

1. Compile voc source file
2. Create C file, which declares and calls necessary function
3. Build it using C compiler, and provide it generated object
4. You now have working executable.

Folder test contains examples of programs, including some simple algorithms such as greatest common divisor and sort.
