
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <Windows.h>
#include <iomanip>
#include"src/spline.h"




using namespace std;

std::vector<std::vector<double>> ReadProfile(string filelocation);

std::vector<std::vector<double>> SizebyChord(std::vector<std::vector<double>> NASAdata,double chord);
std::vector<std::vector<double>> SplitdataUpper(std::vector<std::vector<double>> NASAdata);
std::vector<std::vector<double>> SplitdataLower(std::vector<std::vector<double>> NASAdata);
std::vector<std::vector<double>>flip(std::vector<std::vector<double>> hellowhat);
std::vector<std::vector<double>> Orientationchange(std::vector<std::vector<double>> Nasadata, double staggerang);
std::vector<std::vector<double>> Postionchange(std::vector<std::vector<double>> hellowhat);
void Outputtofile2(std::vector<std::vector<double>> filedataready, int kk);
std::vector<std::vector<double>>  Wrappingradialstation(std::vector<std::vector<double>> Nasadata,double r);
std::vector<std::vector<double>> openTail(std::vector<std::vector<double>> Nasadata,double usergape,double Chordinput);
std::vector<std::vector<double>> fileprep_Inventor(std::vector<std::vector<double>> Nasadata);
std::vector<std::vector<double>> splitfind(std::vector<std::vector<double>> Nasadata, int kk, int bb);
double heightpoints(std::vector<std::vector<double>> Nasadata);
void Outputtofile(std::vector<std::vector<double>> NASAdata, int kk,int k);
double Tothepower(double Power,int p);
std::vector<std::vector<double>> rotatingsearch(std::vector<std::vector<double>> Nasadata);
std::vector<std::vector<double>> Rotate(std::vector<std::vector<double>> Nasadata, int staggerang);
string GetFileName(const string & prompt);
std::vector<std::vector<double>> Splineinterpolation(std::vector<std::vector<double>> Nasadata);



//Raduis at each station
//static const std::vector<double> R{ 0.3084, 0.33153, 0.35466, 0.37779, 0.40092, 0.42405, 0.44718, 0.4703, 0.49344 ,0.51657 ,0.5397 , 0.56283 , 0.58596 , 0.60909, 0.63222 ,0.65535 , 0.67848 , 0.70161 , 0.72474 , 0.74787 , 0.771 };

//blade chord at each station
//static const std::vector<double> Chordinput{ 0.186720436, 0.187629128, 0.188537821, 0.189446513, 0.190355205, 0.191263898, 0.19217259, 0.193081283 , 0.193989975, 0.194898668, 0.19580736, 0.196716053, 0.197624745, 0.198533437, 0.19944213, 0.200350822, 0.201259515, 0.202168207,  0.2030769 , 0.203985592, 0.204894285 };
//stagger angel at each station
//static const std::vector<double> StaggerA{ 55.2624962 , 58.99507055 ,62.11647005, 64.76973455, 66.80571614, 68.57479551 ,70.12707707, 71.50080664, 72.72567311, 73.82507349, 74.81769833, 75.71866208, 76.54032306 , 77.29288765 , 77.98486199, 78.62339429, 79.21453739, 79.76345237, 80.2745683, 80.75170857 , 81.19819189 };
const double PI = 3.141592653589793238463;






