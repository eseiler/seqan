//![header]
#include <seqan/sequence.h>
#include <seqan/stream.h>
#include <seqan/seeds.h>

using namespace seqan2;

int main()
{
//![header]
//![example]
    // The horizontal and vertical sequence (subject and query sequences).
    CharString seqH = "The quick BROWN fox jumped again!";
    CharString seqV =     "thick BROWNIES for me!";
    // Create and print the seed sequence.
    Seed<Simple> seed(11, 7, 14, 10);
    std::cout << "original\n"
              << "seedH: " << infix(seqH, beginPositionH(seed),
                          endPositionH(seed)) << "\n"
              << "seedV: " << infix(seqV, beginPositionV(seed),
                          endPositionV(seed)) << "\n";

    // Perform match extension.
    extendSeed(seed, seqH, seqV, EXTEND_RIGHT, MatchExtend());
    // Print the resulting seed.
    std::cout << "result\n"
              << "seedH: " << infix(seqH, beginPositionH(seed),
                          endPositionH(seed)) << "\n"
              << "seedV: " << infix(seqV, beginPositionV(seed),
                          endPositionV(seed)) << "\n";
//![example]

//![footer]
    return 0;
}
//![footer]
