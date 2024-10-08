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
// Code for reading Bam.
// ==========================================================================

#ifndef INCLUDE_SEQAN_BAM_IO_READ_BAM_H_
#define INCLUDE_SEQAN_BAM_IO_READ_BAM_H_

namespace seqan2 {

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

/*!
 * @tag FileFormats#Bam
 * @brief Identify the BAM format.
 *
 * @tag FileFormats#Sam
 * @brief Identify the SAM format.
 */

struct Bam_;
typedef Tag<Bam_> Bam;


template <typename T>
struct FileExtensions<Bam, T>
{
    static char const * VALUE[1];    // default is one extension
};

template <typename T>
char const * FileExtensions<Bam, T>::VALUE[1] =
{
    ".bam"     // default output extension
};


template <typename T>
struct MagicHeader<Bam, T>
{
    static unsigned char const VALUE[4];
};

template <typename T>
unsigned char const MagicHeader<Bam, T>::VALUE[4] = { 'B', 'A', 'M', '\1' };  // BAM's magic header

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function readHeader()                                              BamHeader
// ----------------------------------------------------------------------------

template <typename TForwardIter, typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline void
readHeader(BamHeader & header,
           BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> & context,
           TForwardIter & iter,
           Bam const & /*tag*/)
{
    clear(header);
    // Read BAM magic string.
    String<char, Array<4> > magic;
    read(magic, iter, 4);
    if (magic != "BAM\1")
        SEQAN_THROW(ParseError("Not in BAM format."));

    // Read header text, including null padding.
    int32_t lText;
    readRawPod(lText, iter);

    CharString samHeader;
    write(samHeader, iter, lText);

    // Truncate to first position of '\0'.
    cropAfterFirst(samHeader, EqualsChar<'\0'>());

    // Parse out header records.
    BamHeaderRecord headerRecord;
    Iterator<CharString, Rooted>::Type it = begin(samHeader);
    while (!atEnd(it))
    {
        clear(headerRecord);
        readRecord(headerRecord, context, it, Sam());
        appendValue(header, headerRecord);
    }

    // Read # reference sequences.
    int32_t nRef;
    readRawPod(nRef, iter);
    CharString name;

    clear(context.translateFile2GlobalRefId);
    resize(context.translateFile2GlobalRefId, nRef, -1);

    for (int32_t i = 0; i < nRef; ++i)
    {
        // Read length of the reference name.
        int32_t nName;
        readRawPod(nName, iter);
        clear(name);
        write(name, iter, nName);
        resize(name, nName - 1);
        // Read length of the reference sequence.
        int32_t lRef;
        readRawPod(lRef, iter);

        // Add entry to name store and sequenceInfos if necessary.
        // Compute translation from local ids (used in the BAM file) to corresponding ids in the name store
        size_t globalRefId = nameToId(contigNamesCache(context), name);
        context.translateFile2GlobalRefId[i] = globalRefId;
        if (length(contigLengths(context)) <= globalRefId)
            resize(contigLengths(context), globalRefId + 1, 0);
        contigLengths(context)[globalRefId] = lRef;
    }
}

// ----------------------------------------------------------------------------
// Function readRecord()                                     BamAlignmentRecord
// ----------------------------------------------------------------------------

template <typename TBuffer, typename TForwardIter>
inline int32_t
_readBamRecordWithoutSize(TBuffer & rawRecord, TForwardIter & iter)
{
    int32_t recordLen = 0;
    readRawPod(recordLen, iter);

    // fail, if we read "BAM\1" (did you miss to call readRecord(header, bamFile) first?)
    if (recordLen == 0x014D4142)
        SEQAN_THROW(ParseError("Unexpected BAM header encountered."));

    // The recordLen after readRawPod() was -1 for a corrupted BAM file (EOF marker absent)
    // which made the following write(rawRecord, iter, (size_t)recordLen) function call
    // throw a std::bad_alloc exception.
    SEQAN_ASSERT_MSG(recordLen >= 0, "Cannot read BAM record of length < 0. Possibly corrupted BAM file!");

    clear(rawRecord);
    write(rawRecord, iter, (size_t)recordLen);
    return recordLen;
}

template <typename TBuffer, typename TForwardIter>
inline void
_readBamRecord(TBuffer & rawRecord, TForwardIter & iter, Bam)
{
    int32_t recordLen = 0;
    readRawPod(recordLen, iter);

    // fail, if we read "BAM\1" (did you miss to call readRecord(header, bamFile) first?)
    if (recordLen == 0x014D4142)
        SEQAN_THROW(ParseError("Unexpected BAM header encountered."));

    clear(rawRecord);
    appendRawPod(rawRecord, recordLen);
    write(rawRecord, iter, (size_t)recordLen);
}

template <typename TForwardIter, typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline void
readRecord(BamAlignmentRecord & record,
           BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> & context,
           TForwardIter & iter,
           Bam const & /* tag */)
{
    typedef typename Iterator<CharString, Standard>::Type                             TCharIter;
    typedef typename Iterator<String<CigarElement<> >, Standard>::Type SEQAN_RESTRICT TCigarIter;
    typedef typename Iterator<IupacString, Standard>::Type SEQAN_RESTRICT             TSeqIter;
    typedef typename Iterator<CharString, Standard>::Type SEQAN_RESTRICT              TQualIter;

    // Read size and data of the remaining block in one chunk (fastest).
    int32_t remainingBytes = _readBamRecordWithoutSize(context.buffer, iter);
    TCharIter it = begin(context.buffer, Standard());

    // BamAlignmentRecordCore.
    arrayCopyForward(it, it + sizeof(BamAlignmentRecordCore), reinterpret_cast<char*>(&record));
    enforceLittleEndian(*reinterpret_cast<BamAlignmentRecordCore*>(&record));
    it += sizeof(BamAlignmentRecordCore);

    remainingBytes -= sizeof(BamAlignmentRecordCore) + record._l_qname +
                      record._n_cigar * 4 + (record._l_qseq + 1) / 2 + record._l_qseq;
    SEQAN_ASSERT_GEQ(remainingBytes, 0);

    // Translate file local rID into a global rID that is compatible with the context contigNames.
    if (record.rID >= 0 && !empty(context.translateFile2GlobalRefId))
        record.rID = context.translateFile2GlobalRefId[record.rID];
    if (record.rID >= 0)
        SEQAN_ASSERT_LT(static_cast<uint64_t>(record.rID), length(contigNames(context)));

    // ... the same for rNextId
    if (record.rNextId >= 0 && !empty(context.translateFile2GlobalRefId))
        record.rNextId = context.translateFile2GlobalRefId[record.rNextId];
    if (record.rNextId >= 0)
        SEQAN_ASSERT_LT(static_cast<uint64_t>(record.rNextId), length(contigNames(context)));

    // query name.
    resize(record.qName, record._l_qname - 1, Exact());
    arrayCopyForward(it, it + record._l_qname - 1, begin(record.qName, Standard()));
    it += record._l_qname;

    // cigar string.
    resize(record.cigar, record._n_cigar, Exact());
    static char const * CIGAR_MAPPING = "MIDNSHP=X*******";
    TCigarIter cigEnd = end(record.cigar, Standard());
    for (TCigarIter cig = begin(record.cigar, Standard()); cig != cigEnd; ++cig)
    {
        uint32_t opAndCnt;
        readRawPod(opAndCnt, it);
        SEQAN_ASSERT_LEQ(opAndCnt & 15, 8u);
        cig->operation = CIGAR_MAPPING[opAndCnt & 15];
        cig->count = opAndCnt >> 4;
    }

    // query sequence.
    resize(record.seq, record._l_qseq, Exact());
    TSeqIter sit = begin(record.seq, Standard());
    TSeqIter sitEnd = sit + (record._l_qseq & ~1);
    while (sit != sitEnd)
    {
        unsigned char ui = getValue(it);
        ++it;
        *sit = Iupac(ui >> 4);
        ++sit;
        *sit = Iupac(ui & 0x0f);
        ++sit;
    }
    if (record._l_qseq & 1)
        *sit++ = Iupac((uint8_t)*it++ >> 4);

    // phred quality
    resize(record.qual, record._l_qseq, Exact());
    // If qual is a sequence of 0xff (heuristic same as samtools: Only look at first byte) then we clear it, to get the
    // representation of '*';
    TQualIter qitEnd = end(record.qual, Standard());
    for (TQualIter qit = begin(record.qual, Standard()); qit != qitEnd;)
        *qit++ = '!' + *it++;
    if (!empty(record.qual) && static_cast<char>(record.qual[0] - '!') == '\xff')
        clear(record.qual);

    // tags
    resize(record.tags, remainingBytes, Exact());
    if (remainingBytes > 0)
        arrayCopyForward(it, it + remainingBytes, begin(record.tags, Standard()));
}

}  // namespace seqan2

#endif  // #ifndef INCLUDE_SEQAN_BAM_IO_READ_BAM_H_