int main(int argc, char *argv[])
{
	
		int ask = 0;

	 do
	{
		 int btn = MessageBox(0,
			 argc > 1 ? argv[1] : "WOULD YOU LIKE TO RUN THE PROGRAME?",
			 argc > 2 ? argv[2] : "WELCOME",
			 MB_YESNO + MB_ICONQUESTION);
		 if (btn == IDYES) {
			 ask = 0;
		 }
		 else {
			 exit(EXIT_SUCCESS);
		 }
		 MessageBox(0, "Select the Text Document containing the raduis points", "Input", 0);
		 std::vector<double> R;
		 string fname = GetFileName("Excel sheet containing raduis points : ");
		 ifstream ifs(fname.c_str());

		 if (!ifs.is_open()) {
			 cerr << "cannot open " << fname << " for input" << endl;
		 }
		 else {
			 string line;
			 int lineno = 1;
			 while (getline(ifs, line)) {
				 R.push_back( stod(line));
			 }
		 }
		 MessageBox(0, "Select the Text Document containing the Chord points", "Input", 0);
		 std::vector<double> Chordinput;
		 string fname1 = GetFileName("Excel sheet containing Chord points:  ");
		 ifstream ifs1(fname1.c_str());
		 if (!ifs1.is_open()) {
			 cerr << "cannot open " << fname1 << " for input" << endl;
		 }
		 else {
			 string line;
			 int lineno = 1;
			 while (getline(ifs1, line)) {
				 Chordinput.push_back(stod(line));
			 }
		 }
		 MessageBox(0, "Select the Text Document containing the Stagger angles points", "Input", 0);
		 std::vector<double> StaggerA;
		 string fname2 = GetFileName("Excel sheet containing Stagger angles:  ");
		 ifstream ifs2(fname2.c_str());
		 if (!ifs2.is_open()) {
			 cerr << "cannot open " << fname2 << " for input" << endl;
		 }
		 else {
			 string line;
			 int lineno = 1;
			 while (getline(ifs2, line)) {
				 StaggerA.push_back(stod(line));
			 }
		 }
		 MessageBox(0, "Select the Text Document containing the Air foil profile in Lednicer format", "Input", 0);
		 string filelocation = GetFileName("Air foil profile file ,Lednicer txt format no headers:  ");
		 ifstream ifs4(filelocation.c_str());
		 if (!ifs4.is_open()) {
			 cerr << "cannot open " << filelocation << " for input" << endl;

		 }	


		 double usergape;

		 MessageBox(0, "Enter trailing edge gap in [m]", "Concol input", 0);
		 cin >> usergape;
		 cout << "\n";


		 int result = MessageBox(0,
			 argc > 1 ? argv[1] : "are you satisfied with your Input and wish to continue?",
			 argc > 2 ? argv[2] : "Menue",
			 MB_YESNOCANCEL + MB_ICONQUESTION);
		 switch (result)
		 {
		 case IDYES:
			 ask = 1;
			 break;
		 case IDNO:
			 ask = 2;
			 break;
		 case IDCANCEL:
			 ask = 3;
			 break;
		 }

		 if (ask == 1)
		 {
			 int kk = 0;
			 std::ofstream Testfile;
			 for (size_t i = 0; i < R.size(); i++)
			 {


				 std::ostringstream oss;
				 oss << kk << ".xls";
				 std::string var = oss.str();

				 Testfile.open(var, std::ofstream::out | std::ofstream::trunc);
				 Testfile.close();

				 Testfile.open(var);
				 Testfile << "m" << "\n";
				 Testfile << "X\tY\tZ" << "\n";
				 Testfile.close();
				 kk++;
			 }
			 for (size_t i = R.size(); i < 2*R.size(); i++)
			 {


				 std::ostringstream oss;
				 oss << kk << ".xls";
				 std::string var = oss.str();

				 Testfile.open(var, std::ofstream::out | std::ofstream::trunc);
				 Testfile.close();

				 Testfile.open(var);
				 Testfile << "m" << "\n";
				 Testfile << "X\tY\tZ" << "\n";
				 Testfile.close();
				 kk++;
			 }


			 int length = R.size();

			 int lenght2 = (ReadProfile(filelocation).size()) / 2;

			 std::vector<std::vector<double>> NASADATAPCHORD;
			NASADATAPCHORD.resize(lenght2, std::vector<double>(9));

			 std::vector<std::vector<double>> NASADATAPCHORD4;
			 NASADATAPCHORD4.resize(lenght2, std::vector<double>(9));

			 std::vector<std::vector<double>> NASADATAPCHORD2;
			 NASADATAPCHORD2.resize(lenght2*length, std::vector<double>(9));

			 std::vector<std::vector<double>> NASADATAPCHORD3;
			 NASADATAPCHORD3.resize(lenght2*length, std::vector<double>(9));

			 cout << "Processing Please wait...";
			// MessageBox(0, "Please wait...", "Processing", 0);


			 for (size_t i = 0; i < length; i++)
			 {
	
					//NASADATAPCHORD = Wrappingradialstation(Orientationchange(Splineinterpolation(Postionchange(openTail(SizebyChord(ReadProfile(filelocation), Chordinput[i]), usergape, Chordinput[i]))), StaggerA[i]), R[i]);

			
					NASADATAPCHORD = Wrappingradialstation(Orientationchange(Postionchange(openTail(flip(SizebyChord(ReadProfile(filelocation), Chordinput[i])), usergape, Chordinput[i])), StaggerA[i]), R[i]);
				 //Outputtofile2( Wrappingradialstation(Orientationchange(Postionchange(openTail(flip(SizebyChord(ReadProfile(filelocation), Chordinput[i])), usergape, Chordinput[i])), StaggerA[i]), R[i]),i);



				 for (size_t L = 0; L < lenght2; L++)
				 {
					 for (size_t j = 0; j < 9; j++)
					 {
						 NASADATAPCHORD2[L + lenght2 * i][j] = NASADATAPCHORD[L][j];
					 }

				 }

			 }

			 int kenght = NASADATAPCHORD.size();
			 NASADATAPCHORD3 = rotatingsearch(NASADATAPCHORD2);

			 for (size_t i = 0; i < length; i++)
			 {

				 for (size_t L = 0; L < lenght2; L++)
				 {
					 for (size_t j = 0; j < 9; j++)
					 {
						 NASADATAPCHORD4[L][j] = NASADATAPCHORD3[L + lenght2 * i][j];
					 }

				 }
				//Outputtofile(fileprep_Inventor(NASADATAPCHORD4),i,1);
				splitfind(fileprep_Inventor(NASADATAPCHORD4), i, length);
			 }
			 cout << "Computation complete...";
		 }

	 } while (ask!=3);
	 
	 return 0;
}

