# General Utility Library for C++17

The General Utility Library for C++17 (GUL17) contains often-used utility functions and
types that form the foundation for other libraries and programs. The main fields covered
by the library are:

  * String handling
  * Concurrency
  * Time
  * Statistics and numerics
  * Debugging
  * Containers
  * Type traits
  * Core guideline support library (GSL) excerpts
  * Backports from new standard library extensions

To get started, have a look at ~the [documentation website](https://gul17.info/)~
the [gul14 documentation website](https://gul14.info/)

Here you find more detailed information on:

* [Building](#Building)
  * [Build configuration switches](#Build-configuration-switches)
  * [Default Configurations](#Default-configurations)
  * [Building on Windows with Visual C++](#Building-on-Windows-with-Visual-C)
  * [Single header use](#Single-header-use)
* [Testing](#Testing)
* [Coverage report generation](#Coverage-report)
* [Versioning](#Versioning)
  * [Version number examples](#Version-number-examples)

## Installing with vcpkg

~GUL17 is available from the [vcpkg](https://vcpkg.io/) package manager. Once you have
vcpkg installed, just run:~

~vcpkg install gul17~

## Building <a name="Building"></a>

Clone this repository:

        git clone https://github.com/gul-cpp/gul17.git
        cd gul17

Use meson to configure the build you want to have. A list of the
[build options](#Build-configuration-switches) is given below. Build directory name can be
what you want and can even be located outside of the projecte directory. E.g. on Linux you
can place the build directory into a tmpfs mount (aka RAM disk).
If your build directories are inside the project root directory and starting with 'build'
they will be ignored by git.
Examples:

        meson setup <build-dir>
        meson setup -D tests=false <build-without-tests-dir>
        meson setup --prefix=/usr --buildtype=release <build-rel-dir>
        meson setup --buildtype=release --unity=on <build-rel-unity-dir>
        meson setup --buildtype=release -D docs=false <build-nodocs-rel-dir>
        meson setup -Dcpp_args='-Wconversion -Wshadow' --buildtype=debug <build-with-extra-warnings-dir>
        CXX=clang++ meson setup <build-clang-dir>

Typical build configurations can be easily summoned as detailed in
[Default configurations](#Default-configurations) below.

Afterwards, enter the build directory – in this case, for the release build – and start
the build process:

        cd <build-dir>
        ninja
        ninja test
        ninja data/docs

_Notes:_

1. The configuration to be changed can immediately follow the ``-D`` for example
   ``-Dwarning_level=1``
2. ``debuild`` is the normal Debian packager. For DESY environments, consider using
   ``makeDdeb`` instead.
3. It is not necessary to call ``ninja`` before calling ``debuild``/``makeDdeb``. But of
   course you usually want to run ``ninja test`` before you package.
4. ``debuild``/``makeDdeb`` has to be called in the repo root directory.

### Build configuration switches <a name="Build-configuration-switches"></a>

You can list all possible build configuration switches with ``meson configure``
in an existing build directory [1]. This command can also be used to change
build configurations after the directory has been created. For example to
decrease the warning level after the build directory has already been created:
``meson configure -D warning_level=1`` (Use ``mesonconf`` if your meson is too
old.)

In addition to meson's standard switches there are:

    Option         Default Value  Possible Values  Description
    ------         -------------  ---------------  -----------
    docs           true           [true, false]    Generate documentation via doxygen
    tests          true           [true, false]    Generate tests

Overview of maybe useful standard project options:

    Option         Default Value         Description
    ------         -------------         -----------
    prefix         /export/doocs         Installation prefix
    libdir         lib/...               Library directory
    includedir     lib/include           Header file directory
    datadir        share                 Data file (Doxygen website) directory

### Building on Windows with Visual C++ <a name="Building-on-Windows-with-Visual-C"></a>

GUL can be built with Visual C++, although the integration with the Visual Studio IDE is
somewhat patchy. In principle, meson and ninja can be used as outlined above. However,
the devil is in the details:

* For Meson, prefer the MSI installer from https://mesonbuild.com/. We found that Meson
  versions packaged with the Anaconda Python distribution do not work, to name an
  example.
* For Ninja, the version from the Meson MSI installer works just as well as the one
  distributed with Visual Studio 2019 (in the cmake workload).

Setting up the PATH and calling the correct executable can be difficult. We distribute a
Windows batch file ``tools\make_vs.bat`` as an example. It may require some editing to
adapt it to your system, but afterwards you can call it from the command line or directly
from Visual Studio to initiate a build, cleanup, or to run the unit test suite.

    Usage:
    make_vs mrproper                    - Remove the entire build directory
    make_vs release <platform>          - Build the release version of the library for the specified platform
    make_vs debug <platform>            - Build the debug version of the library for the specified platform
    make_vs test <buildtype> <platform> - Run the unit tests for the specified build type and platform
    where:
    <buildtype> is one of "release" or "debug"
    <platform> is one of "x86" or "x64"

The files ``.vs\tasks_vs.json`` and ``.vs\launch_vs.json`` as well as
``CppProperties.json`` in the root directory contain information for Visual Studio 2019
that should allow you to open the folder via "open folder" (i.e. without a project file)
and still be able to build and debug GUL via the batch file.

It is not possible to build GUL with a version prior to Visual Studio 2017 version 15.7
(a.k.a. 19.14), because basic language features are
[missing](https://docs.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance)
(e.g. two phase name lookup is needed, N3652 and N3653 are used in GUL code).

### Single header use <a name="Single-header-use"></a>

Under some circumstances it is nice to have all of GUL in only one header file that can
be included - and no library to link to. For example when doing experiments with online
compilers.

This is not the recommended way to use GUL, but you can use the script `tools/single_gul`
to create such a complete header file (without Catch2).

## Testing <a name="Testing"></a>

After configuring the build directory one can run all tests by

        ninja test

To see which tests have failed and get other additional information you might want to do
instead

        meson test --test-args '-r junit'

Or for older meson versions

        mesontest --test-args '-r junit'

## Coverage report <a name="Coverage-report"></a>

To generate a coverage report you need to specify ``-D b_coverage=true`` on the meson call
for the build configuration. Then after running the tests (see previous paragraph) the
coverage report is generated by

        ninja coverage-html

Depending on the installed coverage tool the targets ``coverage-text`` and
``coverage-xml`` might also be there. To generate a coverage report the package ``gcov``
must be installed, and ``lcov/genhtml`` or *(recommended)* ``gcovr`` (>= 3.1) is used to
generate the HTML pages.

## Versioning <a name="Versioning"></a>

The version number for GUL17 resides in the LIBNO file.

GUL17 follows a calendar-oriented versioning scheme with a version number of the
form YY.M.minor where YY stands for the last two digits of the year, M is the
number of the month, and minor is a minor version number that starts from zero
for each release in that month.

For instance, the first release in March 2025 is called `25.3.0`, the second one `25.3.1`.

This is not semantic versioning.
