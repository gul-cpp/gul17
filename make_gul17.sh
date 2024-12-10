#!/usr/bin/env bash
#
# Create a GUL17 from GUL14 automatically
#
# This drops
# * gul14::in_place_t
# * gul14::optional
# * gul14::string_view
# * gul14::variant
# * gul14::void_t

set -e

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 || exit && pwd -P)"

git reset --hard

cd "${root_dir}/include"
rm -Rf gul17
mv gul14 gul17

######
### Prepare include files
cd "${root_dir}/include/gul17"

rm optional.h
rm string_view.h
rm variant.h
rm utility.h # At the moment contains only in_place_t and monostate

# Rip out void_t while keeping the rest
remove_to=$(grep -nh 'using void_t' traits.h | sed 's/:.*//')
remove_from=$(head -n ${remove_to} traits.h | grep -nh '^$' | tail -n 1 | sed 's/:.*//')
remove_to=$((remove_to + 1))
remove_from=$((remove_from - 1))
mv -f traits.h traits.h_
head -n ${remove_from} traits.h_ > traits.h
echo "using std::void_t;" >> traits.h
tail -n +${remove_to} traits.h_ >> traits.h

######
### Prepare tests
cd "${root_dir}/tests"

rm test_optional.cc
rm test_variant.cc

######
### Patch all the files

cd "${root_dir}"
while IFS= read -d $'\0' -r file ; do
    echo "Patching ${file}"
    sed \
        -Ee 's/([^\/])(gul14::)?(basic_)?string_view/\1std::\3string_view/g' \
        -Ee 's/^#include\s+["<]gul14\/optional.h[">].*/#include <optional>/' \
        -Ee 's/^#include\s+["<]gul14\/string_view.h[">].*/#include <string_view>/' \
        -Ee 's/^#include\s+["<]gul14\/variant.h[">].*/#include <variant>/' \
        -Ee 's/^#include\s+["<]gul14\/utility.h[">].*/#include <utility>/' \
        -Ee 's/(\W)gul14(\W)/\1gul17\2/g' \
        -Ee 's/^gul14(\W)/gul17\1/g' \
        -Ee 's/^(gul17::)?string_view/\1std::string_view/g' \
        -Ee 's/([^A-Za-z0-9_:<])(gul17::)?optional/\1std::optional/g' \
        -Ee 's/([^A-Za-z0-9_:<])(gul17::)?variant/\1std::variant/g' \
        -Ee 's/([^A-Za-z0-9_:])in_place/\1std::in_place/g' \
        -Ee 's/([^A-Za-z0-9_:])holds_alternative/\1std::holds_alternative/g' \
        -Ee 's/([^A-Za-z0-9_:])monostate(\W)/\1std::monostate\2/g' \
        -Ee 's/([^A-Za-z0-9_:])make_optional(\W)/\1std::make_optional\2/g' \
        -Ee 's/([^A-Za-z0-9_:])nullopt(\W)/\1std::nullopt\2/g' \
        -i "${file}"
done < <(find . \( -name '*.h' -o -name '*.cc' \) \
    -a \( -path './include/gul17/*' -o -path './src/*' -o -path './tests/*' \) \
    -a -not -name 'catch.h' -print0 | sort -z)

######
### Patch some files with specific rules

sed 's/14/17/' -i meson.build
sed -E 's/([^.])14/\117/g' -i README.md
sed -E 's/^.*(optional|string_view|variant|utility).h.*//' -i include/gul17/meson.build
sed -E 's/^.*(optional|variant).cc.*//' -i tests/meson.build

sed -E 's/(\W)gul14(\W)/\1gul17\2/g' -i src/version.cc.in
sed '1i #include <variant>' -i include/gul17/expected.h
echo "using std::in_place_t;" >> include/gul17/utility.h

######
### Patch all the remaining places

find . -not  \( \
        -name 'make_gul17.sh' \
        -o -path './.*' \
        -o -path './build*' \
    \) \
    -type f \
    -exec sed -E "s/(gul|GUL)14/\117/g;s/([cC]\+\+)14/\117/" -i {} \;
