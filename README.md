# Setup

In order for this to work, it requires the following dependencies
- GLFW (Window creation)
- GLAD (OpenGL functions)
- GLM  (Math)

## Build

### Building for Windows

```
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Building for Linux/macOS

```
mkdir build && cd build
cmake ..
make
```

