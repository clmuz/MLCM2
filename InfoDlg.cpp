// InfoDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MLCM-PROJECT.h"
#include "InfoDlg.h"
#include "afxdialogex.h"


// диалоговое окно InfoDlg

IMPLEMENT_DYNAMIC(InfoDlg, CDialogEx)

InfoDlg::InfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(InfoDlg::IDD, pParent)
	, mInfo(_T(""))
{

}

InfoDlg::~InfoDlg()
{
}

void InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, Static_Info, mInfo);
}

void InfoDlg::print(const CString &text)
{
	mInfo = text;
	DoModal();
}

BEGIN_MESSAGE_MAP(InfoDlg, CDialogEx)
END_MESSAGE_MAP()


// обработчики сообщений InfoDlg
