#include "StdAfx.h"
#include "KeywordPosition.h"
#include "SearchVector.h"


using namespace std;
using namespace Logic;


SearchVector::SearchVector( void )
{
    //positions = new vector<KeywordPosition>();
}

SearchVector::SearchVector( CString keyword, long selPosition, CPoint point )
{
    this->keyword = keyword;
    this->point = point;
    this->selPosition = selPosition;
}

SearchVector::~SearchVector( void )
{

}


CString SearchVector::GetKeyword()
{
    return keyword;
}

void SearchVector::SetKeyword( CString keyword )
{
    this->keyword = keyword;
}

void SearchVector::Add( KeywordPosition position )
{
    positions.push_back(position);
}

void SearchVector::Add( long position )
{
    KeywordPosition keywordPosition;
    keywordPosition.position = position;
    Add(keywordPosition);
}

KeywordPositionVector::const_iterator SearchVector::GetVectorBegin()
{
    return positions.cbegin();
}

KeywordPositionVector::const_iterator SearchVector::GetVectorEnd()
{
    return positions.cend();
}

KeywordPosition SearchVector::Get( int index )
{
    return positions[index];
}

int SearchVector::GetSize()
{
    return positions.size();
}

CPoint SearchVector::GetPoint()
{
    return point;
}

long SearchVector::GetSelPosition()
{
    return selPosition;
}
