# Generate an architecture descriptor like x86_64-linux
ARCH := $(shell uname -ms | sed -E 's/([^ ]+) (.+)/\2-\1/' | tr '[A-Z]' '[a-z]')

# Hack: Limit ARM targets to sequential compilation to avoid out-of-memory problems
ifneq "$(findstring arm,$(shell uname -m))" ""
NINJA_ARGS += -j 1
endif

# On Darwin, build universal binaries with 32- and 64-bit code
ifeq "$(shell uname -s)" "Darwin"
MESON_EXTRA_ARGS += -D 'cpp_args=-arch i386 -arch x86_64'
MESON_EXTRA_ARGS += -D 'cpp_link_args=-arch i386 -arch x86_64'
PREFIX ?= /local
endif

BUILDTYPE = release
BUILDDIR = build/$(ARCH)/$(BUILDTYPE)

# LOCALINSTDIR The build dir used for local install
LOCALINSTDIR = build/$(ARCH)/localinstall-$(BUILDTYPE)
DOOCSARCH ?= unknown_doocsarch
LOCALINSTPRE = $(abspath ../../../${DOOCSARCH})
LOCALSECTION := $(notdir $(abspath ../..))/$(notdir $(abspath ..))

PREFIX ?= /export/doocs

JUNIT_XML_FILE ?= $(BUILDDIR)/test.xml

# Colorful toys
INTRO = \033[1;34m------------
OUTRO = ------------\033[0m


help:
	@echo This makefile is a wrapper for meson/ninja.
	@echo You are encouraged to use meson and ninja directly to build the library.
	@echo
	@echo \'make release\' builds the release version of the library under build/$(ARCH)/release
	@echo \'make debug\' builds the debug version of the library under build/$(ARCH)/debug
	@echo
	@echo \'make test\' runs unit tests on the release version
	@echo \'make BUILDTYPE=debug test\' runs unit tests on the debug version
	@echo \'make test-junit\' runs unit tests and generates output in JUnit XML format under build/$(ARCH)/release/test.xml
	@echo \'make JUNIT_XML_FILE=a.xml test-junit\' runs unit tests and generates output in JUnit XML format in the specified file
	@echo
	@echo \'make clean\' cleans up the build files of the release version
	@echo \'make BUILDTYPE=debug clean\' cleans up the build files of the debug version
	@echo \'make mrproper\' removes all build directories completely
	@echo
	@echo \'make doc\' builds documentation under build/$(ARCH)/release/data/doxygenerated \(needs Doxygen\)
	@echo
	@echo \'make localinstall\' makes a local installation of the library under $(LOCALINSTPRE)/lib


# Needed for NetBeans IDE
build-tests:
	@printf "$(INTRO) $@ $(OUTRO)\n"

clean: $(BUILDDIR)/build.ninja
	@printf "$(INTRO) $@ $(OUTRO)\n"
	ninja $(NINJA_ARGS) -C $(BUILDDIR) clean

debug: build/$(ARCH)/debug/build.ninja
	@printf "$(INTRO) $@ $(OUTRO)\n"
	ninja $(NINJA_ARGS) -C build/$(ARCH)/debug

doc: $(BUILDDIR)/build.ninja
	@printf "$(INTRO) $@ $(OUTRO)\n"
	ninja $(NINJA_ARGS) -C $(BUILDDIR) data/docs

doocs-release: release

localinstall: $(LOCALINSTDIR)/build.ninja
	@printf "$(INTRO) $@ $(OUTRO)\n"
	ninja $(NINJA_ARGS) -C $(LOCALINSTDIR) install

mrproper:
	@printf "$(INTRO) $@ $(OUTRO)\n"
	rm -rf build

release: $(BUILDDIR)/build.ninja
	@printf "$(INTRO) $@ $(OUTRO)\n"
	ninja $(NINJA_ARGS) -C $(BUILDDIR)

test: $(BUILDDIR)/build.ninja
	@printf "$(INTRO) $@ $(OUTRO)\n"
	ninja $(NINJA_ARGS) -C $(BUILDDIR) test

test-junit: $(BUILDDIR)/build.ninja
	@printf "$(INTRO) $@ $(OUTRO)\n"
	ninja $(NINJA_ARGS) -C $(BUILDDIR) tests/libgul-test
	@$(BUILDDIR)/tests/libgul-test -r junit >$(JUNIT_XML_FILE)
	@echo Test results in JUnit format written to $(JUNIT_XML_FILE)

build/$(ARCH)/debug/build.ninja:
	@printf "$(INTRO) $@ $(OUTRO)\n"
	meson setup build/$(ARCH)/debug --buildtype=debug $(MESON_EXTRA_ARGS)

$(BUILDDIR)/build.ninja:
	@printf "$(INTRO) $@ $(OUTRO)\n"
	meson setup $(BUILDDIR) $(MESON_EXTRA_ARGS)

$(LOCALINSTDIR)/build.ninja:
	@printf "$(INTRO) $@ $(OUTRO)\n"
	meson setup --prefix ${LOCALINSTPRE} --bindir 'obj/${LOCALSECTION}' \
              ${LOCALINSTDIR} $(MESON_EXTRA_ARGS)

.PHONY: build-tests debug clean doc doocs-release help install-doc libs localinstall mrproper release test
