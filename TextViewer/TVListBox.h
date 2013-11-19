#pragma once

class TVListBox : public CListBox
{
    DECLARE_DYNAMIC(TVListBox)

public:
    TVListBox();
    virtual ~TVListBox();

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


