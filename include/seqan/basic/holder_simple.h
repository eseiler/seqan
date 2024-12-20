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
// Author: Andreas Gogol-Döring <andreas.doering@mdc-berlin.de>
// ==========================================================================
// Simple Holder specialization.
// ==========================================================================

#ifndef SEQAN_BASIC_HOLDER_SIMPLE_H_
#define SEQAN_BASIC_HOLDER_SIMPLE_H_

namespace seqan2 {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

/*!
 * @class SimpleHolder
 * @extends Holder
 * @headerfile <seqan/basic.h>
 * @brief Simple copying wrapper without any additional state.
 *
 * @signature template <typename TValue>
 *            class Holder<TValue, Simple>;
 *
 * @tparam TValue Type of the managed object.
 *
 * @section Remarks
 *
 * This holder stores a copy of the value.
 */

template <typename TValue>
struct Holder<TValue, Simple>
{
    typedef typename Value<Holder>::Type THolderValue;
    typedef typename Parameter_<THolderValue>::Type THolderParameter;

    // ------------------------------------------------------------------------
    // Members
    // ------------------------------------------------------------------------

    mutable typename RemoveConst_<THolderValue>::Type data_value;

    // ------------------------------------------------------------------------
    // Constructors
    // ------------------------------------------------------------------------

    Holder() {}

    Holder(Holder & source_) : data_value(source_.data_value)
    {
    }

    Holder(Holder const & source_) : data_value(source_.data_value)
    {
    }

    template <typename TSource>
    explicit
    Holder(TSource & value_) : data_value(value_)
    {
    }

    template <typename TSource>
    explicit
    Holder(TSource const & value_) : data_value(value_)
    {
    }

    // ------------------------------------------------------------------------
    // Assignment Operators;  Have to be defined in class.
    // ------------------------------------------------------------------------

    Holder &
    operator=(Holder const & source_)
    {
        data_value = source_.data_value;
        return *this;
    }

    Holder &
    operator=(THolderValue const & value_)
    {
        data_value = value_;
        return *this;
    }

    // ------------------------------------------------------------------------
    // Conversion Operators;  Have to be defined in class.
    // ------------------------------------------------------------------------

    operator THolderParameter()
    {
        return *data_value;
    }
};

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function empty()
// ----------------------------------------------------------------------------

template <typename TValue>
inline bool
empty(Holder<TValue, Simple> const & /*me*/)
{
    return false;
}

// ----------------------------------------------------------------------------
// Function dependent()
// ----------------------------------------------------------------------------

template <typename TValue>
inline bool
dependent(Holder<TValue, Simple> const & /*me*/)
{
    return false;
}

// ----------------------------------------------------------------------------
// Function clear()
// ----------------------------------------------------------------------------

template <typename TValue>
inline void
clear(Holder<TValue, Simple> & /*me*/)
{
}

// ----------------------------------------------------------------------------
// Function create()
// ----------------------------------------------------------------------------

template <typename TValue>
inline void
create(Holder<TValue, Simple> & /*me*/)
{
    // TODO(holtgrew): Should be create(me.data_value), right?
}

template <typename TValue>
inline void
create(Holder<TValue, Simple> & me,
       TValue const & value_)
{
    me.data_value = value_;
}

template <typename TValue>
inline void
create(Holder<TValue, Simple> & me,
       TValue const & value_,
       Move const &)
{
    // TODO(holtgrew): Real implementation once HasMoveConstructor metafunction is in place.
    me.data_value = value_;
}

// ----------------------------------------------------------------------------
// Function detach()
// ----------------------------------------------------------------------------

template <typename TValue>
inline void
detach(Holder<TValue, Simple> & /*me*/)
{
}

// ----------------------------------------------------------------------------
// Function setValue()
// ----------------------------------------------------------------------------

template <typename TValue>
inline void
setValue(Holder<TValue, Simple> & me,
         TValue const & value_)
{
    set(me.data_value, value_);
}

// ----------------------------------------------------------------------------
// Function value()
// ----------------------------------------------------------------------------

template <typename TValue>
inline typename Reference<Holder<TValue, Simple> >::Type
value(Holder<TValue, Simple> & me)
{
    return me.data_value;
}

template <typename TValue>
inline typename Reference<Holder<TValue, Simple> const>::Type
value(Holder<TValue, Simple> const & me)
{
    return me.data_value;
}

// ----------------------------------------------------------------------------
// Function getValue()
// ----------------------------------------------------------------------------

template <typename TValue>
inline typename GetValue<Holder<TValue, Simple> >::Type
getValue(Holder<TValue, Simple> & me)
{
    return me.data_value;
}

template <typename TValue>
inline typename GetValue<Holder<TValue, Simple> const>::Type
getValue(Holder<TValue, Simple> const & me)
{
    return me.data_value;
}

// ----------------------------------------------------------------------------
// Function assignValue()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSource>
inline void
assignValue(Holder<TValue, Simple> & me,
            TSource const & value_)
{
    assign(me.data_value, value_);
}

// ----------------------------------------------------------------------------
// Function moveValue()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSource>
inline void
moveValue(Holder<TValue, Simple> & me,
          TSource const & value_)
{
    move(me.data_value, value_);
}

// ----------------------------------------------------------------------------
// Function assign()
// ----------------------------------------------------------------------------

template <typename TValue>
inline void
assign(Holder<TValue, Simple> & target_,
       Holder<TValue, Simple> const & source_)
{
    assignValue(target_, source_);
}

}  // namespace seqan2

#endif  // #ifndef SEQAN_BASIC_HOLDER_SIMPLE_H_
