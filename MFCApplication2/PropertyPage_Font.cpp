// PropertyPage_Font.cpp : implementation file
//

#include "pch.h"
#include "MFCApplication2.h"
#include "PropertyPage_Font.h"
#include "afxdialogex.h"


// PropertyPage_Font dialog

IMPLEMENT_DYNAMIC(PropertyPage_Font, CMFCPropertyPage)

PropertyPage_Font::PropertyPage_Font(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_DIALOG3)
	, m3_bold(FALSE)
	, m3_italic(FALSE)
	, m3_underLine(FALSE)
{

}

PropertyPage_Font::~PropertyPage_Font()
{
}

void PropertyPage_Font::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCFONTCOMBO1, m3_fontType);
	DDX_Check(pDX, IDC_CHECK1, m3_bold);
	DDX_Check(pDX, IDC_CHECK2, m3_italic);
	DDX_Check(pDX, IDC_CHECK3, m3_underLine);
}


BEGIN_MESSAGE_MAP(PropertyPage_Font, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_MFCFONTCOMBO1, &PropertyPage_Font::OnSelchangeMfcfontcombo1)
END_MESSAGE_MAP()


// PropertyPage_Font message handlers


void PropertyPage_Font::OnSelchangeMfcfontcombo1()
{
	int index = m3_fontType.GetCurSel();
	if (index != LB_ERR)
	{
		m3_fontType.GetLBText(index, font);
	}
}
