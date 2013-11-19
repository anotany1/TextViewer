#include "stdafx.h"
#include <TOM.h>
#include <Unknwn.h>
#include "Clock.h"
#include "SearchList.h"
#include "TVRichEdit.h"
#include "resource.h"


using namespace Logic;


static const IID IID_ITextViewerEdit = { 0x88F157F2, 0x51F0, 0x40E5, { 0x8F, 0x4B, 0x7D, 0x32, 0xEC, 0xB5, 0x5, 0x5F } };


TVRichEdit::TVRichEdit()
{
    EnableAutomation();
}

TVRichEdit::~TVRichEdit()
{
}


#pragma region MAPS

IMPLEMENT_DYNAMIC(TVRichEdit, CRichEditCtrl)

BEGIN_MESSAGE_MAP(TVRichEdit, CRichEditCtrl)
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONUP()
//    ON_NOTIFY_REFLECT(EN_SELCHANGE, &TVRichEdit::OnEnSelchange)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(TVRichEdit, CRichEditCtrl)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(TVRichEdit, CRichEditCtrl)
    INTERFACE_PART(TVRichEdit, IID_ITextViewerEdit, Dispatch)
END_INTERFACE_MAP()

#pragma endregion MAPS


#pragma region OldHighLightFunc
/*
DWORD WINAPI TextViewerEdit::ThreadFunc( LPVOID lpParam )
{
    TextViewerEdit* richEdit = static_cast<TextViewerEdit*>(lpParam);
    
    CString str = richEdit->GetSelText();
    FINDTEXTEXW findText;
    findText.lpstrText = str;
    findText.chrg.cpMin = 0;
    findText.chrg.cpMax = -1;

    CHARFORMAT2 charformat;
    ZeroMemory( &charformat, sizeof( charformat ) );
    charformat.cbSize = sizeof( charformat );
    charformat.dwMask = CFM_BACKCOLOR;
    charformat.crBackColor = RGB( 255, 40, 40 );

    richEdit->HideSelection(TRUE, FALSE);
    CPoint caret( richEdit->GetCaretPos() );
    CHARRANGE charRange;
    
    richEdit->GetSel( charRange );

    while ( richEdit->FindTextW( FR_DOWN, &findText ) != -1 )
    {
        richEdit->SetSel( findText.chrgText.cpMin, findText.chrgText.cpMax );
        richEdit->SetSelectionCharFormat( charformat );
        findText.chrg.cpMin =  findText.chrgText.cpMax;
    }
      
    richEdit->SetSel( charRange );
    richEdit->HideSelection( FALSE, FALSE );
    richEdit->SetOptions( ECOOP_AND, ECO_AUTOVSCROLL );
    return 1;
}
*/
#pragma endregion OldHighLightFunc

bool TVRichEdit::FindKeyword( SearchList *searchList, SearchRestrictions restrictions )
{
    FINDTEXTEXW findText, findTextFront;
    long upperItemMaxCount( 0 ), downItemMaxCount( 0 ), upperItemCount( 0 ), downItemCount( 0 );
    
    upperItemMaxCount = restrictions.maxSearchItemCount / 2;
    

    findTextFront.lpstrText = findText.lpstrText = searchList->GetKeyword();
    findTextFront.chrg.cpMin = findText.chrg.cpMin = searchList->GetSelPosition();


    if (searchList->GetSelPosition() - restrictions.searchArea < 0 )
        findTextFront.chrg.cpMax = 0; //begin of text
    else
        findTextFront.chrg.cpMax = searchList->GetSelPosition() - restrictions.searchArea;
    //Upper search
    while ( ( FindTextW( FR_MATCHCASE, &findTextFront ) != -1) && (searchList->GetSize() < upperItemMaxCount ) )
    {
        findTextFront.chrg.cpMin = findTextFront.chrgText.cpMin;
        searchList->FrontAdd( findTextFront.chrgText.cpMin );
        ++upperItemCount;
    }

    downItemMaxCount = restrictions.maxSearchItemCount - upperItemCount;

    if (searchList->GetSelPosition() + restrictions.searchArea > GetTextLength() )
        findText.chrg.cpMax = -1; //end of text
    else
        findText.chrg.cpMax = searchList->GetSelPosition() + restrictions.searchArea;

    //Down search
    while ( ( FindTextW( FR_DOWN | FR_MATCHCASE, &findText ) != -1) && (searchList->GetSize() < restrictions.maxSearchItemCount) )
    {
        findText.chrg.cpMin = findText.chrgText.cpMax;
        searchList->Add( findText.chrgText.cpMin );
        ++downItemCount;
    }

    
    if (downItemCount < downItemMaxCount)
    {
        while ( ( FindTextW( FR_MATCHCASE, &findTextFront ) != -1) && (searchList->GetSize() < restrictions.maxSearchItemCount) )
        {
            findTextFront.chrg.cpMin = findTextFront.chrgText.cpMin;
            searchList->FrontAdd( findTextFront.chrgText.cpMin );
            
        }
    }
    return true;
}

