#pragma once


// PropertyPage_Color dialog

class PropertyPage_Color : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(PropertyPage_Color)

public:
	PropertyPage_Color(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PropertyPage_Color();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m1_showBK;
	CMFCColorButton m1_forceColor;
	CMFCColorButton m1_bkColor;
};
