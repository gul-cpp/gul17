#!/bin/bash

if [ "$#" -eq 0 ]; then

cat << EOT
Usage: $0 <directories>

This script will update C++ source files and meson.build files in the
given directories to migrate from GUL14 to GUL17. The given directories
are searched recursively.

The migration happens in place, so make sure to have a backup (or a
recent git commit) of your files.

This is a simple text replacement. Some manual modifications may be
necessary to finish the migration and to make the code compile again.

EOT

exit 1
fi

create_gul17_wrap_file() {
    cat > "$1" << EOT
[wrap-git]
directory = gul17
url = https://github.com/gul-cpp/gul17.git
revision = main

[provide]
dependency_names = gul17
gul17 = libgul_dep
EOT
}

handle_control_file() {
    sed -i -E -e 's/dev-doocs-libgul14/gul17-dev/g' "$1"
}

handle_meson_file() {
    sed -i -E \
        -e 's/(dependency\s*\(\s*)'"'libgul14'/\1'gul17'"'/g' \
        -e "s/'libgul14'/'gul17'/g" \
        "$1"
}

handle_source_file() {
    sed -i -E \
        -e 's/(#include\s*["<])gul14\//\1gul17\//g' \
        -e 's/(#include\s*)["<]gul17\/string_view.h[">]/\1<string_view>/g' \
        -e 's/(#include\s*)["<]gul17\/optional.h[">]/\1<optional>/g' \
        -e 's/(#include\s*)["<]gul17\/variant.h[">]/\1<variant>/g' \
        -e 's/gul14::/gul17::/g' \
        -e 's/gul17::string_view/std::string_view/g' \
        -e 's/gul17::optional/std::optional/g' \
        -e 's/gul17::variant/std::variant/g' \
        -e 's/gul17::nullopt/std::nullopt/g' \
        -e 's/gul17::void_t/std::void_t/g' \
        -e 's/gul17::visit/std::visit/g' \
        -e 's/gul17::get(\s*)</std::get\1</g' \
        -e 's/gul17::get_if(\s*)</std::get_if\1</g' \
        -e 's/gul17::holds_alternative/std::holds_alternative/g' \
        -e 's/gul17::in_place_type/std::in_place_type/g' \
        -e 's/(using\s+namespace\s+)gul14/\1gul17/g' \
        "$1"
    if grep -q "make_overload_set" "$1"; then
        mos_found=true
    fi
}

mos_subdirs=
for dir in "$@"; do
    mos_found=false
    if [ -f "${dir}/meson.build" ]; then

        # Meson files
        while IFS= read -d $'\0' -r file; do
            handle_meson_file "$file"
        done < <( find "${dir}" \
            \( -path "./${dir}/build*" -o -path "./${dir}/debian" -o -path "./${dir}/subprojects" \) -prune \
            -o \( -name 'meson.build' -print0 \) )

        # source files
        while IFS= read -d $'\0' -r file; do
            handle_source_file "$file"
        done < <( find "${dir}" \
            \( -path "./${dir}/build*" -o -path "./${dir}/debian" -o -path "./${dir}/subprojects" \) -prune \
            -o \( \( -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cpp" \) -print0 \) )

        # Debian control file
        if [ -f "${dir}/debian/control" ]; then
            handle_control_file "${dir}/debian/control"
        fi

        # wrap file
        if [ -d "${dir}/subprojects" ]; then
            if [ ! -e "${dir}/subprojects/gul17.wrap" ]; then
                echo "Creating gul17.wrap file in ${dir}/subprojects"
                create_gul17_wrap_file "${dir}/subprojects/gul17.wrap"
            fi
        fi

    fi
    if [ "${mos_found}" = true ]; then
        mos_subdirs+=${dir}
    fi
done

if [ -n "${mos_subdirs}" ]; then
    cat << EOT

Warning: The function 'make_overload_set' is used in the following directory/ies:
${mos_subdirs}
This function does not exist in GUL17. Manual migration is required.
Here's the recommended pattern:

#include <gul17/OverloadSet.h>

auto TypeOfIntegral = gul17::OverloadSet
{
    [](char) { return "char"; },
    [](int) { return "int"; },
    [](unsigned int) { return "unsigned int"; },
    [](auto) { return "unknown type"; },
};

for (auto v : container_of_variants)
    std::cout << std::visit(TypeOfIntegral, v) << '\\n';

EOT
fi
