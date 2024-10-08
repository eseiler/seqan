// ==========================================================================
//                                  parse_lm
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
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================

// SEQAN_NO_GENERATED_FORWARDS

#ifndef INCLUDE_SEQAN_PARSE_LM_PARSE_BLASTN_TABULAR_H_
#define INCLUDE_SEQAN_PARSE_LM_PARSE_BLASTN_TABULAR_H_

namespace seqan2 {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

struct BlastnTabular_;
typedef Tag<BlastnTabular_> BlastnTabular;

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function readRecord()
// ----------------------------------------------------------------------------

template <typename TLocalMatchStore, typename TForwardIter>
inline void
readRecord(TLocalMatchStore & store,
           TForwardIter & iter,
           BlastnTabular const & /*tag*/)
{
    typedef typename TLocalMatchStore::TPosition TPosition;

    if (atEnd(iter))
    {
        throw ParseError("Unexpected end of file!");
        return;
    }

    // Skip any comments.
    while (value(iter) == '#') {
        skipLine(iter);
        if (atEnd(iter))
        {
            throw ParseError("Unexpected end of file!");
            return;
        }
    }

    SEQAN_ASSERT_NEQ(value(iter), '#');

    // Read line.
    CharString buffer;

    CharString subjectName;
    CharString queryName;
    TPosition subjectBeginPos = 0;
    TPosition subjectEndPos = 0;
    TPosition queryBeginPos = 0;
    TPosition queryEndPos = 0;

    // Field: query id
    readUntil(queryName, iter, IsTab());

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: subject id
    readUntil(subjectName, iter, IsTab());

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: % identity
    skipUntil(iter, IsTab());

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: alignment length
    skipUntil(iter, IsTab());

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: mismatches
    skipUntil(iter, IsTab());

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: gap opens
    skipUntil(iter, IsTab());

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: q. start
    clear(buffer);
    readUntil(buffer, iter, NotFunctor<IsDigit>());
    queryBeginPos = lexicalCast<TPosition>(buffer) - 1;

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: q. end
    clear(buffer);
    readUntil(buffer, iter, NotFunctor<IsDigit>());
    queryEndPos = lexicalCast<TPosition>(buffer);

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: s. start
    clear(buffer);
    readUntil(buffer, iter, NotFunctor<IsDigit>());
    subjectBeginPos = lexicalCast<TPosition>(buffer) - 1;

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: s. end
    clear(buffer);
    readUntil(buffer, iter, NotFunctor<IsDigit>());
    subjectEndPos = lexicalCast<TPosition>(buffer);

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: evalue
    skipUntil(iter, IsTab());

    // Skip TAB.
    skipOne(iter, IsTab());

    // Field: bit score
    skipUntil(iter, IsWhitespace());

    // Finally, append the local match.
    appendLocalMatch(store, subjectName, subjectBeginPos, subjectEndPos, queryName, queryBeginPos, queryEndPos);
}

}  // namespace seqan2

#endif  // INCLUDE_SEQAN_PARSE_LM_PARSE_BLASTN_TABULAR_H_
