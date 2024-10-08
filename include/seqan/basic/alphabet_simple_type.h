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
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================
// The SimpleType alphabet type is the base class for all residue types.
// ==========================================================================

#ifndef SEQAN_INCLUDE_SEQAN_BASIC_ALPHABET_SIMPLE_H_
#define SEQAN_INCLUDE_SEQAN_BASIC_ALPHABET_SIMPLE_H_

namespace seqan2 {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ----------------------------------------------------------------------------
// Class SimpleType
// ----------------------------------------------------------------------------

/*!
 * @class SimpleType
 * @implements FiniteOrderedAlphabetConcept
 * @headerfile <seqan/basic.h>
 *
 * @brief Implementation for "simple" types.
 *
 * @signature template <typename TValue, typename TSpec>
 *            class SimpleType;
 *
 * @tparam TSpec  Specialization tag.
 * @tparam TValue Type that stores the values of an instance.  TValue must be a simple type.
 *
 * A "simple type" is a C++ type that can be constructed without constructor, destructed without destructor and copied
 * without copy constructor or assignment operator.  All basic types (like <tt>char</tt>, <tt>int</tt> or
 * <tt>float</tt>) are simple. Pointers, references and arrays of simple types are simple.  POD types ("plain old data
 * types"), that are - simplified spoken - C++-types that already existed in C, are simple too.
 *
 * Arrays of simple types can be copied very fast by memory manipulation routines, but the default implementation of
 * functions like arrayCopyForward and arrayCopy are not optimized for simple types this way.  But for classes derived
 * from <tt>SimpleType</tt>, optimized variants of array manipulation functions are applied.
 *
 * Note that simple types need not to be derived or specialized from <tt>SimpleType</tt>, but it could be convenient to
 * do so.
 *
 * @see IsSimple
 */

/*!
 * @var TValue SimpleType::value
 * @brief The internal value storage of the SimpleType object.
 *
 * @important Do not modify this value directly.  SimpleType implements conversion operators for all numeric types, so
 *            you can simply assign and cast the SimpleType to all built-in numeric types.
 *
 * Stores the value of the SimpleType, is of type <tt>TValue</tt>.  Valid values are from <tt>0</tt> to @link
 * FiniteOrderedAlphabetConcept#ValueSize @endlink minus one.
 */

/*!
 * @fn SimpleType::SimpleType
 * @brief Constructor of SimpleType type.
 *
 * @signature SimpleType::SimpleType();
 * @signature SimpleType::SimpleType(other);
 * @signature template <typename TParam> SimpleType::SimpleType(param);
 *
 * @param[in] other Other SimpleType to copy construct with.
 * @param[in] param Any value of type <tt>TParam</tt> that can be assigned to the SimpleType object.
 *
 * The default constructor initializes the SimpleType object with the value <tt>0</tt> and the copy constructor copies
 * over the value of the SimpleType.
 *
 * When constructing with a <tt>param</tt> that is not a SimpleType then <tt>param</tt> is assigned to the SimpleType
 * object, using @link AssignableConcept#assign assign @endlink.  This function can be overloaded differently for each
 * type.  You can expect the following behaviour for all SimpleType objects, however:
 *
 * <ul>
 * <li>If <tt>param</tt> is a builtin integer then this value is directly assigned to the @link SimpleType::value value
 *     @endlink member of the SimpleType object.  Note that this can allow an invalid assignment, e.g., when assigning
 *     <tt>42</tt> to a @link Dna @endlink object.</li>
 * <li>If <tt>param</tt> is a <tt>char</tt> then this character value is converted to the appropriate value and written
 *     to the @link SimpleType::value value @endlink member.  For example, assigning <tt>'A'</tt> or <tt>'a'</tt> to
 *     a SimpleType object assigns <tt>0</tt> to the @link SimpleType::value value @endlink member.</li>
 * </ul>
 *
 * @section Example
 *
 * The following example shows construction of a @link Dna @endlink (specialization of SimpleType) object with from
 * <tt>char</tt> and integer values.
 *
 * @snippet demos/dox/basic/simple_type_construction.cpp simple type construction and assignment
 */

#pragma pack(push,1)
template <typename TValue, typename TSpec>
class SimpleType
{
public:
    // ------------------------------------------------------------------------
    // Members;  Have to be defined in class.
    // ------------------------------------------------------------------------

