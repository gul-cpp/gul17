# Build specification for the General Utilities Library

# Increase the major project version in project() on API breaking changes.
# Increase the minor project version in project() on API extensions

project('gul17', 'cpp',
    default_options : [
        'cpp_std=c++17',
        'warning_level=3',
    ],
    version : '2.12.1', # The API version
    meson_version : '>=0.49')

# We have different version numbers
# API VERSION (from APIVERSION file) - the real API
# LIB VERSION (from LIBNO file or APIVERSION file) - the package version

libgul_api_version_parts = meson.project_version().split('.')
libgul_api_version = libgul_api_version_parts[0] + '.' + libgul_api_version_parts[1]

subdir('include')
subdir('src')

subdir('tools')
subdir('data')

subdir('tests')

message('Install prefix: ' + get_option('prefix'))
subdir('debian')
subdir('examples')

message('API VERSION             @0@'.format(libgul_api_version))
message('FULL API VERSION        @0@'.format(meson.project_version()))
message('LIB VERSION             @0@'.format(lib_version_full))
if deb_install
    message('Debian package names    ' + deb_name + ', ' + deb_dev_name)
    message('Debian package version  ' + debian_package_version)
endif
message('Shared library version  ' + so_version)
