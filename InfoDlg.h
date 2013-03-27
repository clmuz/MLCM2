#pragma once


// диалоговое окно InfoDlg

class InfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~InfoDlg();

// Данные диалогового окна
	enum { IDD = Dialog_Info };
	void print(const CString &text);
	void setCalAndVal(const int &calType, const int &fitnType, const int &valType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	CString mInfo;
};
