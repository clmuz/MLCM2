#pragma once
#include "..\MlcmShell.h"
#include "Calibration.h"

//���������� SLS

class Calibration;

class SLS
{
public:
	SLS(MlcmShell *modelSh, Calibration *cal);
	//���������� ��������� SLS
	void setParams(const double &step, const int &lim, const int &cal0Type);
	//������ ��������� SLS
	void getParams(double &step, int &lim, int &cal0Type) const;
	//��������� ����������
	double doCalibration(const int &N, Element &bestElement);
private:
	//��������� ������� ������� �� ������ ���������� ������ point
	double countF(Element &point) const;
	//�������� MLCM
	MlcmShell *mModelSh;
	//������� ����� ����������. ������ �� ���� ����� ��� ����, ����� ��������� ��������� ���������� ����������
	Calibration *mCal;
	//���� ��� (������ �� 0.01 �� 0.05)
	double mStep;
	//������ ��� ��������, ���� ��������� ������ ������� �� ������� ������� �����������.
	//��������������, ��� ��� ���� ������ �������� ������� �������
	static const double mMax;
	//����� �� ���������� ��������
	int mLim;
	//��� ��������� ���������� ����������
	int m0Type;
};