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
// The Align class provides tabular alignments.  It contains multiple
// Gap objects, one for each row of the alignment.
// ==========================================================================

#ifndef SEQAN_INCLUDE_SEQAN_ALIGN_ALIGN_BASE_H_
#define SEQAN_INCLUDE_SEQAN_ALIGN_ALIGN_BASE_H_

namespace seqan2 {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ----------------------------------------------------------------------------
// Class Align
// ----------------------------------------------------------------------------

/*!
 * @class Align
 * @implements EqualityComparableConcept
 * @headerfile <seqan/align.h>
 * @brief Tabular alignment of same-type sequences.
 *
 * @signature template <typename TSequence, typename TGapSpec>
 *            class Align;
 *
 * @tparam TSequence Type of the underlying sequence.
 * @tparam TGapSpec  Tag for selecting the @link Gaps @endlink specialization.
 *
 * The Alignment class is for storing tabular alignments of sequences having the same type.  They do so by being a
 * container of @link Gaps @endlink objects.  The most common use case is storing pairwise alignments that are
 * generated by one of the dynamic programming alignment algorithms.
 *
 * @section Example
 *
 * Here is an example of using an Align object with @link globalAlignment @endlink.
 *
 * @include demos/dox/align/align.cpp
 *
 * The output is as follows:
 *
 * @include demos/dox/align/align.cpp.stdout
 *
 * @see globalAlignment
 * @see localAlignment
 * @see Gaps
 */

template <typename TSource, typename TSpec = ArrayGaps>
class Align
{
public:
    typedef Gaps<TSource, TSpec> TGaps;
    typedef String<TGaps> TRows;
    typedef typename Size<TRows>::Type TRowsSize;

    TRows data_rows;

    Align() {}

    template <typename TString, typename TStringsetSpec>
    Align(StringSet<TString, TStringsetSpec> & stringset)
    {
        setStrings(*this, stringset);
    }

    Align(Align const &) = default;

