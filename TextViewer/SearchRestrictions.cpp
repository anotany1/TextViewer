#include "StdAfx.h"
#include "SearchRestrictions.h"

const int SearchRestrictions::MinKeywordLength = 3;
const int SearchRestrictions::MaxKeywordLength = 20;
const int SearchRestrictions::MaxSearchItemCount = 10;
const int SearchRestrictions::SearchArea = 500000000;

SearchRestrictions::SearchRestrictions(int searchArea, int maxSearchItemCount, int minKeywordLength, int maxKeywordLength)
{
        this->searchArea = Check( 1, SearchArea, searchArea );
        this->maxSearchItemCount = Check ( 1, MaxSearchItemCount, maxSearchItemCount );
        this->minKeywordLength = Check( 1, MinKeywordLength, minKeywordLength );
        this->maxKeywordLength = Check( this->minKeywordLength, MaxKeywordLength, maxKeywordLength );
}


SearchRestrictions::~SearchRestrictions(void)
{
}


int SearchRestrictions::Check( int l, int r, int value )
{
    if (value < l || value > r)
    {
        return r;
    }
    return value;
}
