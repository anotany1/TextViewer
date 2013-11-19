#include "stdafx.h"
#include "TextViewer.h"
#include "TVRichEdit.h"
#include "TVDockableListBox.h"
#include "SearchList.h"
#include "SearchVector.h"
#include "MainFrm.h"
#include "resource.h"


using namespace Logic;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int  iMaxUserToolbars = 10;


const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;
const int CMainFrame::MaxHistoryVectorSize = 50;
const int CMainFrame::MinListBoxWidth = 100;
const int CMainFrame::MinListBoxHeight = 100;



IMPLEMENT_DYNAMIC( CMainFrame, CFrameWndEx )
BEGIN_MESSAGE_MAP( CMainFrame, CFrameWndEx )
    ON_WM_CREATE()
    ON_COMMAND( ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize )
    ON_REGISTERED_MESSAGE( AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew )
    ON_MESSAGE( UWM_HistoryListClick, &CMainFrame::OnUWMHistoryClick )
    ON_MESSAGE( UWM_ReloadBoxes, &CMainFrame::OnUWMReloadBoxes )
    ON_MESSAGE( UWM_SearchListClick, &CMainFrame::OnUWMSearchListClick )
    ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
    ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
    ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMainFrame::OnUpdateEditUndo)
    ON_UPDATE_COMMAND_UI(ID_IndicatorColorMode, &CMainFrame::OnUpdateStatuscolormode)
    ON_UPDATE_COMMAND_UI(ID_IndicatorSymbolsCount, &CMainFrame::OnUpdateIndicatorlinescount)
    ON_UPDATE_COMMAND_UI(ID_IndicatorCurrentPos, &CMainFrame::OnUpdateIndicatorcurrentpos)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,
    ID_IndicatorCurrentPos,// status line indicator
    ID_IndicatorSymbolsCount,
    ID_IndicatorColorMode,
    //ID_INDICATOR_CAPS,
    //ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

//Message handlers

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
    if( !CFrameWndEx::PreCreateWindow(cs) )
        return FALSE;

    cs.style &= ~FWS_ADDTOTITLE;
    cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.lpszClass = AfxRegisterWndClass(0);
    return TRUE;
}

BOOL CMainFrame::LoadFrame( UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext ) 
{
    if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
    {
        return FALSE;
    }

    BOOL bNameValid;
    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);

    for (int i = 0; i < iMaxUserToolbars; i ++)
    {
        CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
        if (pUserToolbar != NULL)
        {
            pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
        }
    }

    return TRUE;
}

void CMainFrame::SetSearchBoxCaption( int value )
{
    CString caption;
    caption.LoadStringW( SearchString );
    caption.Format(caption + L" %d", value );
    searchListBox.SetWindowTextW( caption );
}