    Align & operator=(Align const & other) = default;
};

// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction Cols
// ----------------------------------------------------------------------------

/*!
 * @mfn Align#Cols
 * @brief Return the type representing a column in an Align object.
 *
 * @signature Cols<TAlign>::Type
 *
 * @tparam TAlign The Align object to get the column type for.
 *
 * @return Type   The resulting column type.
 */

template <typename TSource, typename TSpec>
struct Cols<Align<TSource, TSpec> >
{
    typedef AlignCols<Align<TSource, TSpec> > Type;
};

template <typename TSource, typename TSpec>
struct Cols<Align<TSource, TSpec> const>
{
    typedef AlignCols<Align<TSource, TSpec> const> Type;
};

// ----------------------------------------------------------------------------
// Metafunction Value
// ----------------------------------------------------------------------------

/*!
 * @mfn Align#Value
 * @brief Return the value type for an Align object.
 *
 * @signature Value<TAlign>::Type
 *
 * @tparam TAlign The Align object to get the value type for.
 *
 * @return Type   The resulting value type.
 */

template <typename TSource, typename TSpec>
struct Value<Align<TSource, TSpec> >:
    Value<Gaps<TSource, TSpec> >
{};
template <typename TSource, typename TSpec>
struct Value<Align<TSource, TSpec> const>:
    Value<Gaps<TSource, TSpec> const>
{};

// ----------------------------------------------------------------------------
// Metafunction GetValue
// ----------------------------------------------------------------------------

/*!
 * @mfn Align#GetValue
 * @brief Return the get-value type for an Align object.
 *
 * @signature GetValue<TAlign>::Type
 *
 * @tparam TAlign The Align object to get the get-value type for.
 *
 * @return Type   The resulting get-value type.
 */

template <typename TSource, typename TSpec>
struct GetValue<Align<TSource, TSpec> >:
    GetValue<Gaps<TSource, TSpec> >
{};
template <typename TSource, typename TSpec>
struct GetValue<Align<TSource, TSpec> const>:
    GetValue<Gaps<TSource, TSpec> const>
{};

// ----------------------------------------------------------------------------
// Metafunction Reference
// ----------------------------------------------------------------------------

/*!
 * @mfn Align#Reference
 * @brief Return the reference type for an Align object.
 *
 * @signature GetValue<TAlign>::Type
 *
 * @tparam TAlign The Align object to get the reference type for.
 *
 * @return Type   The resulting reference type.
 */

template <typename TSource, typename TSpec>
struct Reference<Align<TSource, TSpec> >:
    Reference<Gaps<TSource, TSpec> >
{};
template <typename TSource, typename TSpec>
struct Reference<Align<TSource, TSpec> const>:
    Reference<Gaps<TSource, TSpec> const>
{};

// ----------------------------------------------------------------------------
// Metafunction Rows
// ----------------------------------------------------------------------------

/*!
 * @mfn Align#Row
 * @brief Return the row type (@link Gaps @endlink specialization).
 *
 * @signature Row<TAlign>::Type
 *
 * @tparam TAlign The Align object to get the row type for.
 *
 * @return Type   The resulting row type.
 */

/*!
 * @mfn Align#Rows
 * @brief Return the type used for rows in an Align object (a Gaps specialization).
 *
 * @signature Rows<TAlign>::Type
 *
 * @tparam TAlign The Align object to get the rows type for.
 *
 * @return Type   The resulting rows type.
 */

template <typename TSource, typename TSpec>
struct Rows<Align<TSource, TSpec> >
{
    typedef String<Gaps<TSource, TSpec> > Type;
};
template <typename TSource, typename TSpec>
struct Rows<Align<TSource, TSpec> const>
{
    typedef String<Gaps<TSource, TSpec> > const Type;
};

// ----------------------------------------------------------------------------
// Metafunction Source
// ----------------------------------------------------------------------------

/*!
 * @mfn Align#Source
 * @brief Return the type of the underlying sequence.
 *
 * @signature Rows<TAlign>::Type;
 *
 * @tparam TAlign The Align object to get the underlying sequence type for.
 *
 * @return Type   The resulting sequence type.
 */

template <typename TSource, typename TSpec>
struct Source<Align<TSource, TSpec> >
{
    typedef TSource Type;
};
template <typename TSource, typename TSpec>
struct Source<Align<TSource, TSpec> const>
{
    typedef TSource Type;
};

// ----------------------------------------------------------------------------
// Metafunction StringSetType
// ----------------------------------------------------------------------------

/*!
 * @mfn Align#StringSetType
 * @brief Return the type that would be used for a string set of the sources.
 *
 * @signature Rows<TAlign>::Type
 *
 * @tparam TAlign The Align object to get the string set type for.
 *
 * @return Type   The resulting string set type.
 */

template <typename TSource, typename TSpec>
struct StringSetType<Align<TSource, TSpec> >
{
    typedef StringSet<TSource, Dependent<> > Type;
};

template <typename TSource, typename TSpec>
struct StringSetType<Align<TSource, TSpec> const>
{
    typedef StringSet<TSource, Dependent<> > Type;
};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function move()
// ----------------------------------------------------------------------------

template <typename TSource, typename TSpec>
inline
void move(Align<TSource, TSpec> & target, Align<TSource, TSpec> & source)
{
    move(target.data_rows, source.data_rows);
}

// ----------------------------------------------------------------------------
// Function rows()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#rows
 * @brief Returns the rows of an Align object.
 *
 * @signature TRows rows(align);
 *
 * @param[in] align The Align object to get the rows for.
 *
 * @return TRows A container with the Gaps of the Align object.
 */

template <typename TSource, typename TSpec>
inline typename Rows<Align<TSource, TSpec> >::Type &
rows(Align<TSource, TSpec> & me)
{
    return me.data_rows;
}

template <typename TSource, typename TSpec>
inline typename Rows<Align<TSource, TSpec> const>::Type &
rows(Align<TSource, TSpec> const & me)
{
    return me.data_rows;
}

// ----------------------------------------------------------------------------
// Function row()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#row
 * @brief Returns a single row of an Align object.
 *
 * @signature TRow row(align, pos);
 *
 * @param[in] align The Align object to get the row of.
 * @param[in] pos   The number of the row to get.
 *
 * @return TRow Reference to the given row of align  (Metafunction: @link Align#Row @endlink).
 */

template <typename TSource, typename TSpec, typename TPosition>
inline typename Row<Align<TSource, TSpec> >::Type &
row(Align<TSource, TSpec> & me,
    TPosition _pos)
{
    return value(rows(me), _pos);
}

template <typename TSource, typename TSpec, typename TPosition>
inline typename Row<Align<TSource, TSpec> const>::Type &
row(Align<TSource, TSpec> const & me,
    TPosition _pos)
{
    return value(rows(me), _pos);
}

// ----------------------------------------------------------------------------
// Function cols()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#cols
 * @brief Returns the columns of an Align object.
 *
 * @signature TCols cols(align);
 *
 * @param[in] align The Align object to get the cols of.
 *
 * @return TCols The columns of the Align object (Metafunction: @link Align#Cols @endlink).
 */

template <typename TSource, typename TSpec>
inline typename Cols<Align<TSource, TSpec> >::Type
cols(Align<TSource, TSpec> & me)
{
    return typename Cols<Align<TSource, TSpec> >::Type(me);
}

template <typename TSource, typename TSpec>
inline typename Cols<Align<TSource, TSpec> const>::Type
cols(Align<TSource, TSpec> const & me)
{
    return typename Cols<Align<TSource, TSpec> const>::Type(me);
}

// ----------------------------------------------------------------------------
// Function col()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#col
 * @brief Returns the columns of an Align object.
 *
 * @signature TCol col(align);
 *
 * @param[in] align The Align object to get the cols of.
 *
 * @return TCol The column of the Align object (Metafunction: @link Align#Col @endlink).
 */

template <typename TSource, typename TSpec, typename TPosition>
inline typename Col<Align<TSource, TSpec> >::Type
col(Align<TSource, TSpec> & me,
    TPosition _pos)
{
    return value(cols(me), _pos);
}

template <typename TSource, typename TSpec, typename TPosition>
inline typename Col<Align<TSource, TSpec> const>::Type
col(Align<TSource, TSpec> const & me,
    TPosition _pos)
{
    return value(cols(me), _pos);
}

// ----------------------------------------------------------------------------
// Function detach()
// ----------------------------------------------------------------------------

template <typename TSource, typename TSpec>
inline void
detach(Align<TSource, TSpec> & me)
{
    typedef Align<TSource, TSpec> TAlign;
    typedef typename Rows<TAlign>::Type TRows;
    typedef typename Iterator<TRows, Standard>::Type TRowsIterator;

    TRowsIterator it = begin(rows(me));
    TRowsIterator it_end = end(rows(me));

    while (it != it_end)
    {
        detach(*it);
        ++it;
    }
}

// ----------------------------------------------------------------------------
// Function write()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#write
 * @deprecated Old-style I/O.
 * @brief Writing of Gaps to Streams in human-readable format.
 *
 * @signature void write(stream, align);
 *
 * @param[in,out] stream The Stream to write to.
 * @param[in]     align  The Align object to write out.
 */

template <typename TFile, typename TSource, typename TSpec>
inline void
_write(TFile & target,
      Align<TSource, TSpec> const & source)
{
    typedef Align<TSource, TSpec> const TAlign;
    typedef typename Row<TAlign>::Type TRow;
    typedef typename Position<typename Rows<TAlign>::Type>::Type TRowsPosition;
    typedef typename Position<TAlign>::Type TPosition;

    TRowsPosition row_count = length(rows(source));
    TPosition begin_ = 0;
    TPosition end_ = std::min(length(row(source, 0)), length(row(source, 1)));

    unsigned int baseCount = 0;
    unsigned int leftSpace = 6;
    while (begin_ < end_)
    {
        unsigned int windowSize_ = 50;
        if ((begin_ + windowSize_) > end_)
            windowSize_ = end_ - begin_;

        // Print header line
        char buffer[20];
        int len = snprintf(buffer, 20, "%7u", (unsigned)baseCount);
        write(target, buffer, len);
        baseCount += windowSize_;
        writeValue(target, ' ');
        for (TPosition i = 1; i <= windowSize_; ++i)
        {
            if ((i % 10) == 0)
                writeValue(target, ':');
            else if ((i % 5) == 0)
                writeValue(target, '.');
            else
                writeValue(target, ' ');
        }
        writeValue(target, ' ');
        writeValue(target, '\n');

        // Print sequences
        for (TRowsPosition i = 0; i < 2 * row_count - 1; ++i)
        {
            for (unsigned int j = 0; j < leftSpace + 2; ++j)
                writeValue(target, ' ');
            if ((i % 2) == 0)
            {
                TRow & row_ = row(source, i / 2);
                typedef typename Iterator<typename Row<TAlign>::Type const, Standard>::Type TIter;
                TIter begin1_ = iter(row_, begin_);
                TIter end1_ = iter(row_, begin_ + windowSize_);
                for (; begin1_ != end1_; ++begin1_)
                {
                    if (isGap(begin1_))
                        writeValue(target, gapValue<char>());
                    else
                        writeValue(target, getValue(begin1_));
                }
            }
            else
            {
                for (unsigned int j = 0; j < windowSize_; ++j)
                {
                    if ((!isGap(row(source, (i - 1) / 2), begin_ + j)) &&
                        (!isGap(row(source, (i + 1) / 2), begin_ + j)) &&
                        (row(source, (i - 1) / 2)[begin_ + j] == row(source, (i + 1) / 2)[begin_ + j]))
                    {
                        writeValue(target, '|');
                    }
                    else
                    {
                        writeValue(target, ' ');
                    }
                }
            }
            writeValue(target, '\n');
        }
        writeValue(target, '\n');
        begin_ += 50;
    }
    writeValue(target, '\n');
}

template <typename TFile, typename TSource, typename TSpec>
[[deprecated("Old-style I/O. Use stream operator << instead.")]]
inline void
write(TFile & target,
      Align<TSource, TSpec> const & source)
{
    _write(target, source);
}

// ----------------------------------------------------------------------------
// Function clearClipping()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#clearClipping
 * @brief Clear clipping on all rows.
 *
 * @signature void clearClipping(align);
 *
 * @param[in,out] align Align object to clear clippings of.
 */

// TODO(holtgrew): Undocumented.

template <typename TSource, typename TSpec>
inline void
clearClipping(Align<TSource, TSpec> & align_)
{
    typedef typename Rows<Align<TSource, TSpec> >::Type TRows;
    typedef typename Iterator<TRows>::Type TRowsIterator;

    for (TRowsIterator it = begin(rows(align_)); it != end(rows(align_)); goNext(it))
        clearClipping(*it);
}

// ----------------------------------------------------------------------------
// Function operator<<()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#operator<<
 * @brief Stream-output for Align objects.
 *
 * @signature TStream operator<<(stream, align);
 *
 * @param[in,out] stream <tt>std::ostream</tt> to write to.
 * @param[in]     align  Align object to write out.
 *
 * @return TStream Reference to stream after output of align.
 */

// stream operators

template <typename TStream, typename TSource, typename TSpec>
inline TStream &
operator<<(TStream & target,
           Align<TSource, TSpec> const & source)
{
    typename DirectionIterator<TStream, Output>::Type it = directionIterator(target, Output());
    _write(it, source);
    return target;
}

// ----------------------------------------------------------------------------
// Function setStrings()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#setStrings
 * @brief Loads the sequences of a string set into an alignment.
 *
 * @signature void setStrings(align, stringSet);
 *
 * @param[in,out] align     Align object to set underlying sequence of.
 * @param[in]     stringSet The @link StringSet @endlink with the data.
 */

template <typename TSource, typename TSpec, typename TSpec2>
inline void
setStrings(Align<TSource, TSpec> & me,
           StringSet<TSource, TSpec2> & stringset)
{
    typedef Align<TSource, TSpec> TAlign;
    typedef StringSet<TSource, TSpec2> TStringset;

    typedef typename Rows<TAlign>::Type TRows;
    typedef typename Iterator<TRows>::Type TRowsIterator;
    typedef typename Size<TStringset>::Type TStringsetSize;

    clear(me.data_rows);
    resize(me.data_rows, length(stringset));
    TRowsIterator it = begin(rows(me));
    TStringsetSize stringset_length = length(stringset);
    for (TStringsetSize i = 0; i < stringset_length; ++i)
    {
        setSource(*it, value(stringset, i));
        ++it;
    }
}

// ----------------------------------------------------------------------------
// Function clearGaps()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#clearGaps
 * @brief Clear gaps of all Align rows.
 *
 * @signature void clearGaps(align);
 *
 * @param[in] align The Align object to clear all all gaps from.
 */

template <typename TSource, typename TSpec>
inline void
clearGaps(Align<TSource, TSpec> & me)
{
    typedef Align<TSource, TSpec> TAlign;
    typedef typename Rows<TAlign>::Type TRows;
    typedef typename Iterator<TRows>::Type TRowsIterator;

    for (TRowsIterator it = begin(rows(me)); it != end(rows(me)); goNext(it))
        clearGaps(*it);
}

// ----------------------------------------------------------------------------
// Function stringSet()
// ----------------------------------------------------------------------------

/*!
 * @fn Align#stringSet
 * @brief Return string set with all ungapped sequences.
 *
 * @signature TStringSet stringSet(align);
 *
 * @param[in] align Align object to get sequences of.
 *
 * @return TStringSet The set of ungapped sequences (Metafunction: @link Align#StringSetType @endlink).
 */

template <typename TSource, typename TSpec>
inline typename StringSetType<Align<TSource, TSpec> >::Type
stringSet(Align<TSource, TSpec> & me)
{
    typedef Align<TSource, TSpec> TAlign;
    typedef typename StringSetType<TAlign>::Type TStringSet;

    typedef typename Rows<TAlign>::Type TRows;
    typedef typename Iterator<TRows>::Type TRowsIterator;

    TStringSet ss;

    for (TRowsIterator it = begin(rows(me)); it != end(rows(me)); goNext(it))
        appendValue(ss, source(*it));
    return ss;
}

// ----------------------------------------------------------------------------
// Function operator==()
// ----------------------------------------------------------------------------

template <typename TSource, typename TSpec>
inline bool operator==(Align<TSource, TSpec> const & lhs,
                       Align<TSource, TSpec> const & rhs)
{
    if (length(lhs.data_rows) != length(rhs.data_rows))
        return false;
    typedef typename Align<TSource, TSpec>::TGaps        TGaps;
    typedef typename Iterator<TGaps const, Rooted>::Type TIter;
    for (unsigned i = 0; i < length(rhs.data_rows); ++i)
        for (TIter itL = begin(lhs.data_rows[i], Rooted()), itR = begin(rhs.data_rows[i], Rooted()); !atEnd(itL); goNext(itL), goNext(itR))
            if (*itL != *itR)
                return false;
    return true;
}

// ----------------------------------------------------------------------------
// Function operator!=()
// ----------------------------------------------------------------------------

template <typename TSource, typename TSpec>
inline bool operator!=(Align<TSource, TSpec> const & lhs,
                       Align<TSource, TSpec> const & rhs)
{
    return !(lhs == rhs);
}

}  // namespace seqan2

#endif  // #ifndef SEQAN_INCLUDE_SEQAN_ALIGN_ALIGN_BASE_H_
