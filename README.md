# General Utility Library

The General Utility Library (GUL) for C++14 contains often-used utility functions and
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

To get started, have a look at the [documentation website](https://gul-cpp.github.io/gul14/)

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

GUL14 is available from the [vcpkg](https://vcpkg.io/) package manager. Once you have
vcpkg installed, just run:

        vcpkg install gul14

## Building <a name="Building"></a>

Clone this repository:

        git clone https://github.com/gul-cpp/gul14.git
        cd gul14

Use meson to configure the build you want to have. A list of the
[build options](#Build-configuration-switches) is given below. Build directory name can be
what you want. If you use directories starting with 'build' they will be ignored by git.
Examples:

        meson build
        meson -D tests=false build_without_tests
        meson --prefix=/usr -D buildtype=release build_rel
        meson -D buildtype=release -D unity=on build_rel_unity
        meson -D buildtype=release -D docs=false build_nodocs_rel
        CXX=clang++ meson build_clang

Typical build configurations can be easily summoned as detailed in
[Default configurations](#Default-configurations) below.

Afterwards, enter the build directory – in this case, for the release build – and start
the build process:

        cd build_rel
        ninja
        ninja test
        ninja data/docs

If you need one, a Debian package is easily created with _(Note 2)_:

        debuild

Packaging is closely coupled to how version information is extracted (see
[Versioning](#Versioning) below). The debian/ files including the debian/changelog are
autogenerated on configure time. You need to reconfigure your build directory if
tags/versions/commits change afterwards.

_Notes:_

1. The configuration to be changed can immediately follow the ``-D`` for example
   ``-Dwarning_level=1``
2. ``debuild`` is the normal Debian packager. For DESY environments, consider using
   ``makeDdeb`` instead.
3. It is not necessary to call ``ninja`` before calling ``debuild``/``makeDdeb``. But of
   course you usually want to run ``ninja test`` before you package.
4. ``debuild``/``makeDdeb`` has to be called in the build directory of choice (that has
   been usually created with buildtype = release). Note that a proper Debian package
   needs the prefix to be set to /usr (see examples above)

### Build configuration switches <a name="Build-configuration-switches"></a>

You can list all possible build configuration switches with ``meson configure``
in an existing build directory [1]. This command can also be used to change
build configurations after the directory has been created. For example to
decrease the warning level after the build directory has already been created:
``meson configure -D warning_level=1`` (Use ``mesonconf`` if your meson is too
old.)

A lot of the switches change the versioning behaviour, more information on the
versioning is given in a [separate paragraph](#Versioning).

In addition to meson's standard switches there are:

    Option         Default Value  Possible Values  Description
    ------         -------------  ---------------  -----------
    docs           true           [true, false]    Generate documentation via doxygen
    tests          true           [true, false]    Generate tests
    deb-dev-name   @0@-dev        string           Debian package name for development package
    deb-name       @0@            string           Debian package name
    deb-vers-pack  false          [true, false]    Debian package name will contain version
    deb-vers-tag   v              string           Debian package uses git version tags starting with this

The ``deb-*name`` switch can be used to configure the debian package building
process with non-standard packet names. The substring '@0@' will be replaced by
the canonical packet name (i.e. 'libgul14'). Although non-standard packages
automatically conflict with standard packages care must be taken to keep the
install target system consistent (i.e. install always only one variant of
libgul14).

The ``deb-vers-ext`` switch forces the package names and versions to be based on
the latest 'git external tag'. This is a tag in the git repository that does not
start with 'v'. It is expected to give the version number to use in the form
``DESCRIPTION_major_minor_patch``, where major, minor, and patch are positive
integers.

The ``deb-vers-pack`` switch appends the version number to the library and
package names. With this multiple different versions of GUL can be installed in
parallel.

The ``deb-vers-tag`` option specifies how git tag start that are considered as
version number tags. Tags selected here shall have a number after their start,
in the form ``[0-9]+[._][0-9]+([._][0-9])?``. The actual version is deduced from
this and the changelog walks the git repository to find previous versions that
fit this pattern. Note that only the startstring is used to identify these tags,
that they conform to the given regex is to be ensured by the user.

Overview of maybe useful standard project options:

    Option         Default Value         Description
    ------         -------------         -----------
    prefix         /usr/local            Installation prefix
    libdir         lib/...               Library directory
    includedir     include               Header file directory
    datadir        share                 Data file (Doxygen website) directory

### Default configurations for Unix-like systems <a name="Default-configurations"></a>

Some typical build configurations can be set up in build directories under ``build/`` with
the following commands:

* ``make release`` builds the release version of the library under build/x86\_64-linux-gnu/release
* ``make debug`` builds the debug version of the library under build/x86\_64-linux-gnu/debug
* ``make doocs-release`` builds a DOOCS release version that can be packaged with makeDdeb under build/x86\_64-linux-gnu/doocs-release
* ``make test`` runs unit tests on the release version
* ``make BUILDTYPE=debug test`` runs unit tests on the debug version
* ``make test-junit`` runs unit tests and generates output in JUnit XML format under build/x86\_64-linux-gnu/release/test.xml
* ``make JUNIT_XML_FILE=a.xml test-junit`` runs unit tests and generates output in JUnit XML format in the specified file
* ``make clean`` cleans up the build files of the release version
* ``make BUILDTYPE=debug clean`` cleans up the build files of the debug version
* ``make mrproper`` removes all build directories completely
* ``make doc`` builds documentation under build/x86\_64-linux-gnu/release/resources/doxygenerated (needs Doxygen and Graphviz)
* ``make localinstall`` makes a local installation of the library under /home/Ubuntu-16.04-x86\_64/lib

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

The versioning of libgul can be divided into two entities:
* the API version
* the packet version

The API has a semantic version number. It has a major and a minor number. Major
is increased on non-backward compatible changes to the API, while the minor part
is increased when features are added. This results in a API versions like
``1.7``.  The API version is hard coded in the main meson.build file, and the
git revision where it is introduced is tagged with an annotated tag like
``v1.7`` - the lower case letter ``v`` followed by the API version.  Note that
the 'patch number' of usual semantic versioning schemes is not defined here.

The packet version is determined by the latest annotated git tag, that matches a
given format: It has to start with the string given as deb-vers-tag.  The
default is to look for tags starting with ``v``, which are the API version tags.
To continue with the example of the previous paragraph the build system will
look for the latest tag that starts with ``v`` and finds ``v1.7`` extracts the
version identifier (all that follows the deb-vers-tag: ``1.7``).

The packet version needs to take into account that we could have bugfix packets
(packets with the same API version). For this purpose the 'patchlevel' or patch
number is calculated: It is the number of git commits that the current HEAD is
separated from the tagged API version commit.  If GUL is packaged directly from
a commit with a new deb-vers-tag version this will be zero. If there are for
example 2 bug-fix commits on top, that number will be two.

Example:
```
bf184c8 (HEAD) bugfix: Do standalone test on string_util.h
9990745 Remove some static code analysis warnings
8ab1913 tests: Fix time critical tests on Windows
b4b6847 (tag: v1.7) Bump version number to 1.7
18499a3 to_number: Do less thoroughly random test on ARM
...
```

If we package the HEAD commit, the API version is ``1.7``, the packet version is
``1.7.3``.


While the API version is fixed to tags starting with 'v' (e.g. ``v3.5``) the
packet version can examine a different/custom set of tags (deb-vers-tag option).
Normal Debian packets of course have the same (base) version as the API.

So here once again:

The first tag family donates the API version of the package that is in effect
starting with the tagged commit. The version uses semantic versioning, and the
format is defined as ``v1.2`` where 1 is the major, 2 the minor version-part. It
always starts with a lower case ``v``. This tag is cross checked with the
project version number given in the main meson.build file.

Package versioning can be based on that API version tags, or on any other tags
set that start with the same text. Normal packets use the API tags. If another
tag is desired the ``deb-vers-tag`` prefix must be specified.

The package version patchlevel is automatically determined from the number of
commits since the package version tagged commit, and added where appropriate as
third number.

That tags are used to tag specific points in time when a packet has been created
from the project. Its form is ``name_1.2.3``, and again 1, 2, and 3 donate
version number parts, that might or might not be semantic (see recommended
format in the description of the deb-vers-tags option). These tags can be used
to create packet names in the form libgul14-1-2-3 (with ``-D
deb-vers-pack=true`` set in meson). Note that the ``name_`` part is ignored and
can be arbitrary, as long as it does not start with lower case 'v'. The amount
of numbers is arbitrary and just all ``_`` or ``.`` get substituted by ``-`` to
fit required format.

Here again a patchlevel is determined by the number of commits since the packet version tagged commit, and added where appropriate as either a semantic versioning patchlevel (i.e. third number) or explicit patchlevel (i.e. added ``p<number>``)

The packaging rules prevent building dirty repositories. Commit your changes first.

### Version number examples <a name="Version-number-examples"></a>

The versioning related switches work in the following way. Assume that the API
version tag is ``v0.1`` and it is 3 commits behind; and the external tag version
is ``D_18_11_7`` and it is 5 commits behind.

    deb-vers-tag   dev-vers-pack
        'v'            false            libgul14_0.1.3.deb                       ->   libgul14.so.0.1
        'v'            true             libgul14-0-1_0.1.3.deb                   ->   libgul14.so.0.1
        'D_'           false            libgul14_18.11.7.p5.deb                  ->   libgul14.so.18.11.7
        'D_'           true             libgul14-18-11-7_18.11.7.p5.deb          ->   libgul14.so.18.11.7
