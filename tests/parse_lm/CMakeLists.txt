# ===========================================================================
#                  SeqAn - The Library for Sequence Analysis
# ===========================================================================
# File: /tests/parse_lm/CMakeLists.txt
#
# CMakeLists.txt file for the parse_lm module tests.
# ===========================================================================

cmake_minimum_required (VERSION 3.12)
project (seqan_tests_parse_lm CXX)
message (STATUS "Configuring tests/parse_lm")

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
add_executable (test_parse_lm test_parse_lm.cpp test_parse_lm.h)

# Add dependencies found by find_package (SeqAn).
target_link_libraries (test_parse_lm ${SEQAN_LIBRARIES})

# Add CXX flags found by find_package (SeqAn).
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SEQAN_CXX_FLAGS}")

# ----------------------------------------------------------------------------
# Register with CTest
# ----------------------------------------------------------------------------

add_test (NAME test_test_parse_lm COMMAND $<TARGET_FILE:test_parse_lm>)
