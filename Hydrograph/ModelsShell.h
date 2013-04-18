#pragma once
#include <vector>
#include "Mlcm.h"
#include "Fitness\Fitness.h"

//�������� �������

using namespace std;

class Fitness;
enum fitnessType;

enum modelType {
	MT_MLCM = 0
};

class ModelsShell
{
public:
	ModelsShell();
	~ModelsShell();
	void setFitnessClass(Fitness *fitn);
	//���������� ������ ��������� �����
	void setOutFormat(const int &outFormat);
	//������� � ���� ��������������� ������
	void printPrediction(const int *begDate, const int *endDate);
	//������� � ���� ��������������� ������ � ���������� �������� ����� fitnessType
	double printPrediction(const int *begDate, const int *endDate, const fitnessType &fitnType);
	//�������� ��������� MLCM
	void changeModelParametrs(const double *params);
	//���������� ����������� �� �������� ����� ���� � ������ �����
	void setMaxAandZ(const int *maxA, const int *maxZ);
	//���������� ������������� ����������� ����
	void setCLim(const double &minC, const double &maxC);
	//���������� ������ � ����� �������� ������� �������
	void setFitnessBegEnd(const int *begDate, const int *endDate);
	//���������� ���������� ���� �������� ������
	void setWarmingDays(const int &countOfWarmingDays);
	//������ ���������� ���� �������� ������
	int getWarmingDays() const;
	//����������� � ���� ��������� ������
	void printParams(const wchar_t *outputParamFile);
	void loadParams(const wchar_t *inputParamFile);
	//������
	int getOutFormat() const;
	void getMaxAandZ(int *maxA, int *maxZ) const;
	void getCLim(double &minC, double &maxC) const;
	//������ �������� ����
	void setOutFile(const wchar_t *outFileName);
	//��������� ������ � ���������
	void readDeck(const double fbasinFormat, const double etFormat, const wchar_t *filename);
	//��������� ���� � ��������
	void readPcp(const double &format, const wchar_t *filename);
	//��������� ���� � ��������� ����������
	void readDat(const double &format, const wchar_t *filename);
	//������� ��� ����������� MLCM � ������� ���������� �����
	int click();
	//��������� ������� ������� �� ����������
	double countF(const double *params);
	//������ ������
	vector<double> makeRunoff(const int &dayBeg, const int &dayEnd) const;
	//������ ���������� ���������� ������ ��� ���� ��������
	int getModelParams(const int &iter) const;
	//������ �������� ������
	void setModelIter(const int &iter);
	int getIterNum() const;
private:
	//������ ��������� � ������ ������
	double makeET(const int *date, const double &p) const;
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
	//���� �������� �������� ������ � ������� ����������� � ���
	double getRealData(const int &i) const;
	//��������� ����
	void incDate(int *date) const;
	//�������� ������
	modelType mActiveModel;
	//MLCM
	Mlcm *mMlcm;
	//������� �������
	Fitness *mFitness;
	//����� ������
	double mAslope;
	//������� ���������
	double mFbasin;
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
	//���������� ���� �������� ������
	int mWarmingDays;
	//������ �������� ������ � �������
	int mDatBeg[3], mPcpBeg[3]; //0 - day, 1 - month, 2 - year;
	//��� ��������� �����
	const wchar_t *mOutput;
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
	//����������� �������� ������
	double mOutFormat;
	//������ ������� ��������� - 0 - ����� �������������� ��������, 1 - ����� EE � d
	bool mEtWay;
	//�������������� ���������
	double *mAvET;
	//������������
	double mEtEE;
	//�������������� �������� �������� ���������
	double mEtD;
	//���� ��������� �� ������ � �������: ��������� �����: 0 - ����, 1 - �����; ���� �����: 2 - ����, 3 - �����;
	//��� ������ �� �����: 4 - ����, 5 - �����
	int mSnow[6];
};