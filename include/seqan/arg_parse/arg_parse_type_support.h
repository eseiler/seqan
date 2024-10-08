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
// Author: Stephan Aiche <stephan.aiche@fu-berlin.de>
// ==========================================================================

// TODO(aiche): check if parts of the functionality could be merged with lexicalCast in stream module

#ifndef SEQAN_INCLUDE_ARG_PARSE_ARG_PARSE_TYPE_SUPPRT_H_
#define SEQAN_INCLUDE_ARG_PARSE_ARG_PARSE_TYPE_SUPPRT_H_

#include <sstream>
#include <string>

#include <seqan/sequence.h>

namespace seqan2 {


// ==========================================================================
// Tags, Classes, Enums
// ==========================================================================

template <typename TVoidSpec = void>
struct BooleanArgumentValues_
{
    static constexpr std::array<const char *, 5> LIST_TRUE{{"1", "ON", "TRUE", "T", "YES"}};
    static constexpr std::array<const char *, 5> LIST_FALSE{{"0", "OFF", "FALSE", "F", "NO"}};
};

template <typename TVoidSpec>
constexpr std::array<const char *, 5> BooleanArgumentValues_<TVoidSpec>::LIST_TRUE;

template <typename TVoidSpec>
constexpr std::array<const char *, 5> BooleanArgumentValues_<TVoidSpec>::LIST_FALSE;

// ==========================================================================
// Functions
// ==========================================================================

// ----------------------------------------------------------------------------
// Function _tryCast()
// ----------------------------------------------------------------------------
template <typename TTarget, typename TString>
inline bool _tryCast(TTarget & dest, TString const source)
{
    std::istringstream stream(toCString(source));
    bool result = (!(stream >> dest).fail()) && (stream.rdbuf()->in_avail() == 0);
    return result;
}

template <typename TString>
inline bool _tryCast(bool & dst, TString const & s)
{
    // since the validValue check already verifies that the value is one of
    // BooleanArgumentValues_<>::LIST_TRUE and BooleanArgumentValues_<>::LIST_FALSE,
    // one only needs to check one equivalent class
    std::string s_uppercase{s};
    std::transform(s.begin(), s.end(), s_uppercase.begin(), ::toupper); // allow for lowercase letters
    dst = (std::find(BooleanArgumentValues_<>::LIST_TRUE.begin(),
                     BooleanArgumentValues_<>::LIST_TRUE.end(),
                     s_uppercase)
           != BooleanArgumentValues_<>::LIST_TRUE.end());
    return true;
}

// ----------------------------------------------------------------------------
// Function _cast()
// ----------------------------------------------------------------------------
template <typename TTarget, typename TString>
inline TTarget _cast(TString const s)
{
    // We are sing additional braces here for "most vexing parse" resolution
    // when enforcing default initialization also for built-in types.
    TTarget dst((TTarget()));
    std::istringstream stream(toCString(s));
    bool result = (!(stream >> dst).fail()) && (stream.rdbuf()->in_avail() == 0);
    SEQAN_CHECK(result, "could not cast %s", toCString(s));
    return dst;
}

// ----------------------------------------------------------------------------
// Function _isCastable()
// ----------------------------------------------------------------------------
template <typename TTarget, typename TString>
inline bool _isCastable(TString const s)
{
    TTarget dst;
    std::istringstream stream(toCString(s));
    return (!(stream >> dst).fail()) && (stream.rdbuf()->in_avail() == 0);
}

// ----------------------------------------------------------------------------
// Function _isDouble()
// ----------------------------------------------------------------------------

template <typename TString>
inline bool _isDouble(TString const s)
{
    return _isCastable<double>(s);
}

// ----------------------------------------------------------------------------
// Function _isInt()
// ----------------------------------------------------------------------------

template <typename TString>
inline bool _isInt(TString const s)
{
    return _isCastable<int>(s);
}

// ----------------------------------------------------------------------------
// Function _convertFlagValue()
// ----------------------------------------------------------------------------

inline bool _convertFlagValue(bool & dst, std::string const & src)
{
    dst = !empty(src) && (src != "false");
    return true;
}

template <typename TObject>
inline bool _convertFlagValue(TObject & /*dst*/, std::string const & /*s*/)
{
    return false;
}

// ----------------------------------------------------------------------------
// Function _convertArgumentValue()
// ----------------------------------------------------------------------------

class ArgParseArgument;
inline bool isIntegerArgument(ArgParseArgument const & me);
inline bool isInt64Argument(ArgParseArgument const & me);
inline bool isDoubleArgument(ArgParseArgument const & me);
inline bool isStringArgument(ArgParseArgument const & me);
inline bool isBooleanArgument(ArgParseArgument const & me);

inline bool _convertArgumentValue(bool & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isBooleanArgument(opt))
        return false;

    return _tryCast(dst, src);
}

inline bool _convertArgumentValue(int & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isIntegerArgument(opt))
        return false;

    return _tryCast(dst, src);
}

inline bool _convertArgumentValue(unsigned int & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isIntegerArgument(opt))
        return false;

    return _tryCast(dst, src);
}

inline bool _convertArgumentValue(int64_t & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isIntegerArgument(opt) && !isInt64Argument(opt))
        return false;

    return _tryCast(dst, src);
}

inline bool _convertArgumentValue(uint64_t & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isIntegerArgument(opt) && !isInt64Argument(opt))
        return false;

    return _tryCast(dst, src);
}

inline bool _convertArgumentValue(float & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isDoubleArgument(opt))
        return false;

    return _tryCast(dst, src);
}

inline bool _convertArgumentValue(double & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isDoubleArgument(opt))
        return false;

    return _tryCast(dst, src);
}

template <typename TObject>
inline bool _convertArgumentValue(TObject & dst, ArgParseArgument const & opt, std::string const & src)
{
    if (!isStringArgument(opt))
        return false;

    assign(dst, src);
    return true;
}

} // namespace seqan2

#endif // SEQAN_INCLUDE_ARG_PARSE_ARG_PARSE_TYPE_SUPPRT_H_