bool TVRichEdit::SetKeywordBackColor( SearchList *searchList, long color )
{
    SetReadOnly(FALSE);
    IUnknown *unknown;
    ::SendMessage( GetSafeHwnd(), EM_GETOLEINTERFACE, 0, ( LPARAM )&unknown );

    ITextDocument *textDocument;
    unknown->QueryInterface( &textDocument );
    textDocument->Freeze(NULL);

    GETTEXTLENGTHEX tl = { GTL_DEFAULT, 1200 }; 
    int length = ::SendMessageW( GetSafeHwnd(), EM_GETTEXTLENGTHEX, ( WPARAM )&tl, 0 ); 

    ITextFont * textFont;
    ITextRange * keywordRange;
    textDocument->Range(0, 0, &keywordRange);

    for (KeywordPositionList::const_iterator it( searchList->GetListBegin() ); it != searchList->GetListEnd(); ++it )
    {
        keywordRange->SetStart(it->position);
        keywordRange->SetEnd(it->position + searchList->GetKeyword().GetLength());
        keywordRange->GetFont(&textFont);
        textFont->SetBackColor( color ) ;
    }

    textDocument->Unfreeze(NULL);
    textDocument->Release();
    keywordRange->Release();
    textFont->Release();
    SetReadOnly(TRUE);
    return true;
}

SearchListPtr TVRichEdit::Search( CString keyword, long selPosition, CPoint point, SearchRestrictions searchRestrictions )
{
    SearchListPtr searchListPtr( new SearchList( keyword, selPosition, point ) );
    FindKeyword( searchListPtr.get(), searchRestrictions );
    
    return searchListPtr;
}


int TVRichEdit::GetCurrentLine()
{
    return SendMessage(EM_LINEFROMCHAR, -1, 0);
}

// TextViewerEdit message handlers

void TVRichEdit::OnLButtonDblClk( UINT nFlags, CPoint point )
{
    //CRichEditCtrl::OnLButtonDblClk( MK_LBUTTON, point );
    switch ( nFlags )
    {
    case( MK_LBUTTON | MK_CONTROL ):
        {
            int pos, pos_left, pos_right;
            pos = pos_right = pos_left = CharFromPos( point );
            
            SetSel( FindWordBreak( WB_LEFT, pos), FindWordBreak( WB_RIGHT, pos) );
            GetParent()->SendMessage( WM_LBUTTONDBLCLK, ID_TVRICH_EDIT_LBUTTONDBLCLK, MAKELPARAM( point.x, point.y ) );
            break;
        }
    case ( MK_LBUTTON ):
        {
            GetParent()->SendMessage( WM_LBUTTONDBLCLK, ID_TVRICH_EDIT_LBUTTONDBLCLK_WITH_CTRL, MAKELPARAM( point.x, point.y ) );
            CRichEditCtrl::OnLButtonDblClk( MK_LBUTTON, point );
            
            break;
        }
    }
}

void TVRichEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
    switch ( nFlags )
    {
    case( MK_CONTROL ):
        {
            GetParent()->SendMessage( WM_LBUTTONUP, ID_TVRICH_EDIT_LBTNUP_WITH_CTRL, MAKELPARAM( point.x, point.y ) );
            break;
        }
    case ( 0 ):
        {
            GetParent()->SendMessage( WM_LBUTTONUP, ID_TVRICH_EDIT_LBTNUP, MAKELPARAM( point.x, point.y ) );
            break;
        }
    }
}
