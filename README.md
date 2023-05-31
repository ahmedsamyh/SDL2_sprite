# SDL2_sprite

An implementation for a sprite in SDL2.

It has support for animations, origin, scaling and rotation.

The sprite is made as a stb-style single header-only library [sprite.h](./src/sprite.h)

## Quick Start
```console
> premake5 vs2022
> msbuild -p:configuration=Debug build\SDL2_sprite.sln
> bin\Debug\SDL2_sprite.exe
```
Note: You can also open the VS solution file (.sln) directly and build with Visual Studio<s>(If you want to wait eternally for it to open)</s> and build it.

## Dependencies

- [premake5 (version 5.0.0-beta2 and up)](https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip)
- [Visual Studio 17.4.4 (2022)](https://visualstudio.microsoft.com/vs/community/) with (Desktop development with C++ Workload Installed)


# References:

- SDL2: http://libsdl.org/
- stb-style header-only library: https://github.com/nothings/stb
