#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <string>
#include "ModelsShell.h"

using namespace std;

ModelsShell::ModelsShell() :
	mDatFormat(1),
	mPcpFormat(1),
	mOutFormat(1),
	mMeasPerDay(24),
	mInfileFormat(1),
	mOutfileFormat(1),
	mWarmingDays(0),
	mEtD(0),
	mEtEE(1),
	mActiveModel(MT_MLCM)
{
	mDatBeg[0] = -1;
	for (int i = 0; i < 6; i++) {
		mSnow[i] = 1;
	}
	mP.clear();
	mDat.clear();
	mET.clear();
	mMlcm = new Mlcm();
}

ModelsShell::~ModelsShell()
{
	delete mMlcm;
	delete[] mOutput;
}

void ModelsShell::setFitnessClass(Fitness *fitn)
{
	mFitness = fitn;
}

void ModelsShell::setOutFormat(const int &outFormat)
{
	mOutfileFormat = outFormat;
}

void ModelsShell::setMaxAandZ(const int *maxA, const int *maxZ)
{
	mMlcm->setMaxAandZ(maxA, maxZ);
}

void ModelsShell::setCLim(const double &minC, const double &maxC)
{
	mMlcm->setCLim(minC, maxC);
}

vector<double> ModelsShell::makeRunoff(const int &dayBeg, const int &dayEnd) const
{
	switch (mActiveModel) {
	case MT_MLCM:
		return mMlcm->makeRunoff(dayBeg * mMeasPerDay, (dayEnd + 1) * mMeasPerDay);
	}
	throw(0);
}

void ModelsShell::printPrediction(const int *begDate, const int *endDate)
{
	int dayBeg = makeTheGap(begDate, mPcpBeg);
	int dayEnd = makeTheGap(endDate, mPcpBeg);
	if ((dayBeg < 1) || (dayEnd * mMeasPerDay > mP.size()))
		throw(1);
	ofstream rout(mOutput, ios::out);
	if (!rout)
		throw(3);
	vector<double> res;
	res = makeRunoff(dayBeg - 1, dayEnd);
	rout.setf(ios::fixed, ios::floatfield);
	rout.precision(3);
	int *nowDate = new int [3];
	nowDate[0] = begDate[0] - 1;
	nowDate[1] = begDate[1];
	nowDate[2] = begDate[2];
	int i;
	for (i = 0; i < (dayEnd - dayBeg) * mMeasPerDay - 1; i++) {
		if (i % mMeasPerDay == 0)
			incDate(nowDate);
		writeOutFormat(rout, nowDate, i, dayBeg * mMeasPerDay, res[i]);
		rout << endl;
	}
	if (i % mMeasPerDay == 0)
		incDate(nowDate);
	writeOutFormat(rout, nowDate, i, dayBeg * mMeasPerDay, res[i]);
	rout.close();
	delete[] nowDate;
}

double ModelsShell::printPrediction(const int *begDate, const int *endDate, const fitnessType &fitnessType)
{
	int dayBeg = makeTheGap(begDate, mPcpBeg);
	int dayEnd = makeTheGap(endDate, mPcpBeg);
	if((--dayBeg < 0)
		|| (--dayEnd * mMeasPerDay > mP.size())
		|| (dayBeg - mGap < 0)
		|| ((dayEnd - mGap - dayBeg + 1) * mMeasPerDay > mDat.size()))
		throw(1);
	ofstream rout(mOutput, ios::out);
	if (!rout)
		throw(3);
	vector<double> res;
	res = makeRunoff(dayBeg - 1, dayEnd);
	rout.setf(ios::fixed, ios::floatfield);
	rout.precision(3);
	double val;
	if (fitnessType == FT_DefaultVal)
		val = mFitness->getValFitness();
	else
		val = mFitness->getFitness(fitnessType);
	rout << val << endl;
	int *nowDate = new int [3];
	nowDate[0] = begDate[0] - 1;
	nowDate[1] = begDate[1];
	nowDate[2] = begDate[2];
	int i;
	for (i = 0; i < (dayEnd - dayBeg) * mMeasPerDay - 1; i++) {
		if (i % mMeasPerDay == 0)
			incDate(nowDate);
		writeOutFormat(rout, nowDate, i, dayBeg * mMeasPerDay, res[i]);
		rout << endl;
	}
	if (i % mMeasPerDay == 0)
		incDate(nowDate);
	writeOutFormat(rout, nowDate, i, dayBeg * mMeasPerDay, res[i]);
	rout.close();
	return val;
	delete[] nowDate;
}

