#pragma once


// PropertyPage_Font dialog

class PropertyPage_Font : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(PropertyPage_Font)

public:
	PropertyPage_Font(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PropertyPage_Font();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMFCFontComboBox m3_fontType;
	BOOL m3_bold;
	BOOL m3_italic;
	BOOL m3_underLine;
	afx_msg void OnSelchangeMfcfontcombo1();
	CString font;
};
