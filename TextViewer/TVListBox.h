#pragma once

class TVListBox : public CListBox
{
    DECLARE_DYNAMIC(TVListBox)

public:
    TVListBox();
    virtual ~TVListBox();


public:
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
    DECLARE_MESSAGE_MAP()
};