std::vector<std::vector<double>> ReadProfile(string filelocation)
{


	std::ifstream wingfile(filelocation.c_str());

	std::vector<double> WingDATA
		{ 
			(std::istream_iterator<double>(wingfile)),
			std::istream_iterator<double>() 
		};
	//std::copy(WingDATA.begin(), WingDATA.end(), std::ostream_iterator<double>(std::cout, " "));

	
	int rows = WingDATA.size()/2;
	int cols = sizeof WingDATA[0] / sizeof(int);;
	int initial_value = 0;
	//cout <<'\n' << rows;

	std::vector<std::vector<double>> W_data;

	W_data.resize(rows, std::vector<double>(cols, initial_value));
	int i = 0;
	int k = 0;

	for (size_t i = 0; i < rows; i++)
	{

		for (int j = 0; j < cols; j++)
				{
					W_data[i][j] = WingDATA[k];
					k++;
				//	cout << "\n" << W_data[i][j];
				}
	}
	



	return W_data;
}


std::vector<std::vector<double>> SizebyChord(std::vector<std::vector<double>> NASAdata, double chord)
{
	//int	rows = NASAdata.size();
	int rows2 = NASAdata.size() / 2;

	std::vector<std::vector<double>> Wingupper;
	Wingupper.resize(rows2, std::vector<double>(2, 0));
	Wingupper =  SplitdataUpper(NASAdata);

	std::vector<std::vector<double>> Winglower;
	Winglower.resize(rows2, std::vector<double>(2, 0));
	Winglower = SplitdataLower(NASAdata);;

	std::vector<std::vector<double>>  meanchamber;
	meanchamber.resize(rows2, std::vector<double>(2, 0));

	std::vector<double> lowerthickness;
	lowerthickness.resize(rows2);
	std::vector<double> upperthickness;
		upperthickness.resize(rows2);
		/*
	std::vector<double> Dimenstionx;
	Dimenstionx.resize(rows2);
	std::vector<double> Dimenstionyu;
	Dimenstionyu.resize(rows2);
	std::vector<double> Dimenstionyl;
	Dimenstionyl.resize(rows2);
	*/


	std::vector<std::vector<double>> travildata;
	travildata.resize(rows2, std::vector<double>(4,0));


	for (size_t i = 0; i < rows2; i++)
	{	

		meanchamber[i][0] = Winglower[i][0];
		meanchamber[i][1] = (Wingupper[i][1] + Winglower[i][1])/2;//interpolation still reqeured
		lowerthickness[i] = (meanchamber[i][1] - Winglower[i][1])*chord;
		upperthickness[i] = ( Wingupper[i][1]- meanchamber[i][1])*chord;
		//Dimenstionx [i]= meanchamber[i][1] * Chord[0];
		//Dimenstionyu[i] = meanchamber[i][1] * Chord[0]; + upperthickness[i];
		//Dimenstionyl[i] = meanchamber[i][1] * Chord[0]; - lowerthickness[i];

		travildata[i][0] = meanchamber[i][0] * chord;;
		//cout << "Dimentionx  " << travildata[i][0]<<"\n";
		travildata[i][1] = ((Wingupper[i][1] + Winglower[i][1]) / 2)*chord;
		//cout << "camberliney  " << travildata[i][1] << "\n";
		travildata[i][2] = meanchamber[i][1] * chord + upperthickness[i];
		//cout << "DimenstionYupper  " << travildata[i][2] << "\n";
		travildata[i][3] = meanchamber[i][1] * chord -lowerthickness[i];
		//cout <<"DimenstionYlower  " << travildata[i][3] << "\n";

	}


	//cout << travildata[rows2 - 1][2] << "\n" << travildata[rows2 - 1][3] << "\n";

	return travildata;
}

std::vector<std::vector<double>> SplitdataUpper(std::vector<std::vector<double>> NASAdata){

	int	rows = NASAdata.size();
	std::vector<std::vector<double>> WingUpper;
	WingUpper.resize(rows/2, std::vector<double>(2, 0));

	for (size_t i = 0; i < 2; i++)
	{

		for (size_t j = 0; j < rows / 2; j++)
		{
		 WingUpper[j][i] = NASAdata[j][i];
		 //cout << WingUpper[j][i]<<"\n";
		}
			
		
	}


	return WingUpper;
}

std::vector<std::vector<double>> SplitdataLower(std::vector<std::vector<double>> NASAdata) {

	int	amountofR = NASAdata.size();
	std::vector<std::vector<double>> Winglower;
	int const check = (amountofR);
	Winglower.resize(amountofR/2, std::vector<double>(2, 0));

	//cout << NASAdata[103][1];

	
	for (size_t i = 0; i < 2; i++)
	{
		int count = 0;
		for (size_t j = (check /2); j < check  ; j++)
		{
			Winglower[count][i] = NASAdata[j][i];
			//cout << Winglower[count][i] << "\n";
			count++;
		}


	}


	return Winglower;
}

