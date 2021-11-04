#pragma once


// PropertyPage_Border dialog

class PropertyPage_Border : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(PropertyPage_Border)

public:
	PropertyPage_Border(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PropertyPage_Border();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m2_lineType;
	CMFCColorButton m2_lineColor;
	afx_msg void OnSelchangeCombo1();
	int linetype;
};