afx_msg int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if ( CFrameWndEx::OnCreate( lpCreateStruct ) == -1 )
        return -1;

    BOOL bNameValid;

    // set the visual manager used to draw all user interface elements
    CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerWindows ) );

    if ( !m_wndMenuBar.Create( this ) )
    {
        TRACE0( "Failed to create menubar\n" );
        return -1;      // fail to create
    }

    m_wndMenuBar.SetPaneStyle( m_wndMenuBar.GetPaneStyle(  ) | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY );

    CMFCPopupMenu::SetForceMenuFocus( FALSE );

    // create a view to occupy the client area of the frame
    if ( !m_wndView.Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 500, 500 ), this, AFX_IDW_PANE_FIRST, NULL ) )
    {
        TRACE0( "Failed to create view window\n" );
        return -1;
    }

    if ( !m_wndToolBar.CreateEx( this, 
        TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
        !m_wndToolBar.LoadToolBar( theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME ) )
    {
        TRACE0( "Failed to create toolbar\n" );
        return -1;      // fail to create
    }

    CDockingManager::SetDockingMode( DT_SMART );
    EnableDocking( CBRS_ALIGN_ANY );
    EnableAutoHidePanes( CBRS_ALIGN_ANY );
    

    CString strToolBarName;
    bNameValid = strToolBarName.LoadString( IDS_TOOLBAR_STANDARD );
    ASSERT( bNameValid );
    m_wndToolBar.SetWindowText( strToolBarName );

    CString strCustomize;
    bNameValid = strCustomize.LoadString( IDS_TOOLBAR_CUSTOMIZE );
    ASSERT( bNameValid );
    m_wndToolBar.EnableCustomizeButton( TRUE, ID_VIEW_CUSTOMIZE, strCustomize );

    // Allow user-defined toolbars operations:
    InitUserToolbars( NULL, uiFirstUserToolBarId, uiLastUserToolBarId );

    if ( !m_wndStatusBar.Create( this ) )
    {
        TRACE0( "Failed to create status bar\n" );
        return -1;      // fail to create
    }
    m_wndStatusBar.SetIndicators( indicators, sizeof( indicators )/sizeof( UINT ) );

    m_wndMenuBar.EnableDocking( CBRS_ALIGN_ANY );
    DockPane( &m_wndMenuBar );
    m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
    DockPane( &m_wndToolBar );

    // Enable toolbar and docking window menu replacement
    EnablePaneMenu( TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR );

    // enable quick ( Alt+drag ) toolbar customization
    CMFCToolBar::EnableQuickCustomization(  );

    if ( CMFCToolBar::GetUserImages(  ) == NULL )
    {
        if ( m_UserImages.Load( _T( ".\\UserImages.bmp" ) ) )
        {
            CMFCToolBar::SetUserImages( &m_UserImages );
        }
    }


    // enable menu personalization ( most-recently used commands )
    // TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
    CList<UINT, UINT> lstBasicCommands;
    lstBasicCommands.AddTail( ID_FILE_OPEN );
    lstBasicCommands.AddTail( ID_EDIT_COPY );
    lstBasicCommands.AddTail( ID_EDIT_UNDO );
    lstBasicCommands.AddTail( ID_APP_ABOUT );
    lstBasicCommands.AddTail( ID_APP_EXIT );
    lstBasicCommands.AddTail( ID_VIEW_STATUS_BAR );
    lstBasicCommands.AddTail( ID_VIEW_TOOLBAR );
    CMFCToolBar::SetBasicCommands( lstBasicCommands );
    
    if ( !searchListBox.Create(  L"Search results", this, CRect(  0, 0, 0, 0  ), TRUE, ID_SearchListBox, 
                       WS_VISIBLE | WS_CHILD | WS_BORDER | CBRS_RIGHT | CBRS_FLOAT_MULTI  ) )
    {
        TRACE0( "Failed to create searchListBox\n" );
        return -1;
    }
    searchListBox.SetMinSize( CSize( MinListBoxWidth, MinListBoxHeight) );

    DWORD dwStyle = searchListBox.GetControlBarStyle();
    dwStyle &= ~( AFX_CBRS_CLOSE  );
    searchListBox.SetControlBarStyle( dwStyle );


    if ( !historyListBox.Create( L"History", this, CRect( 0, 0, 0, 0 ), TRUE, ID_HistoryListBox, 
        WS_VISIBLE | WS_CHILD | WS_BORDER | CBRS_RIGHT ) )
    {
        TRACE0( "Failed to create historyListBox\n" );
        return -1;
    }
    historyListBox.SetMinSize( CSize( MinListBoxWidth, MinListBoxHeight) );


    searchListBox.EnableDocking( CBRS_ALIGN_ANY );
    historyListBox.EnableDocking( CBRS_ALIGN_ANY );
    //searchListBox.SetAutoHideMode(TRUE, CBRS_RIGHT, NULL, TRUE);
    //historyListBox.SetAutoHideMode(TRUE, CBRS_RIGHT, NULL, FALSE);
    //historyListBox.ToggleAutoHide();
    DockPane( &historyListBox );
    DockPane( &searchListBox );


    return 0;
}

afx_msg LRESULT CMainFrame::OnToolbarCreateNew( WPARAM wp,LPARAM lp )
{
    LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
    if (lres == 0)
    {
        return 0;
    }

    CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
    ASSERT_VALID(pUserToolbar);

    BOOL bNameValid;
    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);

    pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
    return lres;
}


