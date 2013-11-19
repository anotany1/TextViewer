#pragma once

#include <vector>
#include "KeywordPosition.h"


using namespace std;


namespace Logic
{
    typedef vector<KeywordPosition> KeywordPositionVector;


    class SearchVector
    {
     //variables

    protected:
        CString keyword;
        CPoint point;
        long selPosition;

        vector<KeywordPosition> positions;


    private:
        //SearchVector( const SearchVector& );
    public:
        SearchVector( CString keyword, long selPosition, CPoint point );
        SearchVector( void );
        ~SearchVector( void );

    //Methods
    public:
        CString GetKeyword();
        void SetKeyword( CString keyword );
        void Add( KeywordPosition );
        void Add( long position );
        KeywordPositionVector::const_iterator GetVectorBegin();
        KeywordPositionVector::const_iterator GetVectorEnd();
        KeywordPosition Get( int index );
        int GetSize();
        CPoint GetPoint();
        long GetSelPosition();

    };


    typedef auto_ptr<SearchVector> SearchVectorPtr;
}