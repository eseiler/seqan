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

#ifndef SEQAN_HEADER_FIND_SHIFTOR_H
#define SEQAN_HEADER_FIND_SHIFTOR_H

namespace seqan2
{

//////////////////////////////////////////////////////////////////////////////
// ShiftOr
//////////////////////////////////////////////////////////////////////////////

/*!
 * @class ShiftOrPattern
 * @extends Pattern
 * @headerfile <seqan/find.h>
 * @brief Exact string matching using bit parallelism.
 *
 * The Shift-Or algorithm is applicable to search small patterns in texts using a small alphabet.
 *
 * @signature template <typename TNeedle>
 *            class Pattern<TNeedle, ShiftOr>;
 *
 * @tparam TNeedle The needle type. Types: @link String @endlink.
 *
 * The types of the needle and the haystack have to match.
 */

struct ShiftOr_;
typedef Tag<ShiftOr_> ShiftOr;

//////////////////////////////////////////////////////////////////////////////

template <typename TNeedle>
class Pattern<TNeedle, ShiftOr>
{
//____________________________________________________________________________
public:
    typedef uint_fast64_t TWord;
    enum { MACHINE_WORD_SIZE = sizeof(TWord) * 8 };

    Holder<TNeedle> data_host;
    String<TWord> bitMasks;            // Look up table for each character in the alphabet (called B in "Navarro")
    String<TWord> prefSufMatch;        // Set of all the prefixes of needle that match a suffix of haystack (called D in "Navarro")
    TWord needleLength;                // e.g., needleLength=33 --> blockCount=2 (iff w=32 bits)
    TWord blockCount;                // #unsigned ints required to store needle

//____________________________________________________________________________

    Pattern()
    {}

