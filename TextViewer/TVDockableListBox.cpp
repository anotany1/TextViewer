#include "stdafx.h"
#include "TVDockableListBox.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC( TVDockableListBox, CDockablePane )


TVDockableListBox::TVDockableListBox()
{

}

TVDockableListBox::~TVDockableListBox()
{
}


BEGIN_MESSAGE_MAP( TVDockableListBox, CDockablePane )
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_MESSAGE( UWM_SELCHANGE, &TVDockableListBox::OnUWMSelchange )
END_MESSAGE_MAP()


void TVDockableListBox::AddString( CString str )
{
    listBox.AddString( str );
}

void TVDockableListBox::Clear()
{
    listBox.ResetContent();
}

int TVDockableListBox::GetSelection()
{
    return listBox.GetCurSel();
}

void TVDockableListBox::SetSelection( int index )
{
    listBox.SetSel( index );
}

// DockableSearchList message handlers

afx_msg int TVDockableListBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if ( CDockablePane::OnCreate( lpCreateStruct ) == -1 )
        return -1;

    listBox.Create( 
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL , 
        CRect( 0, 0, 0, 0 ), this, ID_TVListBox );

    if ( !listBox )
        return -1;
    return 0;
}

const int TVDockableListBox::ListBoxHieghtCorrection = -10;
const int TVDockableListBox::ListBoxWidthCorrection = 0;
afx_msg void TVDockableListBox::OnSize( UINT nType, int cx, int cy )
{
    CDockablePane::OnSize( nType, cx, cy);

    if ( GetSafeHwnd() == NULL )
    {
        return;
    }

    CRect rect;
    this->GetWindowRect( &rect );

    if (this->IsDocked() == TRUE)
    {
        listBox.MoveWindow( 0, 0, 
            rect.Width() + ListBoxWidthCorrection, 
            rect.Height() + ListBoxHieghtCorrection );
        return;
    }
    listBox.MoveWindow( 0, 0, rect.Width(), rect.Height() );
}

afx_msg void TVDockableListBox::OnItemchangedList( NMHDR* pNMHDR, LRESULT* pResult ) 
{
    NM_LISTVIEW* pNMListView = ( NM_LISTVIEW* )pNMHDR;

    if ( ( pNMListView->uChanged & LVIF_STATE ) 
        && ( pNMListView->uNewState & LVNI_SELECTED ) )
    {
        // do stuff...
    }
 }

afx_msg LRESULT TVDockableListBox::OnUWMSelchange( WPARAM wParam, LPARAM lParam )
{
    switch ( wParam )
    {
    case( ID_TVListBox ):
        {
            switch ( GetDlgCtrlID() )
            {
                case( ID_HistoryListBox ):
                {
                    GetParent()->PostMessage( UWM_HistoryListClick, ID_TVListBox, lParam );
                    break;
                }
                case( ID_SearchListBox ):
                {
                    GetParent()->PostMessage( UWM_SearchListClick, ID_TVListBox, lParam );
                    break;
                }
            }
            break;
        }
    }
    
    return 0;
}