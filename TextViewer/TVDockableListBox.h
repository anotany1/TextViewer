#pragma once

#include "TVListBox.h"


class TVDockableListBox : public CDockablePane
{
    DECLARE_DYNAMIC(TVDockableListBox)

public:
    TVDockableListBox();
    virtual ~TVDockableListBox();


public:
    void AddString( CString str );
    void Clear();
    int GetSelection();
    void SetSelection(int index);


protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
public:
    afx_msg LRESULT OnUWMSelchange(WPARAM wParam, LPARAM lParam);
protected:
    DECLARE_MESSAGE_MAP()

protected:
    static const int ListBoxWidthCorrection;
    static const int ListBoxHieghtCorrection;

    TVListBox listBox;
};