afx_msg void CMainFrame::OnViewCustomize()
{
    CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
    pDlgCust->EnableUserDefinedToolbars();
    pDlgCust->Create();
}

afx_msg LRESULT CMainFrame::OnUWMReloadBoxes( WPARAM wParam, LPARAM lParam )
{
    SearchListPtr ptr = m_wndView.GetLastSearchList();
    if ( !ptr.get() )
        return 0;
    CString str;
    searchListBox.Clear();
    SearchVector searchVector( ptr->GetKeyword(), ptr->GetSelPosition(), ptr->GetPoint() );
    for (KeywordPositionList::const_iterator it ( ptr->GetListBegin() ) ; it != ptr->GetListEnd(); ++it )
    {
        str.Format( L"%ld", it->position );
        searchListBox.AddString( str );
        searchVector.Add( *it );
    }
    currentSearchVector = searchVector;

    historyListBox.Clear();
    historyDeque.push_back( searchVector );
    if (historyDeque.size() == MaxHistoryVectorSize)
        historyDeque.pop_front();
    for (deque<SearchVector>::reverse_iterator it ( historyDeque.rbegin() ); it != historyDeque.rend(); ++it )
    {
        historyListBox.AddString( it->GetKeyword() );
    }
    historyListBox.SetSelection(0);

    SetSearchBoxCaption( ptr->GetSize() );

    return 0;
}

afx_msg LRESULT CMainFrame::OnUWMHistoryClick( WPARAM wParam, LPARAM lParam )
{
    int index = (int)historyDeque.size() - (int)lParam  - 1;
    SearchList searchList ( historyDeque[index].GetKeyword(), historyDeque[index].GetSelPosition(), historyDeque[index].GetPoint() );
    searchListBox.Clear();
    CString str;
    for ( KeywordPositionVector::const_iterator it ( historyDeque[index].GetVectorBegin() ); it != historyDeque[index].GetVectorEnd(); ++it )
    {
        searchList.Add( *it );
        str.Format( L"%ld", it->position ) ;
        searchListBox.AddString( str );
    }
    currentSearchVector = historyDeque[index];
    
    m_wndView.ClearHighlight();
    m_wndView.SetHighlight( &searchList );

    SetSearchBoxCaption( currentSearchVector.GetSize() );
    m_wndView.SetSelection( currentSearchVector.GetSelPosition(), 
                            currentSearchVector.GetKeyword().GetLength(),
                            currentSearchVector.GetPoint() );
    
    return 0;
}

afx_msg LRESULT CMainFrame::OnUWMSearchListClick( WPARAM wParam, LPARAM lParam )
{
    if ( lParam < 0 || lParam >= currentSearchVector.GetSize() )
        return -1;

    m_wndView.SetSelection( currentSearchVector.Get(lParam).position, 
                            currentSearchVector.GetKeyword().GetLength(), 
                            currentSearchVector.GetPoint() );
    
    return 0;
}


afx_msg void CMainFrame::OnFileOpen()
{
    historyDeque.clear();
    historyListBox.Clear();
    searchListBox.Clear();
    m_wndView.OpenFile();
}

afx_msg void CMainFrame::OnEditCopy()
{
    m_wndView.EditCopy();
}

afx_msg void CMainFrame::OnEditUndo()
{
    m_wndView.Undo();
}


void CMainFrame::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
    if ( m_wndView.GetClearStackSize() > 1 )
        pCmdUI->Enable(TRUE);
    else pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateStatuscolormode(CCmdUI *pCmdUI)
{
    CString str;
    pCmdUI->Enable( FALSE );
    pCmdUI->SetText( L"Multi color mode" );
}


void CMainFrame::OnUpdateIndicatorlinescount(CCmdUI *pCmdUI)
{
    CString str;
    pCmdUI->Enable( TRUE );
    str.Format(L"Symbols count: %d", m_wndView.GetTextSize() );
    pCmdUI->SetText( str );
}


void CMainFrame::OnUpdateIndicatorcurrentpos(CCmdUI *pCmdUI)
{
    CString str;
    pCmdUI->Enable( TRUE );
    str.Format(L"Current position: %d", m_wndView.GetCurrentPos() );
    pCmdUI->SetText( str );
}
