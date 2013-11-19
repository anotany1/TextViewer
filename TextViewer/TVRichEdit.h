#pragma once

#include <list>
#include "SearchList.h"
#include "SearchRestrictions.h"

using namespace Logic;

class TVRichEdit : public CRichEditCtrl
{
public:
    TVRichEdit();
    virtual ~TVRichEdit();


protected:
    //static DWORD WINAPI ThreadFunc(LPVOID lpParam);
public:
    bool FindKeyword( SearchList *searchList, SearchRestrictions restrictions );
    bool SetKeywordBackColor( SearchList *searchList, long color );
    SearchListPtr Search( CString keyword, long selPosition, CPoint point, SearchRestrictions searchRestrictions );
    int GetCurrentLine();

//Message handlers

public:
    virtual void OnFinalRelease();
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
    DECLARE_DYNAMIC(TVRichEdit)
    DECLARE_MESSAGE_MAP()
    DECLARE_DISPATCH_MAP()
    DECLARE_INTERFACE_MAP()

//    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual HRESULT accSelect(long flagsSelect, VARIANT varChild);
    afx_msg void OnEnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


