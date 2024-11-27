# Process all the source files of the library

libgul_src = files([
    'case_ascii.cc',
    'cat.cc',
    'escape.cc',
    'replace.cc',
    'string_util.cc',
    'ThreadPool.cc',
    'to_number.cc',
    'Trigger.cc',
    'trim.cc',
])

inc += include_directories('.')

# Normally the version is x.y.z, see README.md
# But sometimes something is added to the version
# by a package maintainer and we have x.y.z-more
#
# The additional suffix is only used for packaging and
# does not reflect in the so-version or pkgconfig version.
lib_version_full = meson.project_version()
lib_version_parts = meson.project_version().split('.')
if lib_version_parts.length() < 3
    error('Version does not have expected two dots (x.y.z)')
endif
lib_version_num = '@0@.@1@.@2@'.format(
    lib_version_parts[0],
    lib_version_parts[1],
    lib_version_parts[2].underscorify().strip(
        'abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ'))

git_conf = configuration_data()
git_conf.set('FULL_VERSION', lib_version_full)
version_cc = configure_file(
    input : 'version.cc.in',
    output : 'version.cc',
    configuration : git_conf,
)

so_version = lib_version_num

add_cpp_args = [ ]
if meson.get_compiler('cpp').has_argument('-Wshadow') # msvc doesnt have
    add_cpp_args += [ '-Wshadow' ]
endif
if meson.get_compiler('cpp').has_argument('-Wconversion') # msvc doesnt have
    add_cpp_args += [ '-Wconversion' ]
endif
if meson.get_compiler('cpp').has_argument('-Wnrvo') # msvc: included in permissive-
    add_cpp_args += [ '-Wnrvo' ]
endif

deps = dependency('threads')

## Static and shared libraries

if get_option('default_library') in ['both', 'static']
    libgul_static = static_library(
        meson.project_name(), libgul_src + [ version_cc ],
        cpp_args : add_cpp_args + [ '-DGUL_USING_STATIC_LIB_OR_OBJECTS' ],
        dependencies : deps,
        include_directories : inc,
        install : true
    )

    libgul_static_dep = declare_dependency(
        dependencies : deps,
        include_directories : inc,
        link_with : libgul_static,
        compile_args : [ '-DGUL_USING_STATIC_LIB_OR_OBJECTS' ],
    )

    libgul = libgul_static
    libgul_dep = libgul_static_dep
endif

if get_option('default_library') in ['both', 'shared']
    libgul_shared = shared_library(
        meson.project_name(), libgul_src + [ version_cc ],
        soversion : so_version,
        cpp_args : add_cpp_args + [ '-DGUL_COMPILING_SHARED_LIB' ],
        dependencies : deps,
        include_directories : inc,
        gnu_symbol_visibility : 'hidden',
        install : true
    )

    libgul_shared_dep = declare_dependency(
        dependencies : deps,
        include_directories : inc,
        link_with : libgul_shared,
    )

    libgul = libgul_shared
    libgul_dep = libgul_shared_dep
endif


## Pkg-config file

pkg = import('pkgconfig')
pkg.generate(
    libgul,
    libraries : [ '-Wl,-rpath,${libdir}' ],
    name : meson.project_name(),
    description : 'General Utility Library for C++17',
    version : '@0@'.format(lib_version_num)
)

# vi:ts=4:sw=4:sts=4:et:syn=conf