/*
std::vector<std::vector<double>> Postionchange(std::vector<std::vector<double>> hellowhat)
{
	double ystripe = 0;
	double yu = 0;
	double yl = 0;
	double xstripe = 0;
	double xu = 0;
	double xl = 0;
	double Au = 0;
	double Al = 0;
	double Areasum =0 ;

	int length  = hellowhat.size();

	for (size_t j = 0; j < length-1; j++)
	{

	//	y = (((hellowhat[j][3] + hellowhat[j + 1][3] + hellowhat[j + 1][3]) / 3)*(0.5*(hellowhat[j + 1][3] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + ((hellowhat[j][3] + hellowhat[j][3] + hellowhat[j + 1][3]) / 3)*(0.5*hellowhat[j][3] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][3] - hellowhat[j][3]), 2))))) +(((hellowhat[j][2] + hellowhat[j + 1][2] + hellowhat[j + 1][2]) / 3)*(0.5*(hellowhat[j + 1][2] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + ((hellowhat[j][2] + hellowhat[j][2] + hellowhat[j + 1][2]) / 3)*(0.5*hellowhat[j][2] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][2] - hellowhat[j][2]), 2))))) / ((0.5*(hellowhat[j + 1][2] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + (0.5*hellowhat[j][2] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][2] - hellowhat[j][2]), 2))))+ ((0.5*(hellowhat[j + 1][3] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + (0.5*hellowhat[j][3] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][3] - hellowhat[j][3]), 2))))));
	//	x = (((hellowhat[j][0] + hellowhat[j + 1][0] + hellowhat[j + 1][0]) / 3)*(0.5*(hellowhat[j + 1][3] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + ((hellowhat[j][0] + hellowhat[j][0] + hellowhat[j + 1][0]) / 3)*(0.5*hellowhat[j][3] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][3] - hellowhat[j][3]), 2))))) +(((hellowhat[j][0] + hellowhat[j + 1][0] + hellowhat[j + 1][0]) / 3)*(0.5*(hellowhat[j + 1][2] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + ((hellowhat[j][0] + hellowhat[j][0] + hellowhat[j + 1][0]) / 3)*(0.5*hellowhat[j][2] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][2] - hellowhat[j][2]), 2))))) / ((0.5*(hellowhat[j + 1][2] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + (0.5*hellowhat[j][2] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][2] - hellowhat[j][2]), 2))))+ ((0.5*(hellowhat[j + 1][3] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + (0.5*hellowhat[j][3] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][3] - hellowhat[j][3]), 2))))));
		
		Au = abs((0.5*(hellowhat[j + 1][2] * (hellowhat[j + 1][0] - hellowhat[j][0]))) + (0.5*hellowhat[j][2] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][2] - hellowhat[j][2]), 2)))));
		Al = abs((0.5*(hellowhat[j + 1][3] * (hellowhat[j + 1][0] - hellowhat[j][0])))+ (0.5*hellowhat[j][3] * (sqrt(Tothepower((hellowhat[j + 1][0] - hellowhat[j][0]), 2) + Tothepower((hellowhat[j + 1][3] - hellowhat[j][3]), 2)))));
		yu = ((hellowhat[j][2] + hellowhat[j + 1][2] + hellowhat[j + 1][2]) / 3);
		xu = ((hellowhat[j][0] + hellowhat[j + 1][0] + hellowhat[j + 1][0]) / 3);
		yl = ((hellowhat[j][3] + hellowhat[j + 1][3] + hellowhat[j + 1][3]) / 3);
		xl = ((hellowhat[j][0] + hellowhat[j + 1][0] + hellowhat[j + 1][0]) / 3);
		ystripe = ((Au*yu + Al * yl))+ ystripe;
		Areasum = (Au + Al)+ Areasum;
		xstripe = ((Au*xu + Al*xl)) + xstripe;

	
	}

	double ysteep = ystripe/ Areasum;
	double xsteep = xstripe/ Areasum;


	for (size_t i = 0; i < length; i++)
	{

		hellowhat[i][0] = hellowhat[i][0] - xsteep;
		hellowhat[i][1] = hellowhat[i][1] + ysteep;
		hellowhat[i][2] = hellowhat[i][2] + ysteep;
		hellowhat[i][3] = hellowhat[i][3] + ysteep;
	}
	
	return hellowhat;
}

*/

