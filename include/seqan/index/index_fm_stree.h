// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2024, Knut Reinert, FU Berlin
// Copyright (c) 2013 NVIDIA Corporation
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
// Author: Enrico Siragusa <enrico.siragusa@fu-berlin.de>
// ==========================================================================

//SEQAN_NO_DDDOC:do not generate documentation for this file

#ifndef INDEX_FM_STREE_H_
#define INDEX_FM_STREE_H_

namespace seqan2 {

// ==========================================================================
// Classes
// ==========================================================================

// ----------------------------------------------------------------------------
// Class HistoryStackFM_
// ----------------------------------------------------------------------------

template <typename TSize, typename TAlphabet>
struct HistoryStackFM_
{
    Pair<TSize> range;
    TSize       repLen;
    TAlphabet   lastChar;

    HistoryStackFM_() {}

    template <typename TSize_, typename TAlphabet_>

    HistoryStackFM_(Pair<TSize_> const &_range, TSize_ _repLen, TAlphabet_ _lastChar):
        range(_range),
        repLen(_repLen),
        lastChar(_lastChar)
    {}

    HistoryStackFM_(HistoryStackFM_ const &) = default;

    HistoryStackFM_ const &
    operator=(HistoryStackFM_ const & _origin)
    {
        range = _origin.range;
        repLen = _origin.repLen;
        lastChar = _origin.lastChar;
        return _origin;
    }
};

// ----------------------------------------------------------------------------
// Class VertexFM
// ----------------------------------------------------------------------------

template <typename TSize, typename TAlphabet>
struct VertexFM
{
    Pair<TSize> range;
    TSize       smaller;
    TSize       repLen;
    TAlphabet   lastChar;

    VertexFM() :
        range(0, 0),
        smaller(0),
        repLen(0),
        lastChar(0)
    {}

    VertexFM(MinimalCtor) :
        range(0, 0),
        smaller(0),
        repLen(0),
        lastChar(0)
    {}

    VertexFM(Pair<TSize> newCurrentRange, TSize newSmallerValue, TSize newRepLen, TAlphabet newChar) :
        range(newCurrentRange),
        smaller(newSmallerValue),
        repLen(newRepLen),
        lastChar(newChar)
    {}

    VertexFM(VertexFM const & other) :
        range(other.range),
        smaller(other.smaller),
        repLen(other.repLen),
        lastChar(other.lastChar)
    {}

    inline
    VertexFM &
    operator = (VertexFM const & _origin)
    {
        range = _origin.range;
        smaller = _origin.smaller;
        repLen = _origin.repLen;
        lastChar = _origin.lastChar;
        return *this;
    }
};

// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction VertexDescriptor                                        [Index]
// ----------------------------------------------------------------------------

template < typename TText, typename TOccSpec, typename TIndexSpec>
struct VertexDescriptor<Index<TText, FMIndex<TOccSpec, TIndexSpec> > >
{
    typedef Index<TText,FMIndex<TOccSpec, TIndexSpec> >     TIndex_;
    typedef typename Size<TIndex_>::Type                    TSize_;
    typedef typename Value<TIndex_>::Type                   TAlphabet_;

