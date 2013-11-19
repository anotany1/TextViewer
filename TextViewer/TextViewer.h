// TextViewer.h : main header file for the TextViewer application
#pragma once


#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif


#include "resource.h"       


using namespace Logic;


class CTextViewerApp : public CWinAppEx
{
protected:
    HICON m_hIcon;
public:
    BOOL  m_bHiColorIcons;


public:
    CTextViewerApp();


//Methods
protected:
    virtual void PreLoadState();
    virtual void LoadCustomState();
    virtual void SaveCustomState();
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();


    DECLARE_MESSAGE_MAP()
    afx_msg void OnAppAbout();
    afx_msg void OnAfxIdsAppTitle();
};

extern CTextViewerApp theApp;