std::vector<std::vector<double>> Postionchange(std::vector<std::vector<double>> hellowhat) {
	int length = hellowhat.size();
	vector<double> tmax;
	tmax.resize(length, 0);

	for (size_t i = 0; i < length; i++)
	{

		tmax[i] = hellowhat[i][2] - hellowhat[i][3];

	}
	std::vector<double>::iterator max;
	max = std::max_element(tmax.begin(), tmax.end());
	//cout << "\n" << (std::distance(tmax.begin(), max));
	std::vector<std::vector<double>> Openeddata;
	Openeddata.resize(length, std::vector<double>(4, 0));


	
	for (size_t i = 0; i < length; i++)
	{
		//Openeddata[i][0] = hellowhat[i][0] - hellowhat[std::distance(tmax.begin(), max)][0];
		//Openeddata[i][1] = hellowhat[i][0] - hellowhat[std::distance(tmax.begin(), max)][0];
		//Openeddata[i][2] = hellowhat[i][2]- (hellowhat[std::distance(tmax.begin(), max)][2] - tmax[std::distance(tmax.begin(), max)] / 2);
		//Openeddata[i][3] = hellowhat[i][3] -(hellowhat[std::distance(tmax.begin(), max)][2] - tmax[std::distance(tmax.begin(), max)] / 2);

		Openeddata[i][0] = hellowhat[i][0] - hellowhat[168][0];
		Openeddata[i][1] = hellowhat[i][0] - hellowhat[168][0];
		Openeddata[i][2] = hellowhat[i][2] - (hellowhat[168][2] - tmax[168] / 2);
		Openeddata[i][3] = hellowhat[i][3] - (hellowhat[168][2] - tmax[168] / 2);

	}

	return Openeddata;
}
std::vector<std::vector<double>>flip(std::vector<std::vector<double>> hellowhat) {
	int length = hellowhat.size();
	std::vector<std::vector<double>> Openeddata;
	Openeddata.resize(length, std::vector<double>(4, 0));

	for (size_t i = 0; i < length; i++)
	{
		Openeddata[i][0] = -hellowhat[i][0];
		Openeddata[i][1] = hellowhat[i][1];
		Openeddata[i][2] = hellowhat[i][2];
		Openeddata[i][3] = hellowhat[i][3];
	}

	return Openeddata;
}

void Outputtofile(std::vector<std::vector<double>> filedataready,int kk, int k) {

	std::ostringstream oss;
	oss << kk << ".xls";
	std::string var = oss.str();
	std::ofstream myfile;
	myfile.open(var, std::ofstream::out | std::ofstream::app);
	int	rows = filedataready.size();

	int j = 0;
	int i = 0;

	//for (size_t PP = 0; PP < 2; PP++)
	//{
		for ( j = 0; j <(rows); j++)
		{
			
				myfile << filedataready[j][k]<<"\t"<< filedataready[j][k+2] <<"\t" << filedataready[j][ k + 4] ;

			myfile << "\n";
		
		}
	//}


	myfile.close();

	
}

double Tothepower(double Power,int p)
{
	
	for (size_t i = 0; i < p-1; i++)
	{
		Power= Power * Power;

	}

	return Power;
}
 
std::vector<std::vector<double>> openTail(std::vector<std::vector<double>> Nasadata, double Usergape,double Chordinput)
{
	int length = Nasadata.size();
	
	double alphaupper = 0;
	double alphalower = 0;
	double calccamber = (Nasadata[length-1][2] + Nasadata[length-1][3]) / 2;

	alphaupper = atan((Usergape+calccamber - Nasadata[length-1][2])/ Chordinput);
	alphalower = atan((calccamber-Usergape - Nasadata[length-1][3]) / Chordinput);

	std::vector<std::vector<double>> Openeddata;
	Openeddata.resize(length, std::vector<double>(4, 0));


	for (size_t i = 0; i < length; i++)
	{
		Openeddata[i][0] = Nasadata[i][0];
	
		Openeddata[i][1] = Nasadata[i][1];
	
		Openeddata[i][2] = (Nasadata[i][2]) * cos(alphaupper) + (Nasadata[i][0])* sin(alphaupper);
		
		Openeddata[i][3] = (Nasadata[i][3])* cos(alphalower) + (Nasadata[i][0])* sin(alphalower);
		
	}

	
	return Openeddata;
}

std::vector<std::vector<double>> Orientationchange(std::vector<std::vector<double>> Nasadata, double staggerang)
{
	int length = Nasadata.size();
	std::vector<std::vector<double>> newNAsadata;
	newNAsadata.resize(length, std::vector<double>(6, 0));

	for (size_t i = 0; i < length; i++)
	{
		//newNAsadata[i][0] =( Nasadata[i][0])* cos(PI/2-staggerang* (PI / 180)) - (Nasadata[i][1])* sin(PI / 2 - staggerang * (PI / 180));

		newNAsadata[i][1] = (Nasadata[i][0])* cos(PI / 2 - staggerang * (PI / 180)) - (Nasadata[i][2])* sin(PI / 2 - staggerang * (PI / 180));
		newNAsadata[i][2] = (Nasadata[i][1])* cos(PI / 2 - staggerang * (PI / 180)) - (Nasadata[i][3])* sin(PI / 2 - staggerang * (PI / 180));

		//newNAsadata[i][3] =( Nasadata[i][1])* cos(PI / 2 - staggerang * (PI / 180)) +( Nasadata[i][0]) * sin(PI / 2 - staggerang * (PI / 180));

		newNAsadata[i][4] =( Nasadata[i][2]) * cos(PI / 2 - staggerang * (PI / 180)) + (Nasadata[i][0])* sin(PI / 2 - staggerang * (PI / 180));
		newNAsadata[i][5] =( Nasadata[i][3])* cos(PI / 2 - staggerang * (PI / 180)) + (Nasadata[i][1])* sin(PI / 2 - staggerang* (PI / 180));
		//cout << "\n" << newNAsadata[i][1] << " " << newNAsadata[i][2] << " " << newNAsadata[i][4] << " " << newNAsadata[i][5];
	}


	return newNAsadata;
}

