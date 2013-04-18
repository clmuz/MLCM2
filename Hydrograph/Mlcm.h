#pragma once
#include <vector>
#include <queue>

//MLCM

using namespace std;

class Mlcm
{
public:
	Mlcm();
	~Mlcm();
	//���������� ����� ������ � ���������� ������� ���������� ����������
	void setAslopeAndNuh(const double &faslope, const int &nuh);
	//���������� �������� ������ � �� ������
	void setRealData(vector<double> *realData, const int &realDatBeg);
	//���������� ���������� �����
	void setN(const int &n);
	int getN() const;
	//���������� �������� ������
	void setParam(const double *params);
	//���������� ������ � ���������
	void setPandET(vector<double> *P, vector<double> *ET);
	//���������� ���������� ����� �������� ������
	void setWarmingSteps(const int &countOfWarmingSteps);
	//������ ���������� ����� �������� ������
	int getWarmingSteps() const;
	//���������� ����������� �� �������� ����� ���� � ������ �����
	void setMaxAandZ(const double *maxA, const double *maxZ);
	//���������� ����������� �� �������� ������������� ���������� �����
	void setCLim(const double &minC, const double &maxC);
	void getMaxAandZ(double *maxA, double *maxZ) const;
	void getCLim(double &minC, double &maxC) const;
	//������ ������
	vector<double> makeRunoff(const int &timeBeg, const int &timeEnd) const;
	//������� ��� ����������� MLCM � ������� ���������� �����
	int click();
	void printParams(const wchar_t *outputParamFile) const; //TODO
	void loadParams(const wchar_t *inputParamFile); //TODO
private:
	//������ �������� ���������� ����������
	void makeHydrOrd();
	//������� Qsum � ������ ������� time
	double countUhT(const vector<double> &Qsum, const int &time) const;
	//���� ��� ������ (��� = 24/<���������� ��������� � ����>)
	double makeStep(const double &P
					, const double &ET
					, vector<double> &state) const;
	//������� ���� ����� �� ����� ������ � ������ ������� time
	double countChannelWater(vector<double> &state) const;
	//���������� �����
	int mN;
	//���������� ������� � ������ ������ �� �������
	int mNOrd;
	//���������� �������
	int mNuh;
	//������ �������� ������ ������������ ������ ������� � ���������� �����
	int mRealBeg;
	//����� �������� ������ ������������ ������ ������� � ���������� �����
	int mRealEnd;
	//���������� �������� ������ �� ������ ������ (int* ����� �� �������� ������������� ������ ������)
	int *mClick;
	//���������� ����� �������� ������
	int mWarmingSteps;
	//�������� ������������� ����������� ����
	double mC;
	//etta � k - ��� �������� ������� ���������� ���������� � ������� ����� �������
	double mEtta;
	double mK;
	//����� �� �������
	double mT;
	//������� ������
	double mFbasin;
	//�������� ���������� ����������
	vector<double> mFxOrd;
	//������ ��������� ����� ����
	double *mAlpha;
	//������ ����� �����
	double *mZ;
	//������
	vector<double> *mP;
	//���������
	vector<double> *mET;
	//�������� ������
	vector<double> *mRealData;
	//����������� �� �������� ����� ���� � �� ��������, ����������� �� ������� (0 - �������������, 10 - 10-�� ����)
	double *mMaxA, *mMaxAS;
	//����������� �� ������ ����� (0 - 1 ����)
	double *mMaxZ, *mMaxZS;
	//����������� �� �������� ������������� ���������� �����
	double mMinC, mMaxC;
};
