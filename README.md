# Image Viewer

## Git submodules

To fetch and update all submodules, you can run:

```console
$ git submodule sync --recursive && git submodule update --init --recursive --checkout --force
Submodule 'external/glfw' (https://github.com/glfw/glfw.git) registered for path 'external/glfw'
Cloning into '/data/hampus/src/github/imageviewer/external/glfw'...
Submodule path 'external/glfw': checked out '2fbb560eb72fea695687d33066c91a329d6d0465'
```

Note that this overwrites local changes in any of the submodule directories.

## Building

Build using CMake, by running for example:

```console
$ mkdir build
$ cd build
$ CC=clang CXX=clang++ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
-- The C compiler identification is Clang 7.0.0
-- The CXX compiler identification is Clang 7.0.0
...
$ ninja
[21/21] Linking CXX executable src/imageviewer
```

The built executable can then be started like this:

```console
$ ./src/imageviewer
Starting image viewer...
```

## Extension loader: glad

Re-generate the [glad](https://github.com/Dav1dde/glad) bindings using:

```console
$glad --generator c --spec gl --profile core --api "gles2=3.2" --no-loader --out-path external/glad/
[03/03/2019 12:28:09][INFO][glad       ]: downloading latest 'gl' specification
[03/03/2019 12:28:09][INFO][glad.opener]: opening: 'https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/master/xml/gl.xml'
[03/03/2019 12:28:09][INFO][glad       ]: generating 'gl' bindings
[03/03/2019 12:28:09][INFO][glad       ]: generating 'gl' bindings - done
```

## License

See [LICENSE](LICENSE) and [NOTICE](NOTICE) for licensing information.
