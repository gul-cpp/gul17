#!/bin/bash

if [ "$#" -eq 0 ]; then
    echo "Usage: $0 <directories>"
    echo
    echo "This script will update C++ source files and meson.build files"
    echo "in the given directories to migrate from GUL14 to GUL17."
    echo "The given directories are searched recursively."
    echo
    echo "The migration happens in place, so make sure to have a backup"
    echo "(or a recent git commit) of your files."
    echo
    echo "This is a simple text replacement. Some manual modifications may"
    echo "be necessary to finish the migration and to make the code"
    echo "compile again."
    exit 1
fi

for dir in "$@"; do
    pushd "$dir"

    source_files=$( \
        find . -name "*.h" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cpp" | \
        sed -e '/^.\/build.*\/.*/d' -e '/^.\/debian.*\/.*/d' -e '/^.\/subprojects.*\/.*/d' | \
        sort )
    meson_files=$( \
        find . -name "meson.build" | \
        sed -e '/^.\/build.*\/.*/d' -e '/^.\/debian.*\/.*/d' -e '/^.\/subprojects.*\/.*/d' | \
        sort )

    if [ -f "meson.build" ]; then
        sed -i -E \
            -e 's/(dependency\s*\(\s*)'"'libgul14'/\1'gul17'"'/g' \
            -e 's/\[\s*'"'libgul14'"'\s*,\s*'"'libgul_dep'"'\s*\]/['"'gul17', 'gul17_dep']"'/g' \
            ${meson_files}
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
            ${source_files}
    fi

    if grep -q "make_overload_set" ${source_files}; then
        echo
        echo "Warning: The function 'make_overload_set' is used in directory '${dir}'."
        echo "This function does not exist in GUL17. Manual migration is required."
        echo "Here's the recommended pattern:"
        echo
        echo "template<typename ... Fcts>"
        echo "struct Overload : Fcts ..."
        echo "{"
        echo "    using Fcts::operator() ...;"
        echo "};"
        echo "template<typename... Fcts>"
        echo "Overload(Fcts...) -> Overload<Fcts...>;"
        echo
        echo 'auto TypeOfIntegral = Overload'
        echo '{'
        echo '    [](char) { return "char"; },'
        echo '    [](int) { return "int"; },'
        echo '    [](unsigned int) { return "unsigned int"; },'
        echo '    [](auto) { return "unknown type"; },'
        echo '};'
        echo
        echo "for (auto v : container_of_variants)"
        echo "    std::cout << std::visit(TypeOfIntegral, v) << '\n';"
        echo
    fi

    popd
done
