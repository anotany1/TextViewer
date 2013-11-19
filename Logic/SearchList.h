#pragma once

#include <list>
#include "KeywordPosition.h"


using namespace std;


namespace Logic
{
    typedef list<KeywordPosition> KeywordPositionList;


    class SearchList
    {
    public:
        SearchList( CString keyword, long selPosition, CPoint point);
        ~SearchList( void );
    private:
        SearchList( void );
    
    
    public:
        CString GetKeyword();
        void SetKeyword( CString keyword );
    
        void Add( KeywordPosition );
        void Add( long position );
        void FrontAdd( KeywordPosition keywordPosition );
        void FrontAdd( long position );

        KeywordPositionList::const_iterator GetListBegin();
        KeywordPositionList::const_iterator GetListEnd();

        int GetSize();
        CPoint GetPoint();
    
        long GetSelPosition();

    //variables

    protected:
        list<KeywordPosition> positions;

        CString keyword;
        CPoint point;
        long selPosition;
    };


    typedef auto_ptr<SearchList> SearchListPtr;
}