#pragma once
#include <vector>
#include "Mlcm.h"
#include "Fitness\Fitness.h"
#include "Element.h"

//�������� MLCM

using namespace std;

class MlcmShell
{
public:
	MlcmShell();
	~MlcmShell();
	//���������� ������� ������� ��� ����������
	void setDefFitnessType(const int &defFitnType);
	//���������� ������� ������� ��� ���������
	void setValType(const int &valType);
	//���������� ������ ��������� �����
	void setOutFormat(const int &outFormat);
	//������� � ���� ��������������� ������
	void printPrediction(const int *begDate, const int *endDate);
	//������� � ���� ��������������� ������ � ���������� �������� ����� fitnessType
	double printPrediction(const int *begDate, const int *endDate, const int &fitnessType);
	//��������� ��������� MLCM
	void loadParametrs(const char *paramFile);
	//�������� ��������� MLCM
	void changeModelParametrs(const Element &element);
	//���������� ����������� �� �������� ����� ���� � ������ �����
	void setMaxAandZ(const int *newMaxA, const int *newMaxZ);
	//���������� ������������� ����������� ����
	void setCLim(const double &c1, const double &c2);
	//������ ������� �������
	double getFitness() const;
	//���������� ���������
	double getValFitness() const;
	//���������� ������ � ����� �������� ������� �������
	void setFitnessBegEnd(const int *begDate, const int *endDate);
	//����������� � ���� ��������� ������
	void printParams(const char *outputParamFile);
	//�������� ���������
	Element getParams() const;
	//������
	void getFitnessTypes(int &defType, int &valType) const;
	int getOutFormat() const;
	void getMaxAandZ(int *maxA, int *maxZ) const;
	void getCLim(double &c1, double &c2) const;
	//������ �������� ����
	void setOutFile(char *outFileName);
	//��������� ������ � ���������
	void readDeck(const double &format, const char *filename);
	//��������� ���� � ��������
	void readPcp(const double &format, const char *filename);
	//��������� ���� � ��������� ����������
	void readDat(const double &format, const char *filename);
	//������� ��� ����������� MLCM � ������� ���������� �����
	int click();
	//��������� ������� ������� �� ����������
	double countF(const Element &point);
private:
	//������ ��������� �� ������
	double makeET(const int &month);
	//������� ������� � ���� ����� ����� ������ � ������� int[3] - ����-�����-���
	int makeTheGap(const int *date1, const int *date2) const;
	//���������� ���������� ���� � ������
	int giveDaysInMonth(const int &month, const int &year) const;
	//���� �������� ������ �� �����
	void readInFormat(ifstream &fin, int &code, int &month, int &day, double &value) const;
	//�������� ������ ������� ����� � ���������� ������
	void readAndSetFormat(ifstream &fin, int &code, int &month, int &day, double &value);
	//���� �������� ������ � ����
	void writeOutFormat(ofstream &fout, const int *date, const int &i, const int &begPoint, const double &value) const;
	//���� �������� �������� ������ � ������� ����������� � 0 ���
	double getRealData(const int &i) const;
	//��������� ����
	void incDate(int *date) const;
	//MLCM
	Mlcm *mModel;
	//������� �������
	Fitness *mFitness;
	//����� ������
	int mAslope;
	//���������� ��������� � ����
	int mMeasPerDay;
	//������ ������� ������
	int mInfileFormat;
	//������ �������� ������
	int mOutfileFormat;
	//��� ���������
	int mCode;
	//������� ����� ������� ������ �� ������� � ������� ������ � �������� ������
	int mGap;
	//������ �������� ������ � �������
	int mDatBeg[3], mPcpBeg[3]; //0 - day, 1 - month, 2 - year;
	//��� ��������� �����
	const char *mOutput;
	//������
	vector<double> mP;
	//���������
	vector<double> mET;
	//�������� ������
	vector<double> mDat;
	//����������� ������� (�� ������� ���� ��������, ����� �������� ��/�)
	double mPcpFormat;
	//����������� �������� ������
	double mDatFormat;
	//������������ ���������
	double mETFormat;
	//����������� �������� ������
	double mOutFormat;
};

