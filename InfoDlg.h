#pragma once

// ���������� ���� ���������� (�� ��������� ����������� MessageBox ��-�� �����)

class InfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InfoDlg)

public:
	InfoDlg(CWnd* pParent = NULL);
	virtual ~InfoDlg();

// ������ ����������� ����
	enum { IDD = Dialog_Info };
	//����� �����
	void print(const CString &text);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
	DECLARE_MESSAGE_MAP()
private:
	//���������� ���������� ����
	CString mInfo;
};
