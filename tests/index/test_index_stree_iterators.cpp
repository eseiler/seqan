// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2024, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================

#include <iostream>
#include <fstream>
#include <functional>
#include <typeinfo>

#define SEQAN_DEBUG
//#define SEQAN_TEST

#include <seqan/basic.h>
#include <seqan/align.h>
#include <seqan/find.h>
#include <seqan/stream.h>
#include <seqan/index.h>
#include <seqan/sequence.h>
#include <seqan/pipe.h>

#include "test_index_helpers.h"
#include "test_stree_iterators.h"

using namespace std;
using namespace seqan2;

SEQAN_BEGIN_TESTSUITE(test_index)
{
	SEQAN_CALL_TEST(testEmptyIndex);
	SEQAN_CALL_TEST(testIssue509);
	SEQAN_CALL_TEST(testIssue509b);
	SEQAN_CALL_TEST(goDownOnEmptyString);
	SEQAN_CALL_TEST(bidirectionalGoDownStringHistory);
	SEQAN_CALL_TEST(testSTreeIterators_Wotd);
	SEQAN_CALL_TEST(testSTreeIterators_WotdOriginal);
	SEQAN_CALL_TEST(testSTreeIterators_Esa);
    SEQAN_CALL_TEST(testTrieIterator);
    SEQAN_CALL_TEST(testRadixTreeIterator);
	SEQAN_CALL_TEST(testFind_Esa_Mlr);
	SEQAN_CALL_TEST(testCompareIndices_Esa_Wotd);
	SEQAN_CALL_TEST(testMultiIndex);
	SEQAN_CALL_TEST(testMUMs);
	SEQAN_CALL_TEST(testMaxRepeats);
	SEQAN_CALL_TEST(testSuperMaxRepeats);
	SEQAN_CALL_TEST(testSuperMaxRepeatsFast);
    SEQAN_CALL_TEST(testMultipleStrings_Ticket1109);
}
SEQAN_END_TESTSUITE