std::vector<std::vector<double>> Wrappingradialstation(std::vector<std::vector<double>> Nasadata, double r)
{
	double sigma1l = 0;
	double sigma1c = 0;
	double sigma1u = 0;
	double sigma2l = 0;
	double sigma2c = 0;
	double sigma2u = 0;
	double s = 0;


	int length = Nasadata.size();


	std::vector<std::vector<double>> travildata;
	travildata.resize(length, std::vector<double>(9, 0));
	//cout << "check\n";

	for (size_t i = 0; i < length; i++)
	{
		//cout << "\n" << r;
		//sigma2l = Nasadata[i][0] / r;
		//cout << "\nx at top:" << Nasadata[i][1] << "\t" << r;

		sigma2c = abs(Nasadata[i][1]) / r;
		sigma2u = abs(Nasadata[i][2]) / r;

		//cout << "\n sigma2:" << sigma2c;
		//cout << "\n" << sigma2u;


		//sigma1l = PI / 2 - sigma2l;
		sigma1c = (PI / 2) - sigma2c;
		sigma1u = (PI / 2) - sigma2u;

		//cout << "\n sigma1:" << sigma1c;

		//travildata[i][0] = r * cos(sigma1l);
		if (Nasadata[i][1]>=0)
		{
			travildata[i][1] = r * cos(sigma1c);
			//cout << "\n xnewtop:" << travildata[i][1];
			
		}
		else if (Nasadata[i][1]<0)
		{
			travildata[i][1] = -r * cos(sigma1c);
			//cout << "\n xnewtop:" << travildata[i][1];
		}

		if (Nasadata[i][2]>=0)
		{
			travildata[i][2] = r * cos(sigma1u);

		}
		else if (Nasadata[i][2]<0)
		{
			travildata[i][2] = -r * cos(sigma1u);
		}

		//travildata[i][3] = Nasadata[i][3];
		travildata[i][4] = Nasadata[i][4];
		travildata[i][5] = Nasadata[i][5];

		//travildata[i][6] = r * sin(sigma1l);
		travildata[i][7] = r * sin(sigma1c);
		travildata[i][8] = r * sin(sigma1u);
		//cout << "\n" << travildata[i][1] << " " << travildata[i][4] << " " << travildata[i][7] << " " << travildata[i][2] << " " << travildata[i][5] << " " << travildata[i][8];
	}

	return travildata;
}

std::vector<std::vector<double>> fileprep_Inventor(std::vector<std::vector<double>> Nasadata) {

	int length = Nasadata.size();
	std::vector<std::vector<double>> travildata;
	travildata.resize(length, std::vector<double>(6, 0));
	int oajsndow = 0;


	for (size_t i = 1; i < length; i++)
	{
		travildata[length-i][0] = Nasadata[i][1];

		travildata[length-i][2] = Nasadata[i][4];
		
		travildata[length-i][4] = Nasadata[i][7];
		//cout << "\n z top to file:" << travildata[length - i][4];
	}
	for (size_t i = 0; i < length; i++)
	{
		travildata[i][1] = Nasadata[i][2];
		travildata[i][3] = Nasadata[i][5];
		travildata[i][5] = Nasadata[i][8];
	}
	

	//travildata.erase(travildata.begin()+( length-1));





	return travildata;
}

double heightpoints(std::vector<std::vector<double>> Nasadata) {

	int length = Nasadata.size();
	vector<double> HIghpoint;
	HIghpoint.resize(length, 0);

	vector<double> HIghpoint2;
	HIghpoint2.resize(length, 0);

	vector<double> LOwpoint;
	LOwpoint.resize(length, 0);

	vector<double> LOwpoint2;
	LOwpoint2.resize(length, 0);


	for (size_t i = 0; i < length; i++)
	{

		HIghpoint[i] = Nasadata[i][4];
		HIghpoint2[i] = Nasadata[i][5];
		LOwpoint[i] = Nasadata[i][4];
		LOwpoint2[i] = Nasadata[i][5];


	}

	std::vector<double>::iterator maxu ;
	maxu = std::max_element(HIghpoint.begin(), HIghpoint.end());


	std::vector<double>::iterator maxl;
	maxl = std::max_element(HIghpoint2.begin(), HIghpoint2.end());
	double highestpointsofar;

	if (HIghpoint[std::distance(HIghpoint.begin(), maxu)] >= HIghpoint2[std::distance(HIghpoint2.begin(), maxl)])
	{
		highestpointsofar = HIghpoint[std::distance(HIghpoint.begin(), maxu)];
	}
	else if (HIghpoint[std::distance(HIghpoint.begin(), maxu)] < HIghpoint2[std::distance(HIghpoint2.begin(), maxl)])
	{
		highestpointsofar = HIghpoint2[std::distance(HIghpoint2.begin(), maxl)];

	}

	std::vector<double>::iterator lowu;
	lowu = std::min_element(LOwpoint.begin(), LOwpoint.end());


	std::vector<double>::iterator lowl;
	lowl = std::min_element(LOwpoint2.begin(), LOwpoint2.end());

	
	double lowestpointsofar;

	if (LOwpoint[std::distance(LOwpoint.begin(), lowu)] >= LOwpoint2[std::distance(LOwpoint2.begin(), lowl)])
	{
		 lowestpointsofar = LOwpoint[std::distance(LOwpoint.begin(), lowu)];
	}
	else if (LOwpoint[std::distance(LOwpoint.begin(), lowu)] < LOwpoint2[std::distance(LOwpoint2.begin(), lowl)])
	{
		 lowestpointsofar = LOwpoint2[std::distance(LOwpoint2.begin(), lowl)];

	}


	double Hoogte = highestpointsofar - lowestpointsofar;
	//cout << "\n"<<Hoogte;

	std::vector<std::vector<double>> maxandmin;
	maxandmin.resize(1, std::vector<double>(2, 0));
	maxandmin[0][0] = highestpointsofar;
	maxandmin[0][1] = lowestpointsofar;



	return Hoogte;
}

