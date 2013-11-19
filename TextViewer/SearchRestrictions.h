#pragma once


struct SearchRestrictions
{
public:
    SearchRestrictions( int searchArea = SearchArea, 
        int maxSearchItemCount = MaxSearchItemCount, 
        int minKeywordLength = MinKeywordLength, 
        int maxKeywordLength = MaxKeywordLength );
    ~SearchRestrictions(void);

    

public:
    // Check borders restrictions, returns standard value(r) if check failed, returns value if check is not failed
    int Check(int l, int r, int value);

public:
    static const int SearchArea; // Max search area = cursor position +- SearchArea
    static const int MaxSearchItemCount;
    static const int MinKeywordLength;
    static const int MaxKeywordLength;

public:
    int searchArea, maxSearchItemCount, minKeywordLength, maxKeywordLength;
};

