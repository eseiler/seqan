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
// Author: Jochen Singer <jochen.singer@fu-berlin.de>
// ==========================================================================


#include <seqan/basic.h>
#include <seqan/stream.h>

#include "test_gff_io.h"


SEQAN_BEGIN_TESTSUITE(test_gff_io)
{
    // GFF tests
    SEQAN_CALL_TEST(test_store_io_read_record_context_gff);
    SEQAN_CALL_TEST(test_store_io_write_record_context_gff);
    SEQAN_CALL_TEST(test_store_io_comment_processing_context_gff);

    // GTF tests
    SEQAN_CALL_TEST(test_store_io_read_record_context_gtf);
    SEQAN_CALL_TEST(test_store_io_read_record_gtf_pseudogenes);
    SEQAN_CALL_TEST(test_store_io_write_record_context_gtf);

    // Tests for GffStream.
    SEQAN_CALL_TEST(test_store_io_gff_stream_read_record_gff);
    SEQAN_CALL_TEST(test_store_io_gff_stream_read_record_gtf);
    SEQAN_CALL_TEST(test_store_io_gff_stream_write_record_gff);
    SEQAN_CALL_TEST(test_store_io_gff_stream_write_record_gtf);

    // isOpen
    SEQAN_CALL_TEST(test_gff_io_isOpen_fileIn);
    SEQAN_CALL_TEST(test_gff_io_isOpen_fileOut);
}
SEQAN_END_TESTSUITE
