//![initialization]
#include <iostream>
#include <seqan/find.h>

using namespace seqan2;

int main()
{
    CharString haystack = "Simon, send more money!";
    CharString needle = "more";
//![initialization]

//![option]
    Finder<CharString> finder(haystack);
    Pattern<CharString, Myers<> > pattern(needle);
    while (find(finder, pattern, -2))
        while (findBegin(finder, pattern, getScore(pattern)))
            std::cout << '[' << beginPosition(finder) << ',' << endPosition(finder) << ")\t" << infix(finder) << std::endl;

    return 0;
}
//![option]
