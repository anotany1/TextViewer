#include "StdAfx.h"
#include "KeywordPosition.h"
#include "SearchList.h"


using namespace std;
using namespace Logic;


SearchList::SearchList( void )
{
    //positions = new vector<KeywordPosition>();
}

SearchList::SearchList( CString keyword, long selPosition, CPoint point )
{
    this->keyword = keyword;
    this->point = point;
    this->selPosition = selPosition;
}

SearchList::~SearchList( void )
{

}


CString SearchList::GetKeyword()
{
    return keyword;
}

void SearchList::SetKeyword( CString keyword )
{
    this->keyword = keyword;
}


void SearchList::Add( KeywordPosition position )
{
    positions.push_back(position);
}

void SearchList::Add( long position )
{
    KeywordPosition keywordPosition;
    keywordPosition.position = position;
    Add(keywordPosition);
}

void SearchList::FrontAdd( KeywordPosition position )
{
    positions.push_front(position);
}

void SearchList::FrontAdd( long position )
{
    KeywordPosition keywordPosition;
    keywordPosition.position = position;
    Add(keywordPosition);
}


KeywordPositionList::const_iterator SearchList::GetListBegin()
{
    return positions.cbegin();
}

KeywordPositionList::const_iterator SearchList::GetListEnd()
{
    return positions.cend();
}


int SearchList::GetSize()
{
    return positions.size();
}

CPoint SearchList::GetPoint()
{
    return point;
}

long SearchList::GetSelPosition()
{
    return selPosition;
}
