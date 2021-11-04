#include "pch.h"
#include "PropertySetting.h"

PropertySetting::PropertySetting()
{
	AddPage(&page1);
	AddPage(&page2);
	AddPage(&page3);
};

BOOL PropertySetting::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	SetWindowText(TEXT("Property Setting"));

	CWnd* pWnd = GetDlgItem(IDHELP);
	pWnd->ShowWindow(SW_HIDE);

	return bResult;
}