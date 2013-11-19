#pragma once

#include <list>
#include "SearchList.h"
#include "SearchRestrictions.h"

using namespace Logic;

class TVRichEdit : public CRichEditCtrl
{
    DECLARE_DYNAMIC(TVRichEdit)

public:
    TVRichEdit();
    virtual ~TVRichEdit();


public:
    bool FindKeyword( SearchList *searchList, SearchRestrictions restrictions );
    bool SetKeywordBackColor( SearchList *searchList, long color );
    SearchListPtr Search( CString keyword, long selPosition, CPoint point, SearchRestrictions searchRestrictions );
    int GetCurrentLine();

public:
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
    DECLARE_MESSAGE_MAP()
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()
};


