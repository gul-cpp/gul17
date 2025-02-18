# Build specification for the General Utilities Library

# The version in project() is the Doocs version / package version

project('gul17', 'cpp',
    default_options : [
        'cpp_std=c++17',
        'buildtype=debugoptimized',
        'prefix=/export/doocs',
        'libdir=lib',
        'includedir=lib/include',
        'warning_level=3',
    ],
    version : run_command(
        [
            find_program('tools/cat.py'),
            meson.current_source_dir() / 'LIBNO'
        ],
        check : true
    ).stdout().split('=')[1].strip(),
    meson_version : '>=0.53.2')

subdir('include')
subdir('src')

subdir('tools')
subdir('data')

subdir('tests')

message('Install prefix: ' + get_option('prefix'))
subdir('examples')

message('FULL VERSION            @0@'.format(meson.project_version()))
message('Shared library version  ' + so_version)
