#include "stdafx.h"
#include "TextViewer.h"
#include "TVRichEdit.h"
#include "SearchList.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Logic;


const int CChildView::MaxSelColorsCount = 5;
const long CChildView::Colors[MaxSelColorsCount] = {
                                                    0x00D7FF, // Gold
                                                    0x32CD32, // LimeGreen
                                                    0x7436FF, // Tomato
                                                    0xFFFB00, // Light blue
                                                    0xF358DC};


CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP( CChildView, CWnd )
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_HOTKEY()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


bool CChildView::OpenFile()
{
    ClearHighlight();
    
    CFileDialog fileDialog( TRUE, NULL, L"*.*" );
    int result = fileDialog.DoModal();
    if( result == IDOK )
    {
        currentFilePath = fileDialog.GetPathName();
    }
    else return 0;

    SetWindowText( fileDialog.GetFileName() );

    TextHandler textHandler;
    if ( !textHandler.ReadFile( currentFilePath.GetString() ) )
    {
        AfxMessageBox( Can_Not_Open_File );
        return false;
    }
    if ( textHandler.GetText().c_str() == L"")
        return false;
    m_tvrichEdit.SetWindowTextW( textHandler.GetText().c_str() );
    return true;
}

void CChildView::EditCopy()
{
    m_tvrichEdit.Copy();
}

void CChildView::Undo()
{
    if (clearStack.size() == 0)
        return;
    m_tvrichEdit.SetKeywordBackColor( &clearStack.top(), RGB( 255, 255, 255 ) );
    clearStack.pop();
    --currentSelColorCount;
    if (clearStack.size() == 0)
        return;
    SetSelection( clearStack.top().GetSelPosition(),
                  clearStack.top().GetKeyword().GetLength(),
                  clearStack.top().GetPoint() );
}


SearchListPtr CChildView::GetLastSearchList()
{
    return searchListPtr;
}

int CChildView::GetClearStackSize()
{
    return clearStack.size();
}

bool CChildView::IsColorMode()
{
    return false;
}



void CChildView::SetHighlight( SearchVector *searchVector, long color /*= Colors[0]*/ )
{
    SearchList searchList ( searchVector->GetKeyword(), searchVector->GetSelPosition(), searchVector->GetPoint() );
    for (KeywordPositionVector::const_iterator it ( searchVector->GetVectorBegin() ); it != searchVector->GetVectorEnd(); ++it )
        searchList.Add( *it );

    SetHighlight( &searchList, color );
}

void CChildView::SetHighlight( SearchList *searchList, long color /*= Colors[0]*/ )
{
    m_tvrichEdit.SetKeywordBackColor( searchList, color );
    clearStack.push( *searchList );
    ++currentSelColorCount;
}

void CChildView::Highlight( CPoint point, bool clearHighlight = true )
{
    
    if ( !clearHighlight && currentSelColorCount == MaxSelColorsCount  )
        return;

    if ( clearHighlight )
        ClearHighlight();

    CString keyword = m_tvrichEdit.GetSelText();
    if ( keyword.GetLength() < searchRestrictions.minKeywordLength ) 
        return;

    long selStartPosition, selEndPosition;
    m_tvrichEdit.GetSel( selStartPosition, selEndPosition );
    searchListPtr = m_tvrichEdit.Search( keyword, selStartPosition, point, searchRestrictions );
    SetHighlight( searchListPtr.get() , Colors[currentSelColorCount] );

    GetParent()->SendMessageW( UWM_ReloadBoxes );
}

void CChildView::ClearHighlight()
{
    while ( !clearStack.empty() )
    {
        m_tvrichEdit.SetKeywordBackColor( &clearStack.top(), RGB( 255, 255, 255 ) );
        clearStack.pop();
    }
    currentSelColorCount = 0;
}


void CChildView::SetSelection( long pos, long length, CPoint point )
{
    m_tvrichEdit.SetCaretPos( point );
    m_tvrichEdit.SetSel( pos, pos + length );
}


int CChildView::GetTextSize()
{
    return m_tvrichEdit.GetTextLength();
}


int CChildView::GetCurrentPos()
{
    long start, end;
    m_tvrichEdit.GetSel( start, end );
    return start;
}

int CChildView::GetCurrentLine()
{
    return m_tvrichEdit.LineIndex();
}



BOOL CChildView::PreCreateWindow( CREATESTRUCT& cs ) 
{
    if ( !CWnd::PreCreateWindow( cs ) )
        return FALSE;
    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    //cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
        ::LoadCursor( NULL, IDC_ARROW ), reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL );

    return TRUE;
}

// Message handlers

afx_msg int CChildView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
        return -1;

    m_tvrichEdit.Create( WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ECO_AUTOVSCROLL | ECO_AUTOHSCROLL | WS_HSCROLL | ECO_READONLY | ES_NOHIDESEL | CS_DBLCLKS ,
        CRect( 0, 0, 0, 0 ), this, ID_TVRICH_EDIT_LBTNUP );

    OpenFile();

    currentSelColorCount = 0;
    //searchRestrictions = SearchRestrictions();

    RegisterHotKey( this->GetSafeHwnd(), IDH_ADD, MOD_CONTROL, 'A' );
    RegisterHotKey( this->GetSafeHwnd(), IDH_HIGHLIGHT, MOD_CONTROL, 'H' );

    return 0;
}

afx_msg void CChildView::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

    // TODO: Add your message handler code here

    // Do not call CWnd::OnPaint() for painting messages
}

afx_msg void CChildView::OnSize( UINT nType, int cx, int cy )
{
    CWnd::OnSize( nType, cx, cy );

    RECT rect;
    GetWindowRect( &rect );
    if ( GetSafeHwnd() == NULL )
    {
        return;
    }
    m_tvrichEdit.MoveWindow( 0, 
        0, 
        rect.right-rect.left-5, 
        rect.bottom - rect.top-5 );
}


void CChildView::OnLButtonDblClk( UINT nFlags, CPoint point )
{
    switch( nFlags )
    {
    case( ID_TVRICH_EDIT_LBTNUP ):
        {
            Highlight( point );
            break;
        }
    }
    
}

void CChildView::OnHotKey( UINT nHotKeyId, UINT nKey1, UINT nKey2 )
{
    switch ( nKey1 )
    {
    case( MOD_CONTROL ):
        {
            switch ( nKey2 )
            {
            case( 'H' ):
                {
                    Highlight( m_tvrichEdit.GetCaretPos() );
                    break;
                }
            }

            break;
        }
    }

    CWnd::OnHotKey( nHotKeyId, nKey1, nKey2 );
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
    switch( nFlags )
    {
    case( ID_TVRICH_EDIT_LBTNUP ):
        {
            Highlight( point, true );
            break;
        }
    case( ID_TVRICH_EDIT_LBTNUP_WITH_CTRL ):
        {
            Highlight( point, false );
            break;
        }
    }

    CWnd::OnLButtonUp(nFlags, point);
}