    TValue value{0};

    // ------------------------------------------------------------------------
    // Constructors;  Have to be defined in class.
    // ------------------------------------------------------------------------

    // TODO(holtgrew): Do we want default initialization?
    SimpleType() = default;
    SimpleType(SimpleType const &) = default;
    SimpleType(SimpleType &&) = default;
    ~SimpleType() = default;

    // TODO(holtgrew): Do we want an explicit here?
    template <typename T>
    SimpleType(T const & other)
    {
        assign(*this, other);
    }

    // ------------------------------------------------------------------------
    // Assignment Operator;  Have to be defined in class.
    // ------------------------------------------------------------------------
    SimpleType & operator=(SimpleType const &) = default;
    SimpleType & operator=(SimpleType &&) = default;

    template <typename T>
    inline SimpleType &
    operator=(T const & other)
    {
        assign(*this, other);
        return *this;
    }

    // ------------------------------------------------------------------------
    // Conversion Operators;  Have to be defined in class.
    // ------------------------------------------------------------------------

    // Class.SimpleType specifies type conversion operators for all built-in
    // integer types since there is no way to extend the build-in types with
    // copy and assignment constructors in C++.
    //
    // This cannot be a template since it would conflict to the template
    // constructor.


    operator int64_t() const
    {
        int64_t c;
        assign(c, *this);
        return c;
    }


    operator uint64_t() const
    {
        uint64_t c;
        assign(c, *this);
        return c;
    }


    operator int() const
    {
        int c;
        assign(c, *this);
        return c;
    }


    operator unsigned int() const
    {
        unsigned int c;
        assign(c, *this);
        return c;
    }


    operator short() const
    {
        short c;
        assign(c, *this);
        return c;
    }


    operator unsigned short() const
    {
        unsigned short c;
        assign(c, *this);
        return c;
    }


    operator char() const
    {
        char c;
        assign(c, *this);
        return c;
    }


    operator signed char() const
    {
        signed char c;
        assign(c, *this);
        return c;
    }


    operator unsigned char() const
    {
        unsigned char c;
        assign(c, *this);
        return c;
    }
};
#pragma pack(pop)

} // namespace seqan2

// ----------------------------------------------------------------------------
// Class numeric_limits
// ----------------------------------------------------------------------------
namespace std
{

template <typename TValue, typename TSpec>
class numeric_limits<seqan2::SimpleType<TValue, TSpec> >
{
public:
    static constexpr bool is_specialized    = true;
    static constexpr bool is_signed         = false;
    static constexpr bool is_integer        = false;
    static constexpr bool is_exact          = true;
    static constexpr bool has_infinity      = false;
    static constexpr bool has_quiet_NaN     = false;
    static constexpr bool has_signaling_NaN = false;
// P2614R2: std::float_denorm_style, has_denorm, and has_denorm_loss are deprecated in C++23.
#if defined(__GNUC__) && __cplusplus >= 202302
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
    static constexpr float_denorm_style has_denorm = denorm_absent;
#if defined(__GNUC__) && __cplusplus >= 202302
#    pragma GCC diagnostic pop
#endif
    static constexpr bool has_denorm_loss   = false;
    static constexpr float_round_style round_style = round_toward_zero;
    static constexpr bool is_iec559         = false;
    static constexpr bool is_bounded        = true;
    static constexpr bool is_modulu         = false;
    static constexpr int  digits            = seqan2::BitsPerValue<seqan2::SimpleType<TValue, TSpec>>::VALUE;
    static constexpr int  digits10          = digits - 1;
    static constexpr int  max_digits10      = 0;
    static constexpr int  radix             = 2;
    static constexpr int  min_exponent      = 0;
    static constexpr int  min_exponent10    = 0;
    static constexpr int  max_exponent      = 0;
    static constexpr int  max_exponent10    = 0;
    static constexpr bool traps             = false;
    static constexpr bool tinyness_before   = false;

