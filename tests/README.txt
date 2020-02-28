cctoos/tests

INTRODUCTION

The cctools/tests directory contains a number of functional tests designed to
exercise cctools commands. The test system can be used as part of a test-driven
development workflow or simply as a regression test suite. Tests generally
compile from source rather than relying on pre-built binaries so that test cases
can be easily built and run for different platforms and architectures.

Tests can be run using the "run-tests" test driver:

    % ./run-tests
    1a_harness_test                  PASS
    lipo-cpusubtype-order            PASS
    bitcode_strip_arm64_32           FAIL WATCHOS
    codesign_allocate_arm64_32       FAIL WATCHOS
    libtool-static                   PASS
    strings-stdin                    PASS
    segedit-extract                  PASS
    segedit-replace                  PASS
    lipo-info                        PASS
    ### 7 of 9 unit-tests passed (77.8 percent)

The cctools test system is organized around Tests and Platforms.

PLATFORMS

The cctools test driver has a well-known list of platforms. This list currently
includes macOS, iOS, watchOS, and tvOS. A platform currently implies three
things:

  * A list of valid architecture flags (e.g., "i386 x86_64")
  * A default architecture flag (e.g., "x86_64")
  * A path to the SDK root.
 
Tests declare the platforms for which they are relevant and are run once for
each relevant platform. The test will pass only if it succeeds on all of its
target platforms. Tests must support a platform.

TESTS

Each test is a directory containing a Makefile and any additional files required
by the test. All of the tests are expected to live in a single test repository
directory. By default, the test repository is a directory named "test-cases" in
the same directory as the "run-tests" test driver. 

The Makefile is required to provide the following things:

  * A declaration of platforms supported by the test. This declaration takes
    the form of a comment typically found at the top of the Makefile:
    
    # PLATFORM: MACOS IOS WATCHOS
    
  * A default target that performs the test for a requested platform. The test
    driver will run make once for each platform specified in the platform
    declaration, passing the platform as a make varaible. Makefiles will use the
    platform to divine which architectures are supported, which SDK to use, etc.

  * A "clean" target that removes all derived files. The test will be cleaned
    before each run and after the test concludes. 

In addition, tests are required to print either "PASS" or "FAIL" as the only
word on a line to STDOUT in order to indicate the test result. Tests are free to
compile whatever code and run whatever tools they need in order to perform the
test.

A sample valid test Makefile is:

    # PLATFORM: MACOS 
    
    PLATFORM = MACOS
    TESTROOT = ../..
    include ${TESTROOT}/include/common.makefile
    
    all:
	    ${PASS_IFF} true
    
    clean:

This test begins with a platform declaration, indicating this test is
appropriate for macOS. It also sets the default platform to MACOS; while
unnecessary for the test driver, it aids in running the test manually from the
command-line.

Then the test locates and loads "common.makefile", which provides initialization
logic common to all tests, including:

  * evaluating the platform
  * locating the cctools binaries to use for the test
  * providing utility commands to aid in PASS/FAIL reporting.

Finally the test runs a utility $(PASS_IFF} which prints PASS if the following
command returns a 0 status and prints FAIL if that command returns non-0.

TEST DRIVER

The test driver, run-tests, supports a number of helpful options:

usage: run-tests [-a] [-c cctools_root] [-r tests_dir] [-t test] [-v]
    -a        - run tests asynchronously, potentially boosting performance.
                This value is currently on by default.
    -c <dir>  - use cctools installed in <dir>. The directory must be a root
                filesystem; i.e., one containing ./usr/bin. By default, cctools
	        will all run through "xcrun", although individual tests have
	        the final say.
    -r <dir>  - run all the tests found in the repository <dir>. By default,
                the test repository is a directory named "test-cases" in the
	        same directory as run-tests.
    -t <test> - run only the test named <test> from the test repository.
    -v        - verbose, print a line for each phase of the test. 

Typical usage runs the tests against the cctools found in the current Xcode toolchain:

    ./run-tests
  
You can also specify a directory of cctools to use instead of relying on the
Xcode toolchain. The tools directory needs to be a cctools project build root,
with tools installed in ./usr/bin and ./usr/local/bin:

    ./run-tests -c /tmp/cctools-922.roots/BuildRecords/cctools-922_install/Root
  
Other options are useful when developing and debugging tests. 