void ModelsShell::changeModelParametrs(const double *params)
{
	switch (mActiveModel) {
	case MT_MLCM:
		mMlcm->setParam(params);
		break;
	default:
		throw(0);
	}
}

void ModelsShell::setFitnessBegEnd(const int *begDate, const int *endDate)
{
	int begDay = makeTheGap(begDate, mPcpBeg);
	int endDay = makeTheGap(endDate, mPcpBeg);
	if((--begDay < 0)
		|| (--endDay * mMeasPerDay > mP.size())
		|| (begDay - mGap < 0)
		|| ((endDay - mGap - begDay + 1) * mMeasPerDay > mDat.size()))
		throw(1);
	mFitness->setBegEnd(begDay, endDay);
}

void ModelsShell::printParams(const wchar_t *outputParamFile)
{
	switch (mActiveModel) {
	case MT_MLCM:
		mMlcm->printParams(outputParamFile);
		break;
	default:
		throw(0);
	}
}

void ModelsShell::loadParams(const wchar_t *inputParamFile)
{
	switch (mActiveModel) {
	case MT_MLCM:
		mMlcm->loadParams(inputParamFile);
		break;
	default:
		throw(0);
	}
}

int ModelsShell::getOutFormat() const
{
	return mOutfileFormat;
}

void ModelsShell::getMaxAandZ(int *maxA, int *maxZ) const
{
	mMlcm->getMaxAandZ(maxA, maxZ);
}

void ModelsShell::getCLim(double &minC, double &maxC) const
{
	mMlcm->getCLim(minC, maxC);
}

void ModelsShell::setOutFile(const wchar_t *outFileName)
{
	mOutput = outFileName;
}

double ModelsShell::makeET(const int &day, const int &month, const double &p) const
{
	if (p > 1e-3)
		return 0;
	//Есть ли сейчас снег
	if ( ((month < mSnow[3]) || ((month == mSnow[3]) && (day <= mSnow[2])) ) 
		|| ((month > mSnow[1]) || ((month == mSnow[1]) && (day >= mSnow[0])) ) )
		return 0;
	//Две недели после снега
	if ( (month < mSnow[5]) || ((month == mSnow[5]) && (day <= mSnow[4])) ) {
		return 0.44 * mEtEE;
	}
	return mEtEE * (1 - exp(-0.65 * mEtD / mEtEE));
}

void ModelsShell::setWarmingDays(const int &countOfWarmingDays)
{
	mWarmingDays = countOfWarmingDays;
	mMlcm->setWarmingSteps(countOfWarmingDays * mMeasPerDay);
}

int ModelsShell::getWarmingDays() const
{
	return mWarmingDays;
}

void ModelsShell::readDeck(const wchar_t *filename)
{
	ifstream deckIn (filename, ios::in);
	if (!deckIn)
		throw(3);
	double Aslope, dt;
	int nuh;
	string str[8];
	int i;
	for (i = 0; i < 4; i++)
		getline(deckIn, str[i]);
	if (str[3] == "") {
		Aslope = atof(str[0].c_str());
		dt = atof(str[1].c_str());
		nuh = atof(str[2].c_str());
		mMeasPerDay = 24.0 / dt;
		mEtD = 0;
		mEtEE = 1;
		for (int i = 0; i < 6; i++) {
			mSnow[i] = 1;
		}
	}
	else {
		for (i = 4; i <= 6; i++)
			getline(deckIn, str[i]);
		if (str[6] == "") {
			Aslope = atof(str[0].c_str());
			dt = atof(str[1].c_str());
			nuh = atof(str[2].c_str());
			mMeasPerDay = 24.0 / dt;
			stringstream etParam (str[3]);
			etParam >> mEtD >> mEtEE;
			stringstream snow1 (str[4]), snow2 (str[5]);
			snow1 >> mSnow[0] >> mSnow[1];
			snow2 >> mSnow[2] >> mSnow[3];
			int daysInLastSnowMonth = giveDaysInMonth(mSnow[3], 2013);
			mSnow[4] = mSnow[2] + 14;
			if (mSnow[4] > daysInLastSnowMonth) {
				mSnow[4] = daysInLastSnowMonth;
				mSnow[5] = mSnow[3] + 1;
			}
			else {
				mSnow[5] = mSnow[3];
			}
		}
		else {
			getline(deckIn, str[7]);
			if(str[7] == "")
				throw(0);
			stringstream dtstream (str[2]);
			dtstream.seekg(53);
			dtstream >> mMeasPerDay;
			stringstream aslopeAndNuh (str[7]);
			aslopeAndNuh.seekg(29);
			aslopeAndNuh >> Aslope >> nuh;
			mEtD = 0;
			mEtEE = 1;
		}
	}
	mAslope = Aslope * 1000;
	mMlcm->setAslopeAndNuh(mAslope, nuh);
	mFitness->setMeasPerDay(mMeasPerDay);
	mMlcm->setWarmingSteps(mWarmingDays * mMeasPerDay);
	deckIn.close();
}