    typedef VertexFM<TSize_, TAlphabet_>                    Type;
};

// ----------------------------------------------------------------------------
// Metafunction HistoryStackEntry_                                      [Index]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TSpec, typename TIterSpec>
struct HistoryStackEntry_<Iter<Index<TText, FMIndex<TOccSpec, TSpec> >,
                               VSTree< TopDown< ParentLinks<TIterSpec> > > > >
{
    typedef HistoryStackFM_<typename Size<Index<TText, FMIndex<TOccSpec, TSpec> > >::Type,
                             typename Value<Index<TText, FMIndex<TOccSpec, TSpec> > >::Type>    Type;
};

// ----------------------------------------------------------------------------
// Metafunction EdgeLabel                                            [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
struct EdgeLabel<Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TSpec> > >
{
    typedef typename Value<Index<TText, FMIndex<TOccSpec, TIndexSpec> > >::Type Type;
};


// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function _indexRequireTopDownIteration()                             [Index]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec>
inline void _indexRequireTopDownIteration(Index<TText, FMIndex<TOccSpec, TIndexSpec> > & index)
{
    indexRequire(index, FibreSALF());
}

// ----------------------------------------------------------------------------
// Function begin()                                                  [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline
typename Iterator<Index<TText,FMIndex<TOccSpec, TIndexSpec> >, TSpec>::Type
begin(Index<TText, FMIndex<TOccSpec, TIndexSpec> > & index, TSpec const /*Tag*/)
{
    typedef typename Iterator<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, TSpec>::Type TIter;

    return TIter(index);
}

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline
typename Iterator<Index<TText,FMIndex<TOccSpec, TIndexSpec> > const, TSpec>::Type
begin(Index<TText, FMIndex<TOccSpec, TIndexSpec> > const & index, TSpec const /*Tag*/)
{
    typedef typename Iterator<Index<TText, FMIndex<TOccSpec, TIndexSpec> > const, TSpec>::Type TIter;

    return TIter(index);
}

// ----------------------------------------------------------------------------
// Function _isRoot()                                                [Iterator]
// ----------------------------------------------------------------------------

template <typename TAlphabet, typename TSize>
inline bool _isRoot(VertexFM<TAlphabet, TSize> const & value)
{
    return _isSizeInval(value.range.i2);
}

// ----------------------------------------------------------------------------
// Function _isLeaf()                                                [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TDfsOrder>
inline bool
_isLeaf(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TSpec> > const & it,
        VSTreeIteratorTraits<TDfsOrder, False> const)
{
    return value(it).range.i1 >= value(it).range.i2;
}

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TDfsOrder>
inline bool
_isLeaf(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TSpec> > const & it,
        VSTreeIteratorTraits<TDfsOrder, True> const)
{
    return _isLeaf(it, VSTreeIteratorTraits<TDfsOrder, False>()) &&
           isSentinel(indexLF(container(it)), value(it).range.i1);
}

// ----------------------------------------------------------------------------
// Function _getNodeByChar()                                         [Iterator]
// ----------------------------------------------------------------------------

#ifndef YARA_MAPPER
template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TSize, typename TChar>
inline bool
_getNodeByChar(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<TSpec> > > const & it,
               typename VertexDescriptor<Index<TText, FMIndex<TOccSpec, TIndexSpec> > >::Type const & vDesc,
               Pair<typename Size<Index<TText, FMIndex<TOccSpec, TIndexSpec> > >::Type> & _range,
               TSize & smaller,
               TChar c)
{
    typedef Index<TText, FMIndex<TOccSpec, TIndexSpec/*FMIndexConfig<TOccSpec, TLengthSum> */> >        TIndex;
#else
template <typename TText, typename TOccSpec, typename TLengthSum, typename TSpec, typename TSize, typename TChar>
inline bool
_getNodeByChar(Iter<Index<TText, FMIndex<TOccSpec, FMIndexConfig<TOccSpec, TLengthSum> > >, VSTree<TopDown<TSpec> > > const & it,
               typename VertexDescriptor<Index<TText, FMIndex<TOccSpec, FMIndexConfig<TOccSpec, TLengthSum> > > >::Type const & vDesc,
               Pair<typename Size<Index<TText, FMIndex<TOccSpec, FMIndexConfig<TOccSpec, TLengthSum> > > >::Type> & _range,
               TSize & smaller,
               TChar c)
{
    typedef Index<TText, FMIndex<TOccSpec, FMIndexConfig<TOccSpec, TLengthSum> > >        TIndex;
#endif

    typedef typename Fibre<TIndex, FibreLF>::Type               TLF;

    TIndex const & index = container(it);
    TLF const & lf = indexLF(index);

    _range = range(index, vDesc);

    TSize _smaller;
    _range.i1 = lf(_range.i1, c, _smaller);
    _range.i2 = lf(_range.i2, c, smaller);
    smaller -= _smaller;

    return _range.i1 < _range.i2;
}

// ----------------------------------------------------------------------------
// Function _goDownChar()                                            [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TChar>
inline bool
_goDownChar(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<TSpec> > > & it, TChar c)
{
    typedef Index<TText, FMIndex<TOccSpec, TIndexSpec> >    TIndex;
    typedef typename Size<TIndex>::Type                     TSize;
    typedef Pair<TSize>                                     TRange;

    TRange _range;
    TSize _smaller = 0;

    if (_getNodeByChar(it, value(it), _range, _smaller, c))
    {
        _historyPush(it);

        value(it).range = _range;
        value(it).smaller = _smaller;
        value(it).lastChar = c;
        value(it).repLen++;

        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
// Function _goDown()                                                [Iterator]
// ----------------------------------------------------------------------------

// TODO(esiragusa): Implement this.
template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TDfsOrder>
inline bool
_goDown(Iter<Index<TText, FMIndex<TOccSpec,TIndexSpec> >, VSTree<TopDown<TSpec> > > & it,
        VSTreeIteratorTraits<TDfsOrder, False> const)
{
    return _goDown(it, VSTreeIteratorTraits<TDfsOrder, True>());
}

template <typename TText, typename TOccSpec, typename TSpec, typename TIndexSpec, typename TDfsOrder>
inline bool
_goDown(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<TSpec> > > & it,
        VSTreeIteratorTraits<TDfsOrder, True> const)
{
    typedef Index<TText, FMIndex<TOccSpec, TIndexSpec> >    TIndex;
    typedef typename Value<TIndex>::Type                    TAlphabet;
//    typedef typename ValueSize<TAlphabet>::Type             TAlphabetSize;

    // NOTE(esiragusa): this should be faster only for texts over small alphabets consisting of few/long sequences.
    if (isLeaf(it)) return false;

    // TODO(esiragusa): Fix increment for alphabets with qualities.
//    for (TAlphabetSize c = 0; c < ValueSize<TAlphabet>::VALUE; ++c)
    for (TAlphabet c = 0; ordValue(c) < ValueSize<TAlphabet>::VALUE; ++c)
        if (_goDownChar(it, c)) return true;

    return false;
}

// ----------------------------------------------------------------------------
// Function _goDownString()                                          [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TString, typename TSize>
inline bool
_goDownString(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<TSpec> > > &it,
              TString const & string,
              TSize & lcp)
{
    typedef Index<TText, FMIndex<TOccSpec, TIndexSpec> >        TIndex;
    typedef typename Size<TIndex>::Type                         TSize2;
    typedef Pair<TSize2>                                        TRange;
    typedef typename Iterator<TString const, Standard>::Type    TStringIter;

    TStringIter stringIt = begin(string, Standard());
    TStringIter stringEnd = end(string, Standard());

    if (SEQAN_UNLIKELY(stringIt == stringEnd))
        return true;

    _historyPush(it);

    for (lcp = 0; stringIt != stringEnd; ++stringIt, ++lcp)
    {
        TRange _range;
        TSize2 _smaller = 0;

        // NOTE(esiragusa): this should be faster only for texts over small alphabets consisting of few/long sequences.
        if (isLeaf(it) || !_getNodeByChar(it, value(it), _range, _smaller, value(stringIt))) break;

        value(it).range = _range;
        value(it).smaller = _smaller;
    }

    value(it).repLen += lcp;

    if (lcp) value(it).lastChar = value(stringIt - 1);

    return stringIt == stringEnd;
}

// ----------------------------------------------------------------------------
// Function _goRight()                                               [Iterator]
// ----------------------------------------------------------------------------

// TODO(esiragusa): Implement this.
template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TDfsOrder>
inline bool
_goRight(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<TSpec> > > & it,
         VSTreeIteratorTraits<TDfsOrder, False> const)
{
    return _goRight(it, VSTreeIteratorTraits<TDfsOrder, True>());
}

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec, typename TDfsOrder>
inline bool
_goRight(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<TSpec> > > & it,
         VSTreeIteratorTraits<TDfsOrder, True> const)
{
    typedef Index<TText, FMIndex<TOccSpec, TIndexSpec> >    TIndex;
    typedef typename Value<TIndex>::Type                    TAlphabet;
    typedef typename Size<TIndex>::Type                     TSize;
    typedef Pair<TSize>                                     TRange;

    typedef typename VertexDescriptor<TIndex>::Type         TVertexDescriptor;

    if (isRoot(it)) return false;

    TVertexDescriptor parentDesc = nodeUp(it);
    TRange _range;
    TSize _smaller = 0;

    // TODO(esiragusa): Fix increment for alphabets with qualities.
//    for (TAlphabetSize c = ordValue(value(it).lastChar) + 1; c < ValueSize<TAlphabet>::VALUE; ++c)
    for (value(it).lastChar++; ordValue(value(it).lastChar) < ValueSize<TAlphabet>::VALUE; value(it).lastChar++)
    {
        if (_getNodeByChar(it, parentDesc, _range, _smaller, value(it).lastChar))
        {
            value(it).range = _range;
            value(it).smaller = _smaller;

            return true;
        }
    }

    return false;
}

// ----------------------------------------------------------------------------
// Function _goUp()                                                  [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline bool
_goUp(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<TSpec> > > & it)
{
    if (isRoot(it)) return false;

    _historyPop(it);
    return true;
}

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline bool
_goUp(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree<TopDown<ParentLinks<TSpec> > > > & it)
{
    if (isRoot(it)) return false;

    _historyPop(it);
    return true;
}

// ----------------------------------------------------------------------------
// Function nodeUp()                                                 [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, class TSpec>
inline typename VertexDescriptor<Index<TText, FMIndex<TOccSpec, TIndexSpec> > >::Type
nodeUp(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec> >, VSTree< TopDown< ParentLinks<TSpec> > > > const & it)
{
    typedef Index<TText, FMIndex<TOccSpec, TIndexSpec> >    TIndex;
    typedef typename VertexDescriptor<TIndex>::Type         TVertexDescriptor;

    if (!empty(it.history))
        return TVertexDescriptor(back(it.history).range, 0, back(it.history).repLen, back(it.history).lastChar);
    else
        return value(it);
}

// ----------------------------------------------------------------------------
// Function _historyPush()                                           [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline void
_historyPush(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<TSpec> > > & it)
{
    it._parentDesc = value(it);
}

template < typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline void
_historyPush(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<ParentLinks<TSpec> > > > & it)
{
    typedef Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<ParentLinks<TSpec> > > > TIter;

    typename HistoryStackEntry_<TIter>::Type h;

    h.range = value(it).range;
    h.repLen = value(it).repLen;
    h.lastChar = value(it).lastChar;

    appendValue(it.history, h);
}

// ----------------------------------------------------------------------------
// Function _historyPop()                                            [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline void
_historyPop(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<TSpec> > > & it)
{
    value(it).range = back(it.history).range;
    value(it).repLen = back(it.history).repLen;
    value(it).lastChar = back(it.history).lastChar;
}

template < typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline void
_historyPop(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<ParentLinks<TSpec> > > > & it)
{
    value(it).range = back(it.history).range;
    value(it).repLen = back(it.history).repLen;
    value(it).lastChar = back(it.history).lastChar;
    eraseBack(it.history);
}

// ----------------------------------------------------------------------------
// Function repLength()                                              [Iterator]
// ----------------------------------------------------------------------------

template <typename TIndex, typename TAlphabet, typename TSize>
inline typename Size<TIndex>::Type
repLength(TIndex const &, VertexFM<TAlphabet, TSize> const & vDesc)
{
    return vDesc.repLen;
}

// ----------------------------------------------------------------------------
// Function parentEdgeLabel()                                        [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline typename EdgeLabel<Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<TSpec> > > >::Type
parentEdgeLabel(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<TSpec> > > const & it)
{
    return value(it).lastChar;
}

// ----------------------------------------------------------------------------
// Function parentEdgeFirstChar()                                    [Iterator]
// ----------------------------------------------------------------------------

template <typename TText, typename TOccSpec, typename TIndexSpec, typename TSpec>
inline typename Value<Index<TText, FMIndex<TOccSpec, TIndexSpec > > >::Type
parentEdgeFirstChar(Iter<Index<TText, FMIndex<TOccSpec, TIndexSpec > >, VSTree<TopDown<TSpec> > > const & it)
{
    return value(it).lastChar;
}

}
#endif  // INDEX_FM_STREE_H_