std::vector<std::vector<double>> Rotate(std::vector<std::vector<double>> Nasadata, int staggerang)
{
	int length = Nasadata.size();
	std::vector<std::vector<double>> newNAsadata;
	newNAsadata.resize(length, std::vector<double>(9, 0));

	for (size_t i = 0; i < length; i++)
	{
		newNAsadata[i][0] = (Nasadata[i][0])* cos(-staggerang * (PI / 180)) - (Nasadata[i][3])* sin(-staggerang * (PI / 180));

		newNAsadata[i][1] = (Nasadata[i][1])* cos(-staggerang * (PI / 180)) - (Nasadata[i][4])* sin(-staggerang * (PI / 180));
		newNAsadata[i][2] = (Nasadata[i][2])* cos(-staggerang * (PI / 180)) - (Nasadata[i][5])* sin(-staggerang * (PI / 180));

		newNAsadata[i][3] = (Nasadata[i][3])* cos(-staggerang * (PI / 180)) + (Nasadata[i][0]) * sin(-staggerang * (PI / 180));

		newNAsadata[i][4] = (Nasadata[i][4]) * cos(-staggerang * (PI / 180)) + (Nasadata[i][1])* sin(-staggerang * (PI / 180));
		newNAsadata[i][5] = (Nasadata[i][5])* cos(-staggerang * (PI / 180)) + (Nasadata[i][2])* sin(-staggerang * (PI / 180));

		newNAsadata[i][6] = Nasadata[i][6];

		newNAsadata[i][7] = Nasadata[i][7];
		newNAsadata[i][8] = Nasadata[i][8];



	}


	return newNAsadata;
}

std::vector<std::vector<double>> rotatingsearch(std::vector<std::vector<double>> Nasadata) {

	
		int length = 360;
		std::vector<double>	points;
		points.resize(length,0);

		for (size_t i = 0; i < length; i++)
		{
			points[i] = heightpoints(Rotate(Nasadata,i));

		}
		std::vector<double>::iterator min;
		min = std::min_element(points.begin(), points.end());

		int min_pos = distance(points.begin(), min);

		int length1 = Nasadata.size();
		std::vector<std::vector<double>> rotateddata;
		rotateddata.resize(length1, std::vector<double>(9, 0));

		cout << min_pos;
		rotateddata = Rotate(Nasadata, min_pos);

		return rotateddata;

	
	
}

