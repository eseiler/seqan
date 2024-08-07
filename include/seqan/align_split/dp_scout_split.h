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
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================
// DPScout_ specialization for the split alignment implementation.
// ==========================================================================

#ifndef SEQAN_INCLUDE_SEQAN_ALIGN_SPLIT_DP_SCOUT_SPLIT_H_
#define SEQAN_INCLUDE_SEQAN_ALIGN_SPLIT_DP_SCOUT_SPLIT_H_

namespace seqan2 {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ----------------------------------------------------------------------------
// Tag SplitAlignmentScout.
// ----------------------------------------------------------------------------

struct SplitAlignmentScout_;
typedef Tag<SplitAlignmentScout_> SplitAlignmentScout;

// ----------------------------------------------------------------------------
// Class DPScoutState_
// ----------------------------------------------------------------------------

template <>
class DPScoutState_<SplitAlignmentScout>
{
public:
    // The best score for each column.  Initialized with 0.5*minValue<TScoreVal>().
    seqan2::String<int> splitScore;
    seqan2::String<int> splitPos;

    DPScoutState_()
    {}
};

// ----------------------------------------------------------------------------
// Class DPScout_
// ----------------------------------------------------------------------------

template <typename TDPCell>
class DPScout_<TDPCell, SplitAlignmentScout> : public DPScout_<TDPCell, Default>
{
public:
    typedef DPScout_<TDPCell, Default> TParent;

    DPScoutState_<SplitAlignmentScout> * state;

    DPScout_() : TParent(), state(0)
    {}

    DPScout_(DPScoutState_<SplitAlignmentScout> & state) : TParent(), state(&state)
    {}
};

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function _scoutBestScore()                                        [DPScout_]
// ----------------------------------------------------------------------------

template <typename TDPCell, typename TTraceMatrixNavigator, typename TIsLastColumn, typename TIsLastRow>
inline void
_scoutBestScore(DPScout_<TDPCell, SplitAlignmentScout> & dpScout,
                TDPCell const & activeCell,
                TTraceMatrixNavigator const & navigator,
                TIsLastColumn /*isLastColumn*/,
                TIsLastRow /*isLastRow*/ )
{
    //typedef typename Value<TDPCell>::Type TScoreValue;
    // Note that the underlying matrix has the coordinates flipped.  We use posH/posV as we would in pairwise alignments
    // and thus this is the reverse from the matrix representation.
    unsigned posH = coordinate(navigator, +DPMatrixDimension_::HORIZONTAL);
    // unsigned posV = coordinate(navigator, +DPMatrixDimension_::VERTICAL);

    int & i = dpScout.state->splitScore[posH];
    if (i < _scoreOfCell(activeCell))
    {
        i = _scoreOfCell(activeCell);
        dpScout.state->splitPos[posH] = position(navigator);
    }

    // TODO(rrahn): Do we need the optimal end-point?
    // We track only the last row for the best traceback score.
    if (TIsLastColumn::VALUE || TIsLastRow::VALUE)
    {
        typedef DPScout_<TDPCell, SplitAlignmentScout> TDPScout;
        typedef typename TDPScout::TParent TParent;
        _scoutBestScore(static_cast<TParent &>(dpScout), activeCell, navigator);
    }
}

}  // namespace seqan2

#endif  // #ifndef SEQAN_INCLUDE_SEQAN_ALIGN_SPLIT_DP_SCOUT_SPLIT_H_
