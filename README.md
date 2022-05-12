# pTK
C++ library for building user interfaces.

**Very early work in progress project.**

## Build
Start by cloning the repository:

```shell
$ git clone --recursive https://github.com/knobin/ptk.git
```

Next follow the instructions to build Skia in the **[Building Skia](#building-skia)** section.

After building skia go to the main ptk directory and create a build directory.

```shell
$ mkdir build
```

Then use cmake to build the project.

Example below is a Release build with unix makefiles.

```shell
$ cd build
$ cmake -G "Unix Makefiles" ..
$ make
```

### Building Skia

First go to the third party directory in ptk.

```shell
$ cd third_party
```

Be sure to have **Clang** as the default compiler and then follow the instructions depending on your platform: [Linux](#skia-on-linux), [macOS](#skia-on-macos) and [Windows](#skia-on-windows).

#### Skia on Linux

```shell
$ git clone 'https://chromium.googlesource.com/chromium/tools/depot_tools.git'
$ export PATH="${PWD}/depot_tools:${PATH}"
$ cd skia
$ python2 tools/git-sync-deps
$ gn gen out/Release --args="is_debug=false is_official_build=true skia_use_system_expat=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false"
$ ninja -C out/Release skia
```

#### Skia on macOS

```shell
$ git clone 'https://chromium.googlesource.com/chromium/tools/depot_tools.git'
$ export PATH="${PWD}/depot_tools:${PATH}"
$ cd skia
$ python2 tools/git-sync-deps
$ gn gen out/Release --args="is_debug=false is_official_build=true skia_use_system_expat=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_sfntly=false skia_use_freetype=true skia_use_harfbuzz=true skia_pdf_subset_harfbuzz=true skia_use_system_freetype2=false skia_use_system_harfbuzz=false skia_use_metal=true target_cpu=\"x64\" extra_cflags=[\"-stdlib=libc++\", \"-mmacosx-version-min=10.13\"] extra_cflags_cc=[\"-frtti\"]"
$ ninja -C out/Release skia
```

#### Skia on Windows
It is recommended to use Clang to build skia due to it being optimized for it and compiling it with another compiler will result in a performance penalty.

However, it is more complicated to do and this project is not really need the added performance for now.

Below are instructions on compiling Skia with **MSVC** using the **Command Prompt**.
Be sure to install **Visual Studio 2022 Community Edition** as it is needed. If it is installed with a custom path, be sure to update **win_vc** with the custom path.

```shell
$ git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
$ cd skia
$ python3 tools/git-sync-deps
$ ..\depot_tools\gn gen out/Release --args="is_debug=false is_official_build=true skia_use_system_expat=false skia_use_system_icu=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_sfntly=false skia_use_freetype=true skia_use_harfbuzz=true skia_pdf_subset_harfbuzz=true skia_use_system_freetype2=false skia_use_system_harfbuzz=false target_cpu=""x64"" win_vc=""C:\Program Files\Microsoft Visual Studio\2022\Community\VC"" extra_cflags=[""-MT""]"
$ ..\depot_tools\ninja -C out/Release skia
```

## Usage
Small sample application:

```cpp
#include "ptk/ptk.hpp"

int main(int argc, char *argv[]) {
    pTK::Application app{"Sample Application", argc, argv};
    pTK::Window window{"Sample Window", {960, 540}};

    pTK::Ref<pTK::Label> label = pTK::Create<pTK::Label>();
    label->setText("Sample Label");
    window.add(label);
    
    return app.exec(&window);
}
```

For more sample applications see:
* [Examples](example)
* [SandboxApp](sandbox)

## License
See [MIT License](LICENSE).

This project uses third-party dependencies:
* [Catch2](https://github.com/catchorg/Catch2): A unit testing framework for C++ ([licence](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt))
* [Skia](https://github.com/google/skia): 2D graphic library for drawing Text, Geometries, and Images ([licence](https://github.com/google/skia/blob/master/LICENSE))
* [spdlog](https://github.com/gabime/spdlog): Fast C++ logging library ([licence](https://github.com/gabime/spdlog/blob/master/LICENSE))
