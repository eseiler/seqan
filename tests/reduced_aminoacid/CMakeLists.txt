# ===========================================================================
#                  SeqAn - The Library for Sequence Analysis
# ===========================================================================
# File: /tests/reduced_aminoacid/CMakeLists.txt
#
# CMakeLists.txt file for the reduced_aminoacid module tests.
# ===========================================================================

cmake_minimum_required (VERSION 3.12)
project (seqan_tests_reduced_aminoacid CXX)
message (STATUS "Configuring tests/reduced_aminoacid")

# ----------------------------------------------------------------------------
# Dependencies
# ----------------------------------------------------------------------------

# Search SeqAn and select dependencies.
if (NOT "${SEQAN_BUILD_SYSTEM}" STREQUAL "DEVELOP")
    find_package (SeqAn REQUIRED)
endif ()

# ----------------------------------------------------------------------------
# Build Setup
# ----------------------------------------------------------------------------

# Add include directories.
include_directories (${SEQAN_INCLUDE_DIRS})

# Add definitions set by find_package (SeqAn).
add_definitions (${SEQAN_DEFINITIONS})

# Update the list of file names below if you add source files to your test.
add_executable (test_reduced_aminoacid test_reduced_aminoacid.cpp test_reduced_aminoacid.h)

# Add dependencies found by find_package (SeqAn).
target_link_libraries (test_reduced_aminoacid ${SEQAN_LIBRARIES})

# Add CXX flags found by find_package (SeqAn).
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_CXX_FLAGS}")

# ----------------------------------------------------------------------------
# Register with CTest
# ----------------------------------------------------------------------------

add_test (NAME test_reduced_aminoacid COMMAND $<TARGET_FILE:test_reduced_aminoacid>)
