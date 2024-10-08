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
// Author: Andreas Gogol-Doering <andreas.doering@mdc-berlin.de>
// ==========================================================================
// Edit distance score class and supporting code.
// ==========================================================================

#ifndef SEQAN_SSCORE_EDIT_H_
#define SEQAN_SSCORE_EDIT_H_

namespace seqan2 {

/*!
 * @class EditDistanceScore
 * @extends Score
 * @headerfile <seqan/score.h>
 * @brief Edit distance scoring scheme.
 *
 * <tt>EditDistance</tt> is a synonym for <tt>LevenshteinDistance</tt>.
 *
 * @signature template <typename TValue>
 *            class Score<TValue, EditDistance>;
 *
 * @tparam TValue The score value type to use.
 */

// TODO(holtgrew): Should EditDistance better live here instead of basic_tag.h?
// EditDistance is defined in basic_tag.h
template <typename TValue>
class Score<TValue, EditDistance> {
public:
    Score() {}
};

// TODO(holtgrew): Remove this?

/*!
 * @typedef EditDistanceScoreTypedef EditDistance
 * @headerfile <seqan/score.h>
 * @brief Edit distance scoring scheme shortcut.
 *
 * @signature typedef Score<int, EditDistance> EditDistanceScore;
 */

typedef Score<int, EditDistance> EditDistanceScore;

template <typename TValue>
inline TValue
scoreMatch(Score<TValue, EditDistance> &) {
    return 0;
}


template <typename TValue>
inline TValue
scoreMatch(Score<TValue, EditDistance> const &) {
    return 0;
}


template <typename TValue>
inline TValue
scoreMismatch(Score<TValue, EditDistance> &) {
    return -1;
}


template <typename TValue>
inline TValue
scoreMismatch(Score<TValue, EditDistance> const &) {
    return -1;
}


template <typename TValue>
inline TValue
scoreGapExtend(Score<TValue, EditDistance> &) {
    return -1;
}


template <typename TValue>
inline TValue
scoreGapExtend(Score<TValue, EditDistance> const &) {
    return -1;
}


template <typename TValue>
inline TValue
scoreGapOpen(Score<TValue, EditDistance> &) {
    return -1;
}


template <typename TValue>
inline TValue
scoreGapOpen(Score<TValue, EditDistance> const &) {
    return -1;
}

}  // namespace seqan2

#endif  // SEQAN_SSCORE_EDIT_H_
