#pragma once


// ���������� ���� InfoDlg

class InfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = NULL);   // ����������� �����������
	virtual ~InfoDlg();

// ������ ����������� ����
	enum { IDD = Dialog_Info };
	void print(const CString &text);
	void setCalAndVal(const int &calType, const int &fitnType, const int &valType);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	CString mInfo;
};
