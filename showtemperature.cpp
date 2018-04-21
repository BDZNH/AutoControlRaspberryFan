#include<iostream>
#include<fstream>
#include<unistd.h>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
//#define SPEED_PATH "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq"
#define MOVEX(x) printf("\033[%dA",x);
#define MOVEY(y) printf("\033[%dD",y);
#define MOVETO(x,y) printf("\033[%d,%dH",(x),(y));
using namespace std;

void GetCpuTempure(double &temp);

int main()
{
	double temp;
	while(true)
	{
		GetCpuTempure(temp);
		//cout<<temp<<endl;
		if (temp >= 42 )
			cout << "Cpu temperature is : \033[0;31m" <<temp << "°C \033[0m";
		else
			cout << "Cpu temperature is : \033[1;32m" <<temp << "°C \033[0m";
		cout<<flush;
		sleep(1);
		cout<<"\r";
		//MOVEX(7);
		//MOVEY(28);
	}
	return 0;
}

void GetCpuTempure(double &temp)
{	
	ifstream fin(TEMP_PATH,ios_base::in);
	//ifstream sin(SPEED_PATH,ios_bash::in);
	if(!fin.is_open())
	{
		cout<<"Cant open file: "<<TEMP_PATH<<endl;
		exit (1);
	}
	fin >> temp;
	temp=temp/1000.0;
	fin.clear();
	fin.seekg(0,ios::beg);
	//if(!sin.is_open())
	//{
	//	cout << "Cant open file : ">>SPEED_PATH<<endl;
	//	return -1;
	//}
	//sin >> speed;
	//speed = speed/1000.0;
	//sin.clear();
	//sin.seekg(0,ios::beg);
}
