#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TextViewer.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CTextViewerApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &CTextViewerApp::OnAppAbout)
    ON_COMMAND(AFX_IDS_APP_TITLE, &CTextViewerApp::OnAfxIdsAppTitle)
END_MESSAGE_MAP()


// CTextViewerApp construction

CTextViewerApp::CTextViewerApp()
{
    m_bHiColorIcons = TRUE;

    // TODO: replace application ID string below with unique ID string; recommended
    // format for string is CompanyName.ProductName.SubProduct.VersionInformation
    SetAppID(_T("TextViewer.AppID.NoVersion"));
}

// The one and only CTextViewerApp object
CTextViewerApp theApp;


// CTextViewerApp initialization

BOOL CTextViewerApp::InitInstance()
{
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinAppEx::InitInstance();


    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();

    EnableTaskbarInteraction(FALSE);

    // AfxInitRichEdit2() is required to use RichEdit control    
    // AfxInitRichEdit2();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));


    InitContextMenuManager();

    InitKeyboardManager();

    InitTooltipManager();
    CMFCToolTipInfo ttParams;
    ttParams.m_bVislManagerTheme = TRUE;
    theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
        RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

    // To create the main window, this code creates a new frame window
    // object and then sets it as the application's main window object
    CMainFrame* pFrame = new CMainFrame;
    if (!pFrame)
        return FALSE;
    m_pMainWnd = pFrame;
    // create and load the frame with its resources
    pFrame->LoadFrame(IDR_MAINFRAME,
        WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
        NULL);

    // The one and only window has been initialized, so show and update it
    pFrame->ShowWindow(SW_SHOW);
    pFrame->UpdateWindow();
    // call DragAcceptFiles only if there's a suffix
    //  In an SDI app, this should occur after ProcessShellCommand
    return TRUE;
}

int CTextViewerApp::ExitInstance()
{
    //TODO: handle additional resources you may have added
    AfxOleTerm(FALSE);

    return CWinAppEx::ExitInstance();
}


class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonUp( UINT nFlags, CPoint point);
};


CAboutDlg::CAboutDlg() : CDialogEx( CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange( CDataExchange* pDX)
{
    CDialogEx::DoDataExchange( pDX);
}

BEGIN_MESSAGE_MAP( CAboutDlg, CDialogEx)
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// App command to run the dialog
void CTextViewerApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal( );
}


// CTextViewerApp customization load/save methods

void CTextViewerApp::PreLoadState()
{
    BOOL bNameValid;
    CString strName;
    bNameValid = strName.LoadString( IDS_EDIT_MENU );
    ASSERT( bNameValid );
    GetContextMenuManager()->AddMenu( strName, IDR_POPUP_EDIT );
}

void CTextViewerApp::LoadCustomState()
{
}

void CTextViewerApp::SaveCustomState()
{
}


// CTextViewerApp message handlers

void CAboutDlg::OnLButtonUp( UINT nFlags, CPoint point )
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnLButtonUp( nFlags, point );
}


void CTextViewerApp::OnAfxIdsAppTitle()
{
    m_hIcon = LoadIcon( IDR_MAINFRAME );
	m_pMainWnd->SetIcon(m_hIcon, FALSE);
}
