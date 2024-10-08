# Print missing optional dependencies and consequently missing SeqAn features.
#
# We have two main cases.  First, if all libraries were found, we print a
# message indicating that.  In the second case, we print a more highlighted
# message to warn the user.

message("")
message("==============================================================================")
message("                            SeqAn Features")
message("==============================================================================")
message("")
if (ZLIB_FOUND AND BZIP2_FOUND AND Boost_FOUND AND OPENMP_FOUND)
    message("ALL dependencies were found and the SeqAn build system has SUCCESSFULLY")
    message("configured your build files or IDE project files.  ALL functionality has")
    message("been enabled.")
else ()
    message("The SeqAn build system has SUCCESSFULLY configured your build files or IDE")
    message("project files.  Most features of SeqAn depend only on core C/C++ features and")
    message("are available on any platform.  However, some functionality is only enabled")
    message("with external libraries installed.  The following report shows you which of")
    message("these OPTIONAL features are disabled on your system.  Also, it gives you")
    message("some hints on what to do to enable the features.")
endif ()
message("")
if (ZLIB_FOUND)
    message("ZLIB       - FOUND")
else (ZLIB_FOUND)
    message("ZLIB       - NOT FOUND")
    message("")
    message("  zlib is a library for file compression (powering the ubiquitous .gz files,")
    message("  for example.  Reading/writing zlib-compressed streams depends on zlib,")
    message("  especially does support for BAM files.  For Linux systems, zlib is available")
    message("  in all distribution's package repositoryies, make sure to also install the")
    message("  developer files.  For Mac Os X, zlib should be installed together with the")
    message("  XCode Tools.  For Windows, we have prepared a ZIP archive with common")
    message("  binaries, available together with installation instructions here:")
    message("")
    message("    https://trac.seqan.de/wiki/HowTo/InstallContribsWindows")
    message("")
    message("  The following functionality has been disabled:")
    message("")
    message("    * Reading/writing of gzip and BGZF files (Stream<GZFile>, Stream<BGZF>).")
    message("    * Reading/writing of BAM files.")
    message("")
endif (ZLIB_FOUND)
if (BZIP2_FOUND)
    message("BZIP2      - FOUND")
else (BZIP2_FOUND)
    message("BZIP2      - NOT FOUND")
    message("")
    message("  bzlib is a library for compression (powering .bz2 files).  The compression")
    message("  ratio is better than with zlib/gzip but compression and decompression are")
    message("  slower.  For Linux, libbz2 is available in all distribution's package")
    message("  repositories, make sure to also install the developer files.  For Mac Os X,")
    message("  bzlib should be installed together with the XCode Tools.  For Windows, we")
    message("  have prepared a ZIP archive with common binaries, including bzlib, available")
    message("  together with installation instructions here:")
    message("")
    message("    https://trac.seqan.de/wiki/HowTo/InstallContribsWindows")
    message("")
    message("  The following functionality has been disabled:")
    message("")
    message("   * Reading/writing of bz2 files (Stream<BZ2Stream>).")
    message("")
endif (BZIP2_FOUND)
if (OPENMP_FOUND)
    message("OPENMP     - FOUND")
else (OPENMP_FOUND)
    message("OPENMP     - NOT FOUND")
    message("")
    message("  OpenMP is a C/C++ language extension for parallel programming.  It provides")
    message("  C pragmas that allow parallelization-related annotations of the program code")
    message("  and a run time library.  No functionality is disabled but the parallel")
    message("  versions of algorithms will not be available.  Also, some applications will")
    message("  not be built with OpenMP missing (see messages above).")
    message("")
    message("  OpenMP is available in the GCC C++/MinGW and Microsoft Visual C++ compilers.")
    message("  LLVM/Clang currently does not have support for OpenMP.")
    message("")
endif (OPENMP_FOUND)
if (Boost_FOUND)
    message("BOOST      - FOUND")
else (Boost_FOUND)
    message("BOOST      - NOT FOUND")
    message("")
    message("  Boost the largest C++ template library.  Because of its size, we chose not")
    message("  to depend on Boost for core library functionality.  However, some")
    message("  applications depend on Boost, e.g. for the math/statistics Boost libraries.")
    message("  These applications will appear in your build and project files (as indicated")
    message("  above).")
    message("")
endif (Boost_FOUND)
message("")
message("Build Type - ${CMAKE_BUILD_TYPE}")
message("")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
message("             Programs will be built in DEBUG mode.  Debug symbols and")
message("             assertions will be enabled.  The code is compiled with DISABLED")
message("             OPTIMIZATIONS")
message("")
message("                  /!\\ DEBUG mode will result in BAD PERFORMANCE /!\\")
message("")
message("             For PRODUCTION USAGE, you should compile in Release mode. To")
message("             enable the release mode, pass -DCMAKE_BUILD_TYPE=Release to the")
message("             cmake call.")
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
message("             Programs will be built in RELEASE WITH DEBUG SYMBOLS mode.")
message("             Debug symbols and optimizations are enabled, assertions are only")
message("             enabled for the tests.")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
message("             Programs will be built in RELEASE mode. Optimizations are")
message("             enabled, debug symbols are disabled, and assertions are only")
message("             enabled for the tests.")
endif ()
message("")
message("==============================================================================")
message("")
