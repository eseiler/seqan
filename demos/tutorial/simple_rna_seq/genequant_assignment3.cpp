#include <iostream>
#include <seqan/store.h>
#include <seqan/misc/interval_tree.h>
#include <seqan/parallel.h>

using namespace seqan2;


//![definitions]
// define used types
typedef FragmentStore<>                         TStore;
typedef Value<TStore::TAnnotationStore>::Type   TAnnotation;
typedef TAnnotation::TId                        TId;
typedef TAnnotation::TPos                       TPos;
typedef IntervalAndCargo<TPos, TId>             TInterval;
typedef IntervalTree<TPos, TId>                 TIntervalTree;
//![definitions]
//![definitions_end]
//
// 1. Load annotations and alignments from files
//
bool loadFiles(TStore & store, std::string const & annotationFileName,  std::string const & alignmentFileName)
{
    BamFileIn alignmentFile;
    if (!open(alignmentFile, alignmentFileName.c_str()))
    {
        std::cerr << "Couldn't open alignment file " << alignmentFileName << std::endl;
        return false;
    }
    std::cout << "Loading read alignments ..... " << std::flush;
    readRecords(store, alignmentFile);
    std::cout << "[" << length(store.alignedReadStore) << "]" << std::endl;

    // load annotations
    GffFileIn annotationFile;
    if (!open(annotationFile, toCString(annotationFileName)))
    {
        std::cerr << "Couldn't open annotation file" << annotationFileName << std::endl;
        return false;
    }
    std::cout << "Loading genome annotation ... " << std::flush;
    readRecords(store, annotationFile);
    std::cout << "[" << length(store.annotationStore) << "]" << std::endl;

    return true;
}

//
// 2. Extract intervals from gene annotations (grouped by contigId)
//
void extractGeneIntervals(String<String<TInterval> > & intervals, TStore const & store)
{
    // extract intervals from gene annotations (grouped by contigId)
    resize(intervals, length(store.contigStore));

    Iterator<TStore const, AnnotationTree<> >::Type it = begin(store, AnnotationTree<>());

    if (!goDown(it))
        return;

    do
    {
        SEQAN_ASSERT_EQ(getType(it), "gene");
        TPos beginPos = getAnnotation(it).beginPos;
        TPos endPos = getAnnotation(it).endPos;
        TId contigId = getAnnotation(it).contigId;

        if (beginPos > endPos)
            std::swap(beginPos, endPos);

        // insert forward-strand interval of the gene and its annotation id
        appendValue(intervals[contigId], TInterval(beginPos, endPos, value(it)));
    }
    while (goRight(it));
}

//![definitions_end]
//![yourcode]
//
// 3. Construct interval trees
//
void constructIntervalTrees(String<TIntervalTree> & intervalTrees,
                            String<String<TInterval> > & intervals)
{
    (void) intervalTrees;
    (void) intervals;
    // INSERT YOUR CODE HERE ...
    //
}
//![yourcode]

//![yourcode_end]
int main(int, char const **)
{
    TStore store;
//![yourcode_end]
//![main]
    String<String<TInterval> > intervals;
    String<TIntervalTree> intervalTrees;
//![main]

//![main_end]
    std::string annotationFileName = getAbsolutePath("demos/tutorial/simple_rna_seq/example.gtf");
    std::string alignmentFileName = getAbsolutePath("demos/tutorial/simple_rna_seq/example.sam");

    if (!loadFiles(store, annotationFileName, alignmentFileName))
        return 1;
//![main_end]

//![main2]
    extractGeneIntervals(intervals, store);
    constructIntervalTrees(intervalTrees, intervals);
//![main2]

//![main2_end]
    return 0;
}
//![main2_end]
