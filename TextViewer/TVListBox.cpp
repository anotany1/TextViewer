#include "stdafx.h"
#include "TVListBox.h"
#include "Resource.h"


IMPLEMENT_DYNAMIC( TVListBox, CListBox )


TVListBox::TVListBox()
{

}

TVListBox::~TVListBox()
{
}


BEGIN_MESSAGE_MAP( TVListBox, CListBox )
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// TVListBox message handlers

void TVListBox::OnLButtonDblClk( UINT nFlags, CPoint point )
{
    BOOL outside = TRUE;
    UINT item = CListBox::ItemFromPoint( point, outside );
    if( outside == FALSE ){
        GetParent()->SendMessageW(UWM_SELCHANGE, GetDlgCtrlID(), GetCurSel());
    }

    CListBox::OnLButtonDblClk(nFlags, point);
}