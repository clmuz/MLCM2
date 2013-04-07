#pragma once

// диалоговое окно информации (не использую стандартный MessageBox из-за звука)

class InfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = NULL);
	virtual ~InfoDlg();

// Данные диалогового окна
	enum { IDD = Dialog_Info };
	//Вывод техта
	void print(const CString &text);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
private:
	//переменная текстового поля
	CString mInfo;
};
