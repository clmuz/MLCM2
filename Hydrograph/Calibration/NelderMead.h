#pragma once
#include <vector>
#include "..\MlcmShell.h"
#include "..\Element.h"

//���������� �������-����

class NelderMead
{
public:
	explicit NelderMead(MlcmShell *modelSh);
	//��������� ���������� �� ������ ���������� ���������
	double doCalibration(const int &N, Element &nowElement);
	//��������� ���������� ��� ���������� ��������� ����������
	double doComplexCalibration(const int &N, Element &nowElement);
	//���������� ������������ ���������� ��������� ��� ��������� ����������
	void setKoeffs(const double &koeff1, const double &koeff2);
	//���������� ������������ ���������� ��� ����������������� ����������
	void setKoeffs(const vector<double> &koeffs);
	//���������� ������� �������� � ����� �� ���������� ��������
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	//������ ������� �������� � ����� �� ���������� ��������
	void getNMParams(double &stop, int &iter) const;
	//����� ���� ������������� ����������
	int getNMKoeffs(vector<double> &koeffs) const;
private:
	//��������� ���������� �� ���������
	double doNelderMead(Element &nowElement, vector<Element*> &elements) const;
	//������� �������� �� �������������
	void doFirstSimplex(const double &koeff1, const double &koeff2, vector<Element*> &elements) const;
	//��������� ������� ������� �� ������ ���������� ������ point
	double countF(const Element &point) const;
	//�������� �� ������� �������� (�������� �������� ������� ������� � �������� ���������)
	bool isLim(const vector<double> &Fs) const;
	//swap ������ ��������� � �� �������� �������
	void swap(const int &i, const int &j, vector<Element*> &elements, vector<double> &Fs) const;
	//������������ �������-����
	static const double mAlpha, mBetta, mGamma;
	//������ ��� ��������, ���� ��������� ������ ������� �� ������� ������� �����������.
	//��������������, ��� ��� ���� ������ �������� ������� �������
	static const double mMax;
	//������� �������� - ��� �������� �� �������� �������� ������� �������
	double mE;
	//������������ ��������� ��� ��������� ����������
	double mKoeff1, mKoeff2;
	//���������� �����
	int mN;
	//���������� ������ ���������
	int mSimplexN;
	//���������� ���������� � ����������������� ����������
	int mComplexNum;
	//����� �� ���������� ��������
	int mLimit;
	MlcmShell *mModelSh;
	//������������ ���������� ��� ����������������� ����������
	vector<double> mKoeffs;
};
