# ===========================================================================
#                  SeqAn - The Library for Sequence Analysis
# ===========================================================================
# File: /apps/snp_store/CMakeLists.txt
#
# CMakeLists.txt file for SNP Store.
# ===========================================================================

cmake_minimum_required (VERSION 3.12)
project (seqan_apps_snp_store CXX)
message (STATUS "Configuring apps/snp_store")

set (SEQAN_APP_VERSION "1.3.11")

# ----------------------------------------------------------------------------
# Dependencies
# ----------------------------------------------------------------------------

# Search SeqAn and select dependencies.
if (NOT "${SEQAN_BUILD_SYSTEM}" STREQUAL "DEVELOP")
    find_package (ZLIB)
    find_package (Boost)
    find_package (SeqAn CONFIG REQUIRED)
endif ()

if (NOT Boost_FOUND)
    message (STATUS "  Skipping snp_store, missing Boost")
    return ()
endif ()

if (NOT ZLIB_FOUND)
    message (STATUS "  Skipping snp_store, missing zlib")
    return ()
endif ()

# ----------------------------------------------------------------------------
# Build Setup
# ----------------------------------------------------------------------------

# Add include directories.
include_directories (${SEQAN_INCLUDE_DIRS})
include_directories (SYSTEM ${Boost_INCLUDE_DIRS})

# Add definitions set by find_package (SeqAn).
add_definitions (${SEQAN_DEFINITIONS})

# Add definitions set by the build system.
add_definitions (-DSEQAN_APP_VERSION="${SEQAN_APP_VERSION}")
add_definitions (-DSEQAN_REVISION="${SEQAN_REVISION}")
add_definitions (-DSEQAN_DATE="${SEQAN_DATE}")

# Update the list of file names below if you add source files to your application.
add_executable (snp_store snp_store.cpp snp_store.h)

# Add dependencies found by find_package (SeqAn).
target_link_libraries (snp_store ${SEQAN_LIBRARIES})

# Add CXX flags found by find_package (SeqAn).
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_CXX_FLAGS}")

# ----------------------------------------------------------------------------
# Setup Common Tool Description for Generic Workflow Nodes
# ----------------------------------------------------------------------------

# Include executable snp_store in CTD structure.
set (SEQAN_CTD_EXECUTABLES ${SEQAN_CTD_EXECUTABLES} snp_store CACHE INTERNAL "")

#Add test to check correct ctd generation.
seqan_add_ctd_test (snp_store)

# ----------------------------------------------------------------------------
# Installation
# ----------------------------------------------------------------------------

# Set variables for installing, depending on the selected build type.
if (NOT SEQAN_PREFIX_SHARE_DOC)
  seqan_setup_install_vars (snp_store)
endif (NOT SEQAN_PREFIX_SHARE_DOC)

# Install snp_store in ${PREFIX}/bin directory
install (TARGETS snp_store
         DESTINATION ${CMAKE_INSTALL_BINDIR})

# Install non-binary files for the package to "." for app builds and
# ${PREFIX}/share/doc/snp_store for SeqAn release builds.
install (FILES LICENSE
               README
         DESTINATION ${SEQAN_PREFIX_SHARE_DOC})
install (FILES example/exampleGenome.fa
               example/exampleReads.gff
         DESTINATION ${SEQAN_PREFIX_SHARE_DOC}/example)

# ----------------------------------------------------------------------------
# App Test
# ----------------------------------------------------------------------------

seqan_add_app_test (snp_store)

# ----------------------------------------------------------------------------
# CPack Install
# ----------------------------------------------------------------------------

if (SEQAN_BUILD_SYSTEM STREQUAL "APP:snp_store")
    set (CPACK_PACKAGE_NAME "snp_store")
    set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "snp_store - SNP and indel calling")
    set (CPACK_DEBIAN_PACKAGE_MAINTAINER "David Weese <david.weese@fu-berlin.de>")
    set (CPACK_PACKAGE_VENDOR "SeqAn Team, FU Berlin")

    seqan_configure_cpack_app (snp_store "snp_store")
endif (SEQAN_BUILD_SYSTEM STREQUAL "APP:snp_store")
