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
// Author: Hannes Hauswedell <hauswedell@mi.fu-berlin.de>
// ==========================================================================
// This file contains routines to extend an existing Align object
// ==========================================================================

#ifndef INCLUDE_SEQAN_ALIGN_DP_SCOUT_EXTEND_H_
#define INCLUDE_SEQAN_ALIGN_DP_SCOUT_EXTEND_H_

namespace seqan2 {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ----------------------------------------------------------------------------
// Tag XDropScout.
// ----------------------------------------------------------------------------

template <typename TScoreValue>
struct XDrop_
{
};

// ----------------------------------------------------------------------------
// Class DPScoutState_
// ----------------------------------------------------------------------------

template <typename TScoreValue>
class DPScoutState_<Terminator_<XDrop_<TScoreValue> > >
{
public:
    TScoreValue const terminationThreshold;
    TScoreValue columnMax;

    DPScoutState_() :
        terminationThreshold(std::numeric_limits<TScoreValue>::max()),
        columnMax(std::numeric_limits<TScoreValue>::min())
    {
    }

    DPScoutState_(TScoreValue const & _terminationThreshold) :
        terminationThreshold(_terminationThreshold),
        columnMax(std::numeric_limits<TScoreValue>::min())
    {
    }
};


// ============================================================================
// Metafunctions
// ============================================================================

// overrides for AligExtend XDrop case
template <typename TScoreValue>
struct HasTerminationCriterium_<AlignExtend_<XDrop_<TScoreValue> > > : True {};

template <typename TScoreValue, typename TSpec>
struct ScoutSpecForAlignmentAlgorithm_<AlignExtend_<XDrop_<TScoreValue> >, DPScoutState_<TSpec> >
{
    typedef Terminator_<XDrop_<TScoreValue> > Type;
};

template <typename TScoreValue, typename TSpec>
struct ScoutSpecForAlignmentAlgorithm_<AlignExtend_<XDrop_<TScoreValue> > const, DPScoutState_<TSpec> >
{
    typedef Terminator_<XDrop_<TScoreValue> > Type;
};

template <typename TDPCell>
struct ScoutStateSpecForScout_<
         DPScout_<
           TDPCell, Terminator_<XDrop_< typename Value<TDPCell>::Type> > > >
{
    typedef Terminator_<XDrop_<typename Value<TDPCell>::Type> > Type;
};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function _scoutBestScore()                                        [DPScout_]
// ----------------------------------------------------------------------------

// NOTE: The original code here used Value<TDPCell>::Type instead of TDPCellValue but this caused ambiguous call
// errors in MSVC.

template <typename TDPCell, typename TTraceMatrixNavigator, typename TDPCellValue, typename TIsLastColumn>
inline void
_scoutBestScore(DPScout_<TDPCell, Terminator_<XDrop_<TDPCellValue> > > & dpScout,
                TDPCell const & activeCell,
                TTraceMatrixNavigator const & navigator,
                TIsLastColumn const & /**/,
                False const & /*IsLastRow*/)
{
    typedef typename Value<TDPCell>::Type TScoreValue;
    typedef XDrop_<TScoreValue> TXDrop;
    typedef DPScout_<TDPCell, Terminator_<TXDrop> > TDPScout;
    typedef typename TDPScout::TBase TParent;

    // global maximum
    _scoutBestScore(static_cast<TParent &>( dpScout ), activeCell, navigator);

    // column maximum
    dpScout.state->columnMax = _max(dpScout.state->columnMax, _scoreOfCell(activeCell));
}

template <typename TDPCell, typename TTraceMatrixNavigator, typename TDPCellValue, typename TIsLastColumn>
inline void
_scoutBestScore(DPScout_<TDPCell, Terminator_<XDrop_<TDPCellValue> > > & dpScout,
                TDPCell const & activeCell,
                TTraceMatrixNavigator const & navigator,
                TIsLastColumn const & /**/,
                True const & /*IsLastRow*/)
{
    typedef typename Value<TDPCell>::Type TScoreValue;

    _scoutBestScore(dpScout, activeCell, navigator, TIsLastColumn(), False());

    // check termination condition
    if (_scoreOfCell(dpScout._maxScore) - dpScout.state->columnMax >= dpScout.state->terminationThreshold)
        terminateScout(dpScout);
    else // reset columMax at end of column
        dpScout.state->columnMax = std::numeric_limits<TScoreValue>::min();
}

// ----------------------------------------------------------------------------
// Function _scoutBestScore()                                        [DPScout_]
// ----------------------------------------------------------------------------

// Computes the score and tracks it if enabled.
template <typename TDPScout, typename TTraceMatrixNavigator,
          typename TDPCell,
          typename TScoreValue, typename TGapCosts,
          typename TSequenceHValue, typename TSequenceVValue, typename TScoringScheme, typename TColumnDescriptor,
          typename TCellDescriptor, typename TTraceback>
inline void
_computeCell(TDPScout & scout,
             TTraceMatrixNavigator & traceMatrixNavigator,
             TDPCell & current,
             TDPCell & diagonal,
             TDPCell const & horizontal,
             TDPCell & vertical,
             TSequenceHValue const & seqHVal,
             TSequenceVValue const & seqVVal,
             TScoringScheme const & scoringScheme,
             TColumnDescriptor const &,
             TCellDescriptor const &,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             // One of FirstCell, InnerCell or LastCell.
             DPProfile_<AlignExtend_<XDrop_<TScoreValue> >, TGapCosts,
                        TTraceback> const &)
{
    typedef DPProfile_<AlignExtend_<XDrop_<TScoreValue> >, TGapCosts, TTraceback> TDPProfile;
    typedef DPMetaColumn_<TDPProfile, TColumnDescriptor> TMetaColumn;

    assignValue(traceMatrixNavigator, _computeScore(current, diagonal, horizontal, vertical,
                                                    seqHVal, seqVVal, scoringScheme,
                                                    typename RecursionDirection_<TMetaColumn, TCellDescriptor>::Type(),
                                                    TDPProfile()));
    if (TrackingEnabled_<TMetaColumn, TCellDescriptor>::VALUE)
    {
        typedef typename IsSameType< typename TColumnDescriptor::TColumnProperty, DPFinalColumn>::Type TIsLastColumn;

        // the following is the only change to the regular _computeCell:
        // for the evaluation of the termination criterium we treat
        // all lastCells as lastRows
        _setVerticalScoreOfCell(current, _verticalScoreOfCell(vertical));
        typedef typename IsSameType<TCellDescriptor, LastCell>::Type TIsLastRow;
        _scoutBestScore(scout, current, traceMatrixNavigator, TIsLastColumn(), TIsLastRow());
    }
}

}  // namespace seqan2

#endif  // #ifndef INCLUDE_SEQAN_ALIGN_DP_SCOUT_EXTEND_H_