    // Custom c'tor setting a needle.
    template <typename TNeedle2>
    Pattern(TNeedle2 && ndl, SEQAN_CTOR_DISABLE_IF(IsSameType<typename std::remove_reference<TNeedle2>::type const &, Pattern const &>))
    {
        setHost(*this, std::forward<TNeedle2>(ndl));
        ignoreUnusedVariableWarning(dummy);
    }

//____________________________________________________________________________
};


//////////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////////

// TODO(rrahn): Change to rvalue reference for TNeedle2 as soon as all pattern store pointer to the needle.
template <typename TNeedle>
inline void
_reinitPattern(Pattern<TNeedle, ShiftOr> & me)
{
    typedef typename Pattern<TNeedle, ShiftOr>::TWord TWord;
    typedef typename Value<TNeedle>::Type TValue;

    TNeedle& ndl = needle(me);
    me.needleLength = length(ndl);
    if (me.needleLength < 1)
        me.blockCount = 1;
    else
        me.blockCount = (me.needleLength - 1) / BitsPerValue<TWord>::VALUE + 1;

    clear(me.bitMasks);
    resize(me.bitMasks, me.blockCount * ValueSize<TValue>::VALUE, ~static_cast<TWord>(0), Exact());

    for (TWord j = 0; j < me.needleLength; ++j)
        me.bitMasks[
            me.blockCount * ordValue(convert<TValue>(getValue(ndl, j)))
            + j / me.MACHINE_WORD_SIZE
        ] ^= static_cast<TWord>(1) << (j % me.MACHINE_WORD_SIZE);

//    setValue(me.data_host, needle);

    /*
    // Debug code
    std::cout << "Alphabet size: " << ValueSize<TValue>::VALUE << std::endl;
    std::cout << "Needle length: " << me.needleLength << std::endl;
    std::cout << "Block count: " << me.blockCount << std::endl;

    for(unsigned int i=0;i<ValueSize<TValue>::VALUE;++i) {
        if ((i<97) || (i>122)) continue;
        std::cout << static_cast<char>(i) << ": ";
        for(int j=0;j<me.blockCount;++j) {
            for(int bit_pos=0;bit_pos<BitsPerValue<unsigned int>::VALUE;++bit_pos) {
                std::cout << ((me.bitMasks[me.blockCount*i+j] & (1<<(bit_pos % BitsPerValue<unsigned int>::VALUE))) !=0);
            }
        }
        std::cout << std::endl;
    }
    */
}

//____________________________________________________________________________

template <typename TNeedle>
inline void
_patternInit (Pattern<TNeedle, ShiftOr> & me)
{
    typedef typename Pattern<TNeedle, ShiftOr>::TWord TWord;

    clear(me.prefSufMatch);
    resize(me.prefSufMatch, me.blockCount, ~static_cast<TWord>(0), Exact());
}

//____________________________________________________________________________

/*
template <typename TFinder, typename TNeedle>
bool _findShiftOrSmallNeedle(TFinder & finder, Pattern<TNeedle, ShiftOr> & me) {
    typedef unsigned int TWord;
    TWord compare= (~(1 << (me.needleLength-1)));
    while (!atEnd(finder)) {
        TWord pos = convert<TWord>(*finder);
        me.prefSufMatch[0] = (me.prefSufMatch[0] << 1) | me.bitMasks[me.blockCount*pos];
        if ((me.prefSufMatch[0] | compare) != (TWord) ~0) {
            finder-=(me.needleLength-1);
            return true;
        }
        goNext(finder);
    }
    return false;
}
*/
template <typename TFinder, typename TNeedle>
inline bool
_findShiftOrSmallNeedle(TFinder & finder, Pattern<TNeedle, ShiftOr> & me)
{
    typedef typename Haystack<TFinder>::Type THaystack;
    typedef typename Parameter_<THaystack>::Type TParamHaystack;
    typedef typename Value<TNeedle>::Type TValue;

    TParamHaystack hstk = haystack(finder);

    typedef typename Iterator<THaystack, Standard>::Type THaystackIterator;
    THaystackIterator hayit = iter(hstk, position(finder));
    THaystackIterator hayit_end = end(hstk, Standard());

    typedef typename Pattern<TNeedle, ShiftOr>::TWord TWord;
    TWord mask = static_cast<TWord>(1) << (me.needleLength - 1);
    TWord & pref_suf_match = me.prefSufMatch[0];

    for (; hayit < hayit_end; ++hayit)
    {
        pref_suf_match <<= 1;                //shift...
        pref_suf_match |= me.bitMasks[ordValue(convert<TValue>(getValue(hayit)))];    //...or

        if (pref_suf_match & mask)
            continue;

        //found a hit!
        //set finder to start position
        _setFinderEnd(finder, (hayit - begin(hstk, Standard())) + 1);
        setPosition(finder,  beginPosition(finder));
        return true;
    }
    return false;
}

template <typename TFinder, typename TNeedle>
inline bool
_findShiftOrLargeNeedle(TFinder & finder, Pattern<TNeedle, ShiftOr> & me)
{
    typedef typename Value<TNeedle>::Type TValue;
    typedef typename Pattern<TNeedle, ShiftOr>::TWord TWord;

    TWord compare = ~(static_cast<TWord>(1) << ((me.needleLength - 1) % BitsPerValue<TWord>::VALUE));
    while (!atEnd(finder))
    {
        TWord pos = ordValue(convert<TValue>(*finder));
        TWord carry = 0;
        for(TWord block = 0; block < me.blockCount; ++block)
        {
            bool newCarry = (me.prefSufMatch[block] & (static_cast<TWord>(1) << (BitsPerValue<TWord>::VALUE - 1))) != 0;
            me.prefSufMatch[block] <<= 1;
            me.prefSufMatch[block] |= carry;
            carry = newCarry;
        }
        for(TWord block = 0; block < me.blockCount; ++block)
            me.prefSufMatch[block] |= me.bitMasks[me.blockCount * pos + block];
        if ((me.prefSufMatch[me.blockCount - 1] | compare) != ~static_cast<TWord>(0))
        {
            _setFinderEnd(finder);
            finder -= me.needleLength - 1;
            return true;
        }

        /*
        // Debug code
        std::cout << "   ";
        for(int j=0;j<me.blockCount;++j) {
            for(int bit_pos=0;bit_pos<BitsPerValue<unsigned int>::VALUE;++bit_pos) {
                std::cout << ((me.prefSufMatch[j] & (1<<(bit_pos % BitsPerValue<unsigned int>::VALUE))) !=0);
            }
        }
        std::cout << std::endl;
        */
        goNext(finder);
    }
    return false;
}

template <typename TFinder, typename TNeedle>
inline bool
find(TFinder & finder, Pattern<TNeedle, ShiftOr> & me)
{
    if (empty(finder)) {
        _patternInit(me);
        _setFinderLength(finder, me.needleLength);
        _finderSetNonEmpty(finder);
    } else
        finder += me.needleLength;

    // Fast algorithm for needles < machine word?
    if (me.blockCount == 1) {
        return _findShiftOrSmallNeedle(finder, me);
    } else {
        return _findShiftOrLargeNeedle(finder, me);
    }
}

}// namespace seqan2

#endif //#ifndef SEQAN_HEADER_FIND_SHIFTOR_H