    static constexpr seqan2::SimpleType<TValue, TSpec> min()
    {
        return seqan2::SimpleType<TValue, TSpec>(0);
    }

    static constexpr seqan2::SimpleType<TValue, TSpec> max()
    {
        return seqan2::SimpleType<TValue, TSpec>(((TValue)seqan2::ValueSize<seqan2::SimpleType<TValue, TSpec> >::VALUE - 1));
    }

    static constexpr seqan2::SimpleType<TValue, TSpec> lowest()
    {
        return seqan2::SimpleType<TValue, TSpec>(0);
    }

    static constexpr seqan2::SimpleType<TValue, TSpec> infinity()
    {
        return seqan2::SimpleType<TValue, TSpec>(((TValue)seqan2::ValueSize<seqan2::SimpleType<TValue, TSpec> >::VALUE - 1));
    }
};

} //namespace std

namespace seqan2
{
// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction IsSimple
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct IsSimple<SimpleType<TValue, TSpec> >
{
    typedef True Type;
};

// ----------------------------------------------------------------------------
// Concept Convertible
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TSource>
struct Is< Convertible<SimpleType<TValue, TSpec>, TSource> > :
    Is< FundamentalConcept<TSource> > {};

template <typename TTarget, typename TValue, typename TSpec>
struct Is< Convertible<TTarget, SimpleType<TValue, TSpec> > > :
    Is< FundamentalConcept<TTarget> > {};

// ----------------------------------------------------------------------------
// Metafunction Value
// ----------------------------------------------------------------------------

// TODO(holtgrew): Rename? SimpleType is no container!

template <typename TValue, typename TSpec>
struct Value<SimpleType<TValue, TSpec> >
{
    typedef TValue Type;
};

template <typename TValue, typename TSpec>
struct Value<SimpleType<TValue, TSpec> const>
{
    typedef TValue const Type;
};

// ----------------------------------------------------------------------------
// Metafunction MinValue
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct MinValue<SimpleType<TValue, TSpec> >
{
    static const SimpleType<TValue, TSpec> VALUE;
};

template <typename TValue, typename TSpec>
const SimpleType<TValue, TSpec> MinValue<SimpleType<TValue, TSpec> >::VALUE = std::numeric_limits<SimpleType<TValue, TSpec>>::min();

template <typename TValue, typename TSpec>
inline SimpleType<TValue, TSpec> const &
infimumValueImpl(SimpleType<TValue, TSpec> *)
{
    return MinValue<SimpleType<TValue, TSpec> >::VALUE;
}

// ----------------------------------------------------------------------------
// Metafunction MaxValue
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct MaxValue<SimpleType<TValue, TSpec> >
{
    static const SimpleType<TValue, TSpec> VALUE;
};

template <typename TValue, typename TSpec>
const SimpleType<TValue, TSpec> MaxValue<SimpleType<TValue, TSpec> >::VALUE = std::numeric_limits<SimpleType<TValue, TSpec>>::max();

template <typename TValue, typename TSpec>
inline SimpleType<TValue, TSpec> const &
supremumValueImpl(SimpleType<TValue, TSpec> *)
{
    return MaxValue<SimpleType<TValue, TSpec> >::VALUE;
}

// ----------------------------------------------------------------------------
// Metafunction Spec
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
struct Spec<SimpleType<TValue, TSpec> >
{
    typedef TSpec Type;
};

template <typename TValue, typename TSpec>
struct Spec<SimpleType<TValue, TSpec> const>
{
    typedef TSpec Type;
};

// ----------------------------------------------------------------------------
// Metafunction CompareType
// ----------------------------------------------------------------------------

// TODO(doering): muss geprueft werden, ob diese Metafunktion noch ausgeweitet oder aber versteckt wird.
// TODO(holtgrew): Is this at the right place here? Do we need it for all types?
// TODO(holtgrew): CompareType is not symmetric because of class instantiation conflicts. Evaluate these problems and possibly fix them.
// TODO(holtgrew): Is some of the code below redundant, can we lose some copy and paste here?

template <typename TValue, typename TSpec, typename TRight>
struct CompareTypeImpl<SimpleType<TValue, TSpec>, TRight>
{
    typedef TRight Type;
};

// ============================================================================
// Functions
// ============================================================================

// TODO(holtgrew): Why are functions with Proxies defined here? Better in Proxy header? Or externalize?

// ----------------------------------------------------------------------------
// Function convertImpl()
// ----------------------------------------------------------------------------

// TODO(holtgrew): Document

template <typename TTarget, typename T, typename TSourceValue, typename TSourceSpec>
inline typename RemoveConst_<TTarget>::Type
convertImpl(Convert<TTarget, T> const,
            SimpleType<TSourceValue, TSourceSpec> const & source_)
{
    typename RemoveConst_<TTarget>::Type target_;
    assign(target_, source_);
    return target_;
}

// ----------------------------------------------------------------------------
// Function operator<<()
// ----------------------------------------------------------------------------

template <typename TStream, typename TValue, typename TSpec>
inline TStream &
operator<<(TStream & stream,
           SimpleType<TValue, TSpec> const & data)
{
    stream << convert<char>(data);
    return stream;
}

// ----------------------------------------------------------------------------
// Function operator>>()
// ----------------------------------------------------------------------------

template <typename TStream, typename TValue, typename TSpec>
inline TStream &
operator>>(TStream & stream,
           SimpleType<TValue, TSpec> & data)
{
    char c;
    stream >> c;
    assign(data, c);
    return stream;
}

// ----------------------------------------------------------------------------
// Function assign()
// ----------------------------------------------------------------------------

template <typename TTargetValue, typename TTargetSpec, typename TSourceValue, typename TSourceSpec>
inline void
assign(SimpleType<TTargetValue, TTargetSpec> & target,
       SimpleType<TSourceValue, TSourceSpec> & source)
{
    target.value = source.value;
}

template <typename TTargetValue, typename TTargetSpec, typename TSourceValue, typename TSourceSpec>
inline void
assign(SimpleType<TTargetValue, TTargetSpec> & target,
       SimpleType<TSourceValue, TSourceSpec> const & source)
{
    target.value = source.value;
}

template <typename TTargetValue, typename TTargetSpec, typename TSource>
inline void
assign(SimpleType<TTargetValue, TTargetSpec> & target,
       TSource & source)
{
    target.value = source;
}

template <typename TTargetValue, typename TTargetSpec, typename TSource>
inline void
assign(SimpleType<TTargetValue, TTargetSpec> & target,
       TSource const & source)
{
    target.value = source;
}

// Assign Proxy to SimpleType
// NOTE(doering): Diese Funktionen wurden noetig wegen eines seltsamen VC++-Verhaltens
// TODO(holtgrew): Still necessary with dropped 2003 support?

template <typename TTargetValue, typename TTargetSpec, typename TSourceSpec>
inline void
assign(SimpleType<TTargetValue, TTargetSpec> & target,
       Proxy<TSourceSpec> & source)
{
    target.value = getValue(source);
}

template <typename TTargetValue, typename TTargetSpec, typename TSourceSpec>
inline void
assign(SimpleType<TTargetValue, TTargetSpec> & target,
       Proxy<TSourceSpec> const & source)
{
    target.value = getValue(source);
}

//INTEGRAL TYPES
// NOTE(doering): It is not possible to write a single function here since "assign" must be specialized for the first argument at the first place

template <typename TValue, typename TSpec>
inline void
assign(int64_t & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(int64_t & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(uint64_t & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(uint64_t & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(int & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(int & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(unsigned int & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(unsigned int & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(short & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(short & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(unsigned short & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(unsigned short & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(char & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(char & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(signed char & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(signed char & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(unsigned char & c_target,
       SimpleType<TValue, TSpec> & source)
{
    c_target = source.value;
}

template <typename TValue, typename TSpec>
inline void
assign(unsigned char & c_target,
       SimpleType<TValue, TSpec> const & source)
{
    c_target = source.value;
}

// ----------------------------------------------------------------------------
// Function operator==()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TRight>
inline bool
operator==(SimpleType<TValue, TSpec> const & left_,
           TRight const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) == convert<TCompareType>(right_);
}

template <typename TLeft, typename TValue, typename TSpec>
inline bool
operator==(TLeft const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TRight, TLeft>::Type TCompareType;
    return convert<TCompareType>(left_) == convert<TCompareType>(right_);
}

template <typename TLeftValue, typename TLeftSpec, typename TRightValue, typename TRightSpec>
inline bool
operator==(SimpleType<TLeftValue, TLeftSpec> const & left_,
           SimpleType<TRightValue, TRightSpec> const & right_)
{
    typedef SimpleType<TLeftValue, TLeftSpec> TLeft;
    typedef SimpleType<TRightValue, TRightSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) == convert<TCompareType>(right_);
}

template <typename TValue, typename TSpec>
inline bool
operator==(SimpleType<TValue, TSpec> const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    return convert<TValue>(left_) == convert<TValue>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator==(Proxy<TSpec> const & left_,
           SimpleType<TValue, TSpec2> const & right_)
{
    typedef Proxy<TSpec> TLeft;
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) == convert<TCompareType>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator==(SimpleType<TValue, TSpec2> const & left_,
           Proxy<TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef Proxy<TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) == convert<TCompareType>(right_);
}

// ----------------------------------------------------------------------------
// Function operator!=()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TRight>
inline bool
operator!=(SimpleType<TValue, TSpec> const & left_,
           TRight const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) != convert<TCompareType>(right_);
}

template <typename TLeft, typename TValue, typename TSpec>
inline bool
operator!=(TLeft const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TRight, TLeft>::Type TCompareType;
    return convert<TCompareType>(left_) != convert<TCompareType>(right_);
}

template <typename TLeftValue, typename TLeftSpec, typename TRightValue, typename TRightSpec>
inline bool
operator!=(SimpleType<TLeftValue, TLeftSpec> const & left_,
           SimpleType<TRightValue, TRightSpec> const & right_)
{
    typedef SimpleType<TLeftValue, TLeftSpec> TLeft;
    typedef SimpleType<TRightValue, TRightSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) != convert<TCompareType>(right_);
}

template <typename TValue, typename TSpec>
inline bool
operator!=(SimpleType<TValue, TSpec> const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    return convert<TValue>(left_) != convert<TValue>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator!=(Proxy<TSpec> const & left_,
           SimpleType<TValue, TSpec2> const & right_)
{
    typedef Proxy<TSpec> TLeft;
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) != convert<TCompareType>(right_);
}

template <typename TValue, typename TSpec, typename TProxySpec>
inline bool
operator!=(SimpleType<TValue, TSpec> const & left_,
           Proxy<TProxySpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef Proxy<TProxySpec> TRight;
    typedef typename CompareType<TRight, TLeft>::Type TCompareType;
    return convert<TCompareType>(left_) != convert<TCompareType>(right_);
}

// ----------------------------------------------------------------------------
// Function operator<()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TRight>
inline bool
operator<(SimpleType<TValue, TSpec> const & left_,
          TRight const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) < convert<TCompareType>(right_);
}

template <typename TLeft, typename TValue, typename TSpec>
inline bool
operator<(TLeft const & left_,
          SimpleType<TValue, TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TRight, TLeft>::Type TCompareType;
    return convert<TCompareType>(left_) < convert<TCompareType>(right_);
}

template <typename TLeftValue, typename TLeftSpec, typename TRightValue, typename TRightSpec>
inline bool
operator<(SimpleType<TLeftValue, TLeftSpec> const & left_,
          SimpleType<TRightValue, TRightSpec> const & right_)
{
    typedef SimpleType<TLeftValue, TLeftSpec> TLeft;
    typedef SimpleType<TRightValue, TRightSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) < convert<TCompareType>(right_);
}

template <typename TValue, typename TSpec>
inline bool
operator<(SimpleType<TValue, TSpec> const & left_,
          SimpleType<TValue, TSpec> const & right_)
{
    return convert<TValue>(left_) < convert<TValue>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator<(Proxy<TSpec> const & left_,
          SimpleType<TValue, TSpec2> const & right_)
{
    typedef Proxy<TSpec> TLeft;
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) < convert<TCompareType>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator<(SimpleType<TValue, TSpec2> const & left_,
          Proxy<TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef Proxy<TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) < convert<TCompareType>(right_);
}

// ----------------------------------------------------------------------------
// Function operator<=()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TRight>
inline bool
operator<=(SimpleType<TValue, TSpec> const & left_,
           TRight const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) <= convert<TCompareType>(right_);
}

template <typename TLeft, typename TValue, typename TSpec>
inline bool
operator<=(TLeft const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TRight, TLeft>::Type TCompareType;
    return convert<TCompareType>(left_) <= convert<TCompareType>(right_);
}

template <typename TLeftValue, typename TLeftSpec, typename TRightValue, typename TRightSpec>
inline bool
operator<=(SimpleType<TLeftValue, TLeftSpec> const & left_,
           SimpleType<TRightValue, TRightSpec> const & right_)
{
    typedef SimpleType<TLeftValue, TLeftSpec> TLeft;
    typedef SimpleType<TRightValue, TRightSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) <= convert<TCompareType>(right_);
}

template <typename TValue, typename TSpec>
inline bool
operator<=(SimpleType<TValue, TSpec> const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    return convert<TValue>(left_) <= convert<TValue>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator<=(Proxy<TSpec> const & left_,
           SimpleType<TValue, TSpec2> const & right_)
{
    typedef Proxy<TSpec> TLeft;
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) <= convert<TCompareType>(right_);
}
template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator<=(SimpleType<TValue, TSpec2> const & left_,
           Proxy<TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef Proxy<TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) <= convert<TCompareType>(right_);
}

// ----------------------------------------------------------------------------
// Function operator>()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TRight>
inline bool
operator>(SimpleType<TValue, TSpec> const & left_,
          TRight const & right_)
{
            typedef SimpleType<TValue, TSpec> TLeft;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) > convert<TCompareType>(right_);
}

template <typename TLeft, typename TValue, typename TSpec>
inline bool
operator>(TLeft const & left_,
          SimpleType<TValue, TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TRight, TLeft>::Type TCompareType;
    return convert<TCompareType>(left_) > convert<TCompareType>(right_);
}

template <typename TLeftValue, typename TLeftSpec, typename TRightValue, typename TRightSpec>
inline bool
operator>(SimpleType<TLeftValue, TLeftSpec> const & left_,
          SimpleType<TRightValue, TRightSpec> const & right_)
{
    typedef SimpleType<TLeftValue, TLeftSpec> TLeft;
    typedef SimpleType<TRightValue, TRightSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) > convert<TCompareType>(right_);
}

template <typename TValue, typename TSpec>
inline bool
operator>(SimpleType<TValue, TSpec> const & left_,
          SimpleType<TValue, TSpec> const & right_)
{
    return convert<TValue>(left_) > convert<TValue>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator>(Proxy<TSpec> const & left_,
          SimpleType<TValue, TSpec2> const & right_)
{
    typedef Proxy<TSpec> TLeft;
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) > convert<TCompareType>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator>(SimpleType<TValue, TSpec2> const & left_,
          Proxy<TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef Proxy<TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) > convert<TCompareType>(right_);
}

// ----------------------------------------------------------------------------
// Function operator>=()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec, typename TRight>
inline bool
operator>=(SimpleType<TValue, TSpec> const & left_,
           TRight const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) >= convert<TCompareType>(right_);
}

template <typename TLeft, typename TValue, typename TSpec>
inline bool
operator>=(TLeft const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TRight, TLeft>::Type TCompareType;
    return convert<TCompareType>(left_) >= convert<TCompareType>(right_);
}

template <typename TLeftValue, typename TLeftSpec, typename TRightValue, typename TRightSpec>
inline bool
operator>=(SimpleType<TLeftValue, TLeftSpec> const & left_,
           SimpleType<TRightValue, TRightSpec> const & right_)
{
    typedef SimpleType<TLeftValue, TLeftSpec> TLeft;
    typedef SimpleType<TRightValue, TRightSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) >= convert<TCompareType>(right_);
}

template <typename TValue, typename TSpec>
inline bool
operator>=(SimpleType<TValue, TSpec> const & left_,
           SimpleType<TValue, TSpec> const & right_)
{
    return convert<TValue>(left_) >= convert<TValue>(right_);
}

template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator>=(Proxy<TSpec> const & left_,
           SimpleType<TValue, TSpec2> const & right_)
{
    typedef Proxy<TSpec> TLeft;
    typedef SimpleType<TValue, TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) >= convert<TCompareType>(right_);
}
template <typename TSpec, typename TValue, typename TSpec2>
inline bool
operator>=(SimpleType<TValue, TSpec2> const & left_,
           Proxy<TSpec> const & right_)
{
    typedef SimpleType<TValue, TSpec> TLeft;
    typedef Proxy<TSpec> TRight;
    typedef typename CompareType<TLeft, TRight>::Type TCompareType;
    return convert<TCompareType>(left_) >= convert<TCompareType>(right_);
}

// ----------------------------------------------------------------------------
// Function operator++()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline SimpleType<TValue, TSpec> &
operator++(SimpleType<TValue, TSpec> & me)
{
    ++me.value;
    return me;
}

template <typename TValue, typename TSpec>
inline SimpleType<TValue, TSpec>
operator++(SimpleType<TValue, TSpec> & me, int)
{
    SimpleType<TValue, TSpec> dummy = me;
    ++me.value;
    return dummy;
}

// ----------------------------------------------------------------------------
// Function operator--()
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline SimpleType<TValue, TSpec> &
operator--(SimpleType<TValue, TSpec> & me)
{
    --me.value;
    return me;
}

template <typename TValue, typename TSpec>
inline SimpleType<TValue, TSpec>
operator--(SimpleType<TValue, TSpec> & me, int)
{
    SimpleType<TValue, TSpec> dummy = me;
    --me.value;
    return dummy;
}

// ----------------------------------------------------------------------------
// Function operator+()                                                 [unary]
// ----------------------------------------------------------------------------

template <typename TValue, typename TSpec>
inline SimpleType<TValue, TSpec>
operator+(SimpleType<TValue, TSpec> const & v)
{
    return v;
}

template <typename TValue, typename TSpec>
inline typename ValueSize<SimpleType<TValue, TSpec> >::Type
_internalOrdValue(SimpleType<TValue, TSpec> const & c)
{
    return c.value;
}

template <typename TValue, typename TSpec>
inline typename ValueSize<SimpleType<TValue, TSpec> >::Type
ordValue(SimpleType<TValue, TSpec> const & c)
{
    return convert<unsigned>(c);
}

}  // namespace seqan2

#endif  // #ifndef SEQAN_INCLUDE_SEQAN_BASIC_ALPHABET_SIMPLE_H_
