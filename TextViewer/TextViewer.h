#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif


#include "resource.h"       


using namespace Logic;


class CTextViewerApp : public CWinAppEx
{
public:
    CTextViewerApp();


public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
protected:
    virtual void PreLoadState();
    virtual void LoadCustomState();
    virtual void SaveCustomState();


public:
    afx_msg void OnAppAbout();
    afx_msg void OnAfxIdsAppTitle();
protected:
    DECLARE_MESSAGE_MAP()


public:
    BOOL  m_bHiColorIcons;
protected:
    HICON m_hIcon;
};

extern CTextViewerApp theApp;