std::vector<std::vector<double>> splitfind(std::vector<std::vector<double>> Nasadata, int kk, int bb) {

	double largest = 0;
	double second = 0;
	int rows = Nasadata.size();
	std::vector<double> myfile;
	myfile.resize(rows*2-1, 0);
	int rowsnew = myfile.size();


	//cout << "\n This is nummer: top " << kk;
	for (size_t j = 1; j < rows; j++)
	{

		myfile[j-1] = Nasadata[j][0];
		//cout << "\n\n";
		//cout << Nasadata[j][0] << "\t";
		//cout << myfile[j - 1];
		
	}
	//cout << "\n This is nummer: bot " << kk;
	for (size_t j = 0; j < rows; j++)
	{
		//cout << "\n\n";
		myfile[j+ rows-1] = Nasadata[j][1];
		//cout << Nasadata[j][1] << "\t";
		//cout << myfile[j + rows - 1];
	}

	std::vector<double>::iterator min;
	min = std::min_element(myfile.begin(), myfile.end());
	int min_pos = distance(myfile.begin(), min);

	std::vector<double>::iterator max;
	max = std::max_element(myfile.begin(), myfile.end());
	int max_pos = distance(myfile.begin(), max);
	int splitposition = 0;

	if (min_pos == 0 || min_pos == rowsnew-1)
	{
		splitposition = max_pos;

	}
	else if (max_pos == 0 || max_pos == rowsnew-1)
	{
		splitposition = min_pos;
	}
	
	std::vector<std::vector<double>>allthedata;

	allthedata.resize(rowsnew, std::vector<double>(6, 0));
	int i = 0;

	for (size_t i = 1; i < rows; i++)
	{
	
			allthedata[i - 1][0] = Nasadata[i][0];
			allthedata[i - 1][2] = Nasadata[i][2];
			allthedata[i - 1][4] = Nasadata[i][4];

	}

	for (size_t i = 0; i < rows; i++)
	{
	
			allthedata[i + rows - 1][1] = Nasadata[i][1];
			allthedata[i + rows - 1][3] = Nasadata[i][3];
			allthedata[i + rows - 1][5] = Nasadata[i][5];

	}


	std::vector<std::vector<double>>upper;
	
	std::vector<std::vector<double>>lower;

	upper.resize(splitposition+1, std::vector<double>(6, 0));
	
	for (size_t i = 0; i < splitposition+1; i++)
	{
		

		if (i<rows-1)
		{
				upper[i][0] = allthedata[i][0];
				upper[i][2] = allthedata[i][2];
				upper[i][4] = allthedata[i][4];
		}
		else if (i>=rows-1)
		{
				upper[i][0] = allthedata[i][1];
				upper[i][2] = allthedata[i][3];
				upper[i][4] = allthedata[i][5];
		}
		//cout << "\n" << upper[i][0] << "\t" << upper[i][2] << "\t" << upper[i][4] << "\t";
	}


	Outputtofile(upper,kk,0);



	lower.resize(rowsnew - splitposition, std::vector<double>(6, 0));
	
	for (size_t i = splitposition; i < rowsnew; i++)
	{


			if (i <rows-1)
			{
				lower[i - splitposition][1] = allthedata[i][0];
				lower[i - splitposition][3] = allthedata[i][2];
				lower[i - splitposition][5] = allthedata[i][4];
			}
			else if (i>=rows-1)
			{
				lower[i - splitposition][1] = allthedata[i][1];
				lower[i - splitposition][3] = allthedata[i][3];
				lower[i - splitposition][5] = allthedata[i][5];
			
			}
			//cout << "\n" << lower[i - splitposition][1] << "\t" << lower[i - splitposition][3] << "\t" << lower[i - splitposition][5] << "\t";
	}

	Outputtofile(lower, kk+bb,1);



	return Nasadata;
}

string GetFileName(const string & prompt) {

	const int BUFSIZE = 1024;
	char buffer[BUFSIZE] = { 0 };
	OPENFILENAME ofns = { 0 };
	ofns.lStructSize = sizeof(ofns);
	ofns.lpstrFile = buffer;
	ofns.nMaxFile = BUFSIZE;
	ofns.lpstrTitle = prompt.c_str();
	GetOpenFileName(&ofns);
	return buffer;
}


std::vector<std::vector<double>> Splineinterpolation(std::vector<std::vector<double>> Nasadata)
{
	tk::spline newspline;
	tk::spline newspline2;

	int length = Nasadata.size();
	std::vector<double> nasasdatax1;
	std::vector<double> nasasdatay1;
	std::vector<double> nasasdatay2;

	for (size_t i = 0; i < length; i++)
	{
		nasasdatax1.push_back(Nasadata[i][0]);
		nasasdatay1.push_back(Nasadata[i][2]);
		nasasdatay2.push_back(Nasadata[i][3]);
	}

	newspline.set_points(nasasdatax1, nasasdatay1);
	newspline2.set_points(nasasdatax1, nasasdatay2);

	vector<double> xcordenits;
	vector<double> xcordenits2;
	//xcordenits.resize(length);
	//xcordenits2.resize(length);


	

	for (size_t i = 16; i < length; i++)
	{
		xcordenits.push_back(Nasadata[i][0]);
		xcordenits2.push_back(Nasadata[i][0]);
	}

	int length2 = xcordenits.size();
	vector<double> ycordenite;
	vector<double> ycordenite2;

	for (size_t i = 0; i < length2; i++)
	{
		ycordenite.push_back(newspline(xcordenits[i]));
		//cout << "\n" << ycordenite[i];
		//cout <<"\n"<< ycordenite[i]<<"\t"<< Nasadata[i][4];
		ycordenite2.push_back(newspline2(xcordenits2[i]));
		//cout << "\n" << ycordenite2[i];

	}

	

	return Nasadata;

}

void Outputtofile2(std::vector<std::vector<double>> filedataready, int kk) {

	std::ostringstream oss;
	oss << kk << ".xls";
	std::string var = oss.str();
	std::ofstream myfile;
	myfile.open(var, std::ofstream::out | std::ofstream::app);
	int	rows = filedataready.size();

	int j = 0;
	int i = 0;

	//for (size_t PP = 0; PP < 2; PP++)
	//{
		for (j = 0; j <(rows); j++)
		{

			myfile << filedataready[j][0] << "\t" << filedataready[j][1] << "\t" << filedataready[j][2] << "\t" << filedataready[j][4] << "\t" << filedataready[j][4] << "\t" << filedataready[j][5] << "\t" << filedataready[j][6] << "\t" << filedataready[j][7] << "\t" << filedataready[j][8];

			myfile << "\n";

		}
	//}


	myfile.close();


}