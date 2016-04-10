# SPPAR #

A header only Library for Space-Partitioning.

See [TODO]()

## Building ##

Requires CMake and one of the following compilers:

* GCC 4.9 or newer;
* Clang 3.6 or newer (Xcode 5.0 or newer on OS X);
* Visual Studio 2010 or newer;

SPPAR requires the following dependencies:

* [SFML](http://sfml-dev.org) 2.2 or newer (only to use Quadtree.render)

* [googletest](https://github.com/google/googletest) 1.7 or newer (only to build tests)

Use CMake to build the project and install.

```
mkdir build && cd build
cmake -DGMOCK_ROOT=<baseDir> -DGTEST_ROOT=<baseDir> \ 
-DSPPAR_BUILD_TESTS=TRUE -DSPPAR_BUILD_EXAMPLES=TRUE -DSPPAR_BUILD_DOCS=TRUE ..
make -j3
make install

```

## Examples ##

See [Examples]()

## Contributions ##

1. Fork it.
2. change to branch develop(git checkout develop)
3. make new tests if needed
3. commit your changes
4. push to develop
5. open a Pull Request

## Documentation ##

To make the documentation you will need doxygen and dot.

Use -DSPPAR_BUILD_DOCS=TRUE with cmake.


