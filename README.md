# Enfield

[![Build Status](https://travis-ci.org/ysiraichi/enfield.svg?branch=master)](https://travis-ci.org/ysiraichi/enfield)

This project was built on top of [Bison](https://www.gnu.org/software/bison/) (v3.0.2),
[Flex](https://github.com/westes/flex) (2.5.39),
and [JsonCpp](https://github.com/open-source-parsers/jsoncpp) (v1.8.4).

Check out the [documentation here!!](https://ysiraichi.github.io/enfield/)

## Building

Enfield uses CMake. So, in order to build this project, issue the following commands:

```
$ mkdir build && cd build
$ cmake ../ -DJSONCPP_ROOT=<path-to-jsoncpp-libs>
$ make
```

The script for finding JsonCpp is simple, so you will have to explicitly show Enfield
where you installed it (the prefix folder).

## Testing

Enfield uses the [Google test framework](https://github.com/google/googletest)
to test its components.
To enable automated tests, you should issue the ```cmake``` command as follows:

```
$ cmake ../ -DENABLE_TESTS=on
$ make && make test
```

It is possible to specify the root folder of the GTest framework. In order to do that,
you should pass to ```cmake``` the option ```-DGTEST_ROOT=<path-to-gtest-libs>```.

## Hacking

Even though this project is pretty new, it was designed to be extensible. So, here are
a few tips in order to implement your own algorithm to your desired architecture.
Below, I'll list some classes that are important to be aware of.

(Note that this is a (really brief) 'begginers guide', so you can do more stuff once
you learn the code)

* ```efd::QModule```: The core class of enfield. It holds the AST preprocessed to be
easier to use, as well as some other useful methods for modifying the AST.
i.e.: ```insertSwapAfter```; ```insertNodeAfter```; ```replaceAllRegsWith```, etc.

* ```efd::Pass```: Enfield is a pass-based compiler.
Much like LLVM (but far from its user base), the transformations are implemented as
```Pass``` for ```QModules```.
There are plenty (ugly, though) so that you can implement yours!
Good luck;

* ```efd::QbitAllocator```: This is the base class for implementing allocators.
In order to implement your own, you should extend this class and implement the method
```allocate```.
This method is responsible for inserting swaps based on the ```QModule``` (that is
given by parameter).
After that, you have to surrender to the "beauty" of C++ macros, and modify the file
```Allocators.def```, so that your allocator is available on command line;

* ```Architectures.def```: Take a look in this file in order to create the specification
of other architectures.
YES!
More Macros!
You will have to specify your architecture in a JSON format.
Check the other architecture descriptions for a better understanding.