void ModelsShell::readInFormat(ifstream &fin, int &code, int &month, int &day, double &value) const
{
	switch (mInfileFormat) {
	case 1:
		fin >> code >> month >> day >> value;
		return;
	case 2:
		fin >> month >> day >> value;
		code = 0;
		return;
	case 3:
		fin >> value;
		code = 0;
		month = 113;
		day = 1;
		return;
	}
}

void ModelsShell::readAndSetFormat(ifstream &fin, int &code, int &month, int &day, double &value)
{
	string line;
	getline(fin, line);
	stringstream l1(line), l2(line), l3(line);
	if (l1 >> code >> month >> day >> value) {
		mInfileFormat = 1;
		return;
	}
	if (l2 >> month >> day >> value) {
		mInfileFormat = 2;
		code = 0;
		return;
	}
	if (l3 >> value) {
		mInfileFormat = 3;
		code = 0;
		month = 113;
		day = 1;
		return;
	}
	throw(2);
}

void ModelsShell::writeOutFormat(ofstream &fout, const int *date, const int &i, const int &begPoint, const double &value) const
{
	int nMonth = date[1] * 100 + date[2] % 100;
	switch (mOutfileFormat) {
	case 1:
		fout << mCode << " " << nMonth << " " << date[0] << " " << value * mOutFormat;
		return;
	case 2:
		fout << nMonth << " " << date[0] << " " << value * mOutFormat;
		return;
	case 3:
		fout << value;
		return;
	case 4:
		fout << nMonth << " " << date[0] << " " << mP[begPoint + i] * mOutFormat << " "
			<< getRealData(begPoint + mGap * mMeasPerDay + i) * mOutFormat << " " << value * mOutFormat;
		return;
	case 5:
		fout << nMonth << " " << date[0] << " " << mP[begPoint + i] * mOutFormat << " " << mET[begPoint + i] * mOutFormat << " "
			<< getRealData(begPoint + mGap * mMeasPerDay + i) * mOutFormat << " " << value * mOutFormat;
	}
}

double ModelsShell::getRealData(const int &i) const
{
	if ((i < 0) || (i >= mDat.size()))
		return 0;
	return mDat[i];
}

void ModelsShell::readPcp(const double &format, const wchar_t *filename)
{
	mPcpFormat = format;
	ifstream pcpIn (filename, ios::in);
	if (!pcpIn)
		throw(3);
	int code, month, day;
	double tmp;
	readAndSetFormat(pcpIn, code, month, day, tmp);
	mCode = code;
	mPcpBeg[0] = day;
	mPcpBeg[1] = month / 100;
	mPcpBeg[2] = month % 100;
	if (mPcpBeg[2] > 25)
		mPcpBeg[2] += 1900;
	else
		mPcpBeg[2] += 2000;
	mP.clear();
	mET.clear();
	mP.push_back(tmp * mPcpFormat);
	int *nowDate = new int [3];
	nowDate[0] = day;
	nowDate[1] = month / 100;
	nowDate[2] = month % 100;
	mET.push_back(makeET(nowDate[0], nowDate[1], mP.back()));
	int i = 1;
	while (!pcpIn.ios::eof()) {
		if (i % mMeasPerDay == 0)
			incDate(nowDate);
		readInFormat(pcpIn, code, month, day, tmp);
		mP.push_back(tmp * mPcpFormat);
		mET.push_back(makeET(nowDate[0], nowDate[1], mP.back()));
		if (i++ == 1000000) {
			pcpIn.close();
			mP.clear();
			delete[] nowDate;
			throw(0);
		}
	}
	delete[] nowDate;
	mMlcm->setPandET(&mP, &mET);
	if (!mDat.empty()) {
		mGap = makeTheGap(mDatBeg, mPcpBeg);
		mMlcm->setRealData(&mDat, --mGap * mMeasPerDay);
		mFitness->setRealVal(&mDat, mGap);
	}
	pcpIn.close();
}

