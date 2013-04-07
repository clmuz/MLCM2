#pragma once
#include "..\MlcmShell.h"
#include "NelderMead.h"
#include "SLS.h"
#include "BruteForce.h"
#include <vector>

//�����, ����������� ��������� ����������
//0 - ������-��� �� ������ ���������, 1 - ����������������� ������-���, 2 - SLS, 3 - �������

class SLS;

class Calibration
{
public:
	Calibration(const int &calibrationType, MlcmShell *MlcmSh);
	explicit Calibration(MlcmShell *MlcmSh);
	~Calibration();
	//��������� ����������
	double doCalibration();
	//��������� ���������� ��� N �����, � ����� ���������� calType
	double doCalStep(const int &N, Element &bestElement, int calType = -1);
	//���������� ��� ����������
	void setCalibrationType(const int &calType);
	//���������� ������� �������� � ����� �� ���������� �������� � �������-����
	void setNMStopAndLim(const double &NMStop, const int &NMLim);
	//���������� ������������ ��������� ��� ���������������� �������-����
	void setNMKoeffs(const double &koeff1, const double &koeff2);
	//���������� ������������ ���������� ��� ���������������� �������-����
	void setNMKoeffs(const vector<double> &koeffs);
	//���������� ���������� ����� � �������� � ��������
	void setBFParams(const int &stepsNum, const int &iterNum);
	//���������� ���, ����� �� ���������� �������� � ��� ��������������� ���������� ��� SLS
	void setSlsParam(const double &slsStep, const int &slsLim, const int &slsCalType);
	//������ ���, ����� �� ���������� �������� � ��� ��������������� ���������� ��� SLS
	void getSlsParams(double &slsStep, int &slsLim, int &slsCalType) const;
	//���������� ����������� ������� �� ����������� ���� � ������
	void setMinGrowth(const double &minGrowth);
	//������ ��� ����������
	int getCalType() const;
	//������ ������� �������� � ����� �� ���������� �������� � �������-����
	void getNMParams(double &stop, int &iter) const;
	//������ ������������ ���������� � �������-����
	int getNMKoeffs(vector<double> &koeffs) const;
	//������ ����������� ������� �� ����������� ���� � ������
	double getMinGrowth() const;
	//������ ���������� ����� � �������� � ��������
	void getBFParams(int &stepsNum, int &iterNum) const;
private:
	//��� ����������
	int mCalType;
	//����������� ������� �� ����������� ���� � ������
	double mMinGrowth;
	MlcmShell *mMlcmSh;
	NelderMead *mNM;
	SLS *mSLS;
	BruteForce *mBF;
};
