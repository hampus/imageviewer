# Image Viewer

Experimental image viewer. Work in progress.

## Dependencies

Included in the source code:

* [GLFW](https://www.glfw.org/)
* [GLM](https://glm.g-truc.net/)
* [stb_image](https://github.com/nothings/stb)
* [glad](https://github.com/Dav1dde/glad)

Other dependencies:

* [CMake](https://cmake.org/)
* C++ compiler (e.g. [Clang](https://clang.llvm.org/))
* OpenGL ES 3.2

Install a suitable compiler (e.g. clang), CMake, and the dependencies needed by
GLFW.

On Ubuntu, the dependencies can be installed by running:

```console
$ sudo apt install clang cmake libglfw3-dev
```

Note that GLFW itself is built from source, but its dependencies are needed.

## Git submodules

To fetch and update all submodules, you can run:

```console
$ git submodule sync --recursive && git submodule update --init --recursive \
    --checkout --force
```

Note that this overwrites local changes in any of the submodule directories.

## Building

Build using CMake, by running for example:

```console
$ mkdir build
$ cd build
$ CC=clang CXX=clang++ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=`pwd`/install ..
$ ninja install
```

The built executable can then be started like this:

```console
$ ./install/bin/imageviewer ../checkerboard.png
Starting image viewer...
```

checkerboard.png is an example image for testing.

## Extension loader: glad

Re-generate the [glad](https://github.com/Dav1dde/glad) bindings using:

```console
$ glad --generator c --spec gl --profile core --api "gles2=3.2" --no-loader \
    --out-path external/glad/
```

## License

See [LICENSE](LICENSE) and [NOTICE](NOTICE) for licensing information.
