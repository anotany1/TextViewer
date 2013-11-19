#pragma once
struct SearchRestrictions
{
    static const int SearchArea; //Max search area = cursor position +- SearchArea
    static const int MaxSearchItemCount;
    static const int MinKeywordLength;
    static const int MaxKeywordLength;
    
    int searchArea, maxSearchItemCount, minKeywordLength, maxKeywordLength;

public:
    SearchRestrictions( int searchArea = SearchArea, 
        int maxSearchItemCount = MaxSearchItemCount, 
        int minKeywordLength = MinKeywordLength, 
        int maxKeywordLength = MaxKeywordLength );
    
    ~SearchRestrictions(void);

    //Check borders restrictions, returns standard value(r) if check failed, returns value if check is not failed
    int Check(int l, int r, int value);
};

