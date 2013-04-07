#pragma once
#include <vector>
#include <queue>
#include "Element.h"

//MLCM

using namespace std;

class Mlcm
{
public:
	Mlcm();
	~Mlcm();
	//���������� ����� ������ � ���������� ������� ���������� ����������
	void setAslopeAndNuh(const double &Aslope, const int &nuh);
	//���������� �������� ������ � �� ������
	void setRealData(vector<double> *realData, const int &realDatBeg);
	//���������� �������� ������
	void setParam(const Element &element);
	//���������� ������ � ���������
	void setPandET(vector<double> *P, vector<double> *ET);
	//������ ������
	vector<double> makeRunoff(const int &timeBeg, const int &timeEnd) const;
	//������� ��� ����������� MLCM � ������� ���������� �����
	int click();
	//�������� ��������� ������
	Element getParam() const;
private:
	//������� ���� ������ �� ����� ������ � ������ ������� time
	struct Water {
		Water (const double &waterThikness, const int &newTime);
		double water;
		int time;
	};
	//������ �������� ���������� ����������
	void makeHydrOrd();
	//������� Qsum � ������ ������� time
	double countUhT(const vector<double> &Qsum, const int &time) const;
	//���� ��� ������ (��� = 24/<���������� ��������� � ����>)
	double makeStep(const double &P
					, const double &ET
					, const int &time
					, queue<Water> *waterQueue
					, vector<double> &state) const;
	//������� ���� ����� �� ����� ������ � ������ ������� time
	double countChannelWater(const int &time, queue<Water> *waterQueue, vector<double> &state) const;
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
	//����� �� ������� ���� ������ �� ����������� (� �����)
	int mTime0;
	//�������� ������������� ����������� ����
	double mC;
	//etta � k - ��� �������� ������� ���������� ���������� � ������� ����� �������
	double mEtta;
	double mK;
	//����� �� �������
	double mT;
	//�������� �������������� ��������
	double mAlpha0;
	//����� ������
	double mAslope;
	//���������, ������ ������� �������� ��������� �����
	static const double mMin;
	//������ ������, �� ������� ���� ������ �� i-��� ����
	double *mTime;
	//�������� ���������� ����������
	vector<double> mFxOrd;
	//������ ��������� ����� ����
	vector<double> mAlpha;
	//������ ����� �����
	vector<double> mZ;
	//������
	vector<double> *mP;
	//���������
	vector<double> *mET;
	//�������� ������
	vector<double> *mRealData;
};
