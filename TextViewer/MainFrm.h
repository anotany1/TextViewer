#pragma once

#include <vector>
#include <stack>
#include <deque>
#include "ChildView.h"
#include "TVRichEdit.h"
#include "SearchVector.h"


using namespace Logic;


class CMainFrame : public CFrameWndEx
{
    DECLARE_DYNAMIC( CMainFrame )

public:
    CMainFrame();
    virtual ~CMainFrame();


public:
    virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
    virtual BOOL LoadFrame( UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
                            CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL );

    void SetSearchBoxCaption( int value );

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif


public:
    afx_msg void OnFileOpen();
    afx_msg void OnEditCopy();
    afx_msg void OnEditUndo();
    afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
    afx_msg void OnUpdateStatuscolormode(CCmdUI *pCmdUI);
    afx_msg void OnUpdateIndicatorlinescount(CCmdUI *pCmdUI);
    afx_msg void OnUpdateIndicatorcurrentpos(CCmdUI *pCmdUI);
protected:
    afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
    afx_msg LRESULT OnToolbarCreateNew( WPARAM wp, LPARAM lp );
    
    afx_msg void OnViewCustomize();
    afx_msg LRESULT OnUWMReloadBoxes( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnUWMSearchListClick( WPARAM wParam, LPARAM lParam );
    afx_msg LRESULT OnUWMHistoryClick( WPARAM wParam, LPARAM lParam );

    DECLARE_MESSAGE_MAP()

//variables

public:
    static const int MaxHistoryVectorSize;
    static const int MinListBoxWidth;
    static const int MinListBoxHeight;

protected:
    CMFCMenuBar       m_wndMenuBar;
    CMFCToolBar       m_wndToolBar;
    CMFCStatusBar     m_wndStatusBar;
    CMFCToolBarImages m_UserImages;
    CChildView        m_wndView;
    TVRichEdit        m_richEdit;

    deque<SearchVector> historyDeque;
    TVDockableListBox searchListBox, historyListBox;

    CString currentFilePath;
    SearchVector currentSearchVector;
};


