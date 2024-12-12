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

use_libno = false
if get_option('deb-vers-tag') != 'v'
    # Non-standard i.e. Doocs package => use LIBNO file
    cat = find_program(meson.source_root() / 'tools' / 'cat.py')
    if cat.found()
        use_libno = true
    endif
endif
if use_libno
    libno_cat_run = run_command(cat, meson.source_root() / 'LIBNO', check : false)
    if libno_cat_run.returncode() == 0
        message('Using LIBNO file')
        lib_version_full = libno_cat_run.stdout().split('=')[1].strip()
    else
        lib_version_full = meson.project_version()
    endif
    lib_version_parts = lib_version_full.split('_')
    lib_version_parts = '-'.join(lib_version_parts).split('-')
    lib_version_num = '.'.join(lib_version_parts)
    lib_version_parts = lib_version_num.split('.')
else
    lib_version_full = meson.project_version()
    lib_version_num = libgul_api_version
    lib_version_parts = libgul_api_version_parts
endif

git_conf = configuration_data()
git_conf.set('FULL_API_VERSION', lib_version_full)
git_conf.set('LIBGUL_API_VERSION', libgul_api_version)
version_cc = configure_file(
    input : 'version.cc.in',
    output : 'version.cc',
    configuration : git_conf,
)

so_version = lib_version_num

add_cpp_args = [
    '-DLIBGUL_API_VERSION=@0@'.format(libgul_api_version),
]
if meson.get_compiler('cpp').has_argument('-Wshadow') # msvc doesnt have
    add_cpp_args += [ '-Wshadow' ]
endif
if meson.get_compiler('cpp').has_argument('-Wconversion') # msvc doesnt have
    add_cpp_args += [ '-Wconversion' ]
endif

deps = dependency('threads')

libgul = shared_library(
    meson.project_name(), libgul_src + [ version_cc ],
    soversion : so_version,
    cpp_args : add_cpp_args + [ '-DGUL_COMPILING_SHARED_LIB' ],
    dependencies : deps,
    include_directories : inc,
    gnu_symbol_visibility : 'hidden',
    install : true
)
libgul_static = static_library(
    meson.project_name(), libgul_src + [ version_cc ],
    cpp_args : add_cpp_args + [ '-DGUL_USING_STATIC_LIB_OR_OBJECTS' ],
    dependencies : deps,
    include_directories : inc,
    install : true
)
libgul_dep = declare_dependency(
    dependencies : deps,
    include_directories : inc,
    link_with : libgul,
)
libgul_static_dep = declare_dependency(
    dependencies : deps,
    include_directories : inc,
    link_with : libgul_static,
    compile_args : [ '-DGUL_USING_STATIC_LIB_OR_OBJECTS' ],
)

pkg = import('pkgconfig')
pkg.generate(
    libgul,
    libraries : [ '-Wl,-rpath,${libdir}' ],
    name : meson.project_name(),
    description : 'General Utility Library for C++17',
    version : '@0@'.format(libgul_api_version)
)

# vi:ts=4:sw=4:sts=4:et:syn=conf