void ModelsShell::readDat(const double &format, const wchar_t *filename)
{
	mDatFormat = format;
	mOutFormat = 1.0 / format;
	ifstream datIn (filename, ios::in);
	if (!datIn)
		throw(3);
	int code, month, day;
	double tmp;
	readAndSetFormat(datIn, code, month, day, tmp);
	mDat.clear();
	mDatBeg[0] = day;
	mDatBeg[1] = month / 100;
	mDatBeg[2] = month % 100;
	if (mDatBeg[2] > 25)
		mDatBeg[2] += 1900;
	else
		mDatBeg[2] += 2000;
	int i = 0;
	mDat.push_back(tmp * mDatFormat);
	while (!datIn.ios::eof()) {
		readInFormat(datIn, code, month, day, tmp);
		mDat.push_back(tmp * mDatFormat);
		if (i++ == 1000000) {
			datIn.close();
			mDat.clear();
			mDatBeg[0] = -1;
			throw(0);
		}
	}
	datIn.close();
	mGap = makeTheGap(mDatBeg, mPcpBeg);
	mMlcm->setRealData(&mDat, --mGap * mMeasPerDay);
	mFitness->setRealVal(&mDat, mGap);
}

int ModelsShell::makeTheGap(const int *date1, const int *date2) const		//positive if date1 is after date2
{
	if ((date1[0] == -1) || (date2[0] == -1)) {
		return -1;
	}
	int absDate1, absDate2;
	absDate1 = date1[0] + date1[1] * 50 + date1[2] * 600;
	absDate2 = date2[0] + date2[1] * 50 + date2[2] * 600;
	int gap = 0, i;
	if (absDate1 == absDate2)
		return 1;
	if (absDate2 < absDate1) {
		for (i = date2[2]; i < date1[2]; i++) {
			gap += 365;
			if (i % 4 == 0)
				gap += 1;
		}
		for (i = 1; i < date1[1]; i++)
			gap += giveDaysInMonth(i, date1[2]);
		gap += date1[0];
		for (i = 1; i < date2[1]; i++)
			gap -= giveDaysInMonth(i, date2[2]);
		gap -= date2[0] - 1;
	}
	else {
		for (i = date1[2]; i < date2[2]; i++) {
			gap += 365;
			if (i % 4 == 0)
				gap += 1;
		}
		for (i = 1; i < date2[1]; i++)
			gap += giveDaysInMonth(i, date2[2]);
		gap += date2[0];
		for (i = 1; i < date1[1]; i++)
			gap -= giveDaysInMonth(i, date1[2]);
		gap -= date1[0] + 1;
		gap = -gap;
	}
	return gap;
}


int ModelsShell::giveDaysInMonth(const int &month, const int &year) const
{
	switch (month) {
	case 1: return 31;
	case 2:
		if (year % 4 == 0)
			return 29;
		else
			return 28;
	case 3: return 31;
	case 4: return 30;
	case 5: return 31;
	case 6: return 30;
	case 7: return 31;
	case 8: return 31;
	case 9: return 30;
	case 10: return 31;
	case 11: return 30;
	case 12: return 31;
	default: return 30;
	}
}

void ModelsShell::incDate(int *date) const
{
	if (date[0]++ == giveDaysInMonth(date[1], date[2])) {
		date[0] = 1;
		if (date[1]++ == 12) {
			date[1] = 1;
			date[2]++;
		}
	}
}

int ModelsShell::click()
{
	switch (mActiveModel) {
	case MT_MLCM:
		return mMlcm->click();
	default:
		throw(0);
	}
}

int ModelsShell::getModelParams(const int &iter) const
{
	switch (mActiveModel) {
	case MT_MLCM:
		return 5 + 2 * iter;
		break;
	default:
		throw(0);
	}
}

void ModelsShell::setModelIter(const int &iter)
{
	switch (mActiveModel) {
	case MT_MLCM:
		mMlcm->setN(iter);
		break;
	default:
		throw(0);
	}
}

int ModelsShell::getIterNum() const
{
	switch (mActiveModel) {
	case MT_MLCM:
		return 11;
	default:
		throw(0);
	}
}