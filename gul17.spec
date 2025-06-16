#
# This file is used to build RPMS for gul17
#
# To use it do the following:
#
# 1) create a dist tarball
#   $ meson build && ninja -C build dist
#
# 2) Prepare the rpm build environment
#   $ pkcon install rpmdevtools
#   $ rpmdev-setuptree
#   $ cp build/meson-dist/gul17-<version>.tar.xz ~/rpmbuild/SOURCES
#   $ cp gul17.spec ~/rpmbuild/SPECS
#
# 3) Build the package
#   $ cd ~/rpmbuild
#   $ rpmbuild -ba SPECS/gul17.spec
#
# https://rpm-packaging-guide.github.io/
#

Name:           gul17
Version:        25.4.1
Release:        1%{?dist}
Summary:        General Utility Library

License:        LGPL-2.1+
URL:            https://github.com/gul-cpp/gul17
Source0:        gul17-%{version}.tar.xz

%if 0%{?rhel} < 8
BuildRequires:  devtoolset-7-gcc-c++
# This is needed to fix an issue with the currently meson
# package, it might be obsolete as soon as a new meson
# package is rolled out.
BuildRequires:  epel-rpm-macros
%endif
BuildRequires:  gcc-c++
BuildRequires:  meson
BuildRequires:  ninja-build
# we actually use some git checks in meson
BuildRequires:  git-core

%description
The general utility library for C++17 contains often-used
utility functions and types that form the foundation for
other libraries and programs.

%package devel
Summary:        Development libraries and header files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}
# We can use BuildRequire:  pkgconfig(gul17)
# in using projects to automatically use either one:
##Requires:       pkgconfig
##Requires:       pkgconf-pkg-config

%description devel
%{summary}.

%package static
Summary:        Static libraries for %{name}
Requires:       %{name}-devel%{?_isa} = %{version}-%{release}

%description static
%{summary}.

%ifarch %{arm} aarch64
# Disable the creation of debug packages on ARM
%global debug_package %{nil}
%endif

%prep
%setup -q -n gul17-%{version}

%build
%meson
%meson_build

%install
%meson_install

%check
%meson_test

%files
%{_libdir}/lib%{name}.so.*

%files devel
%{_libdir}/lib%{name}.so
%{_libdir}/pkgconfig/%{name}.pc
%{_includedir}/gul17/*

%files static
%{_libdir}/lib%{name}.a

%changelog
* Mon Jun 16 2025 Fini Jastrow <ulf.fini.jastrow@desy.de> - 25.4.1-1
- New upstream release

* Fri Feb 16 2024 Soeren Grunewald <soeren.grunewald@desy.de> - 2.10-1
- New upstream release

* Thu Oct 13 2022 Soeren Grunewald <soeren.grunewald@desy.de> - 2.7-1
- New upstream release

* Thu Aug 19 2021 Soeren Grunewald <soeren.grunewald@desy.de> - 2.6-1
- Initial release
