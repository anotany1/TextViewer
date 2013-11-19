#pragma once

#include "TVDockableListBox.h"
#include "TVRichEdit.h"
#include "SearchList.h"
#include "SearchVector.h"
#include <stack>


using namespace Logic;


class CChildView : public CWnd
{
public:
    CChildView();
    virtual ~CChildView();

public:
    bool OpenFile();
    void EditCopy();
    void Undo();
    SearchListPtr GetLastSearchList();
    int GetClearStackSize();
    bool IsColorMode();
    int GetTextSize();
    int GetCurrentPos();
    int GetCurrentLine();
    
    void SetHighlight( SearchVector *searchVector, long color = Colors[0] );
    void SetHighlight( SearchList *searchList, long color = Colors[0] );
    void Highlight( CPoint point, bool cleanHighlight );
    void ClearHighlight();
    
    void SetSelection( long pos, long length, CPoint point );

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Message handles

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()

//Variables

public:
    static const int MaxSelColorsCount;
    static const long Colors[];

protected:
    TVRichEdit m_tvrichEdit;

    SearchListPtr searchListPtr;
    std::stack<SearchList> clearStack;
    SearchRestrictions searchRestrictions;

    int currentSelColorCount;
    CString currentFilePath;

};

