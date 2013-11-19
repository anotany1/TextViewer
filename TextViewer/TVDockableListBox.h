#pragma once

#include "TVListBox.h"
// DockableSearchList

class TVDockableListBox : public CDockablePane
{
    DECLARE_DYNAMIC(TVDockableListBox)

// variables

    static const int ListBoxWidthCorrection;
    static const int ListBoxHieghtCorrection;

    TVListBox listBox;

//constructors, destructor

public:
    TVDockableListBox();
    virtual ~TVDockableListBox();

// methods

public:
    void AddString( CString str );
    void Clear();
    int GetSelection();
    void SetSelection(int index);

// message handlers

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
protected:
    afx_msg LRESULT OnUWMSelchange(WPARAM wParam, LPARAM lParam);
};


