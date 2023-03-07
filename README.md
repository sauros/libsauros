<p align="center">
  <img src="https://img.icons8.com/ios-glyphs/90/null/lizard.png"/>
</p>

#libsauros

[![CircleCI](https://dl.circleci.com/status-badge/img/gh/sauros/libsauros/tree/main.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/sauros/libsauros/tree/main)

The Sauros language library.

With this library, the Sauros language can be easily embedded into an application.

Check out the [language wiki](https://github.com/sauros/sauros/wiki) to get information how the language works.

## Embedding Sauros

Embedding sauros into another application can now be done with a `CMakeLists.txt` as follows:

```
find_package(libsauros REQUIRED)
include_directories(${LIBAUROS_INCLUDE_DIRS})
```

and then link the libraries to yourr application with:

```
target_link_libraries(YOUR-PROJECT-NAME-HERE
   PRIVATE
   ${LIBSAUROS_LIBRARIES})
```

Check out the `example` in the root directory to see how this is done and how Sauros is used within an application.
