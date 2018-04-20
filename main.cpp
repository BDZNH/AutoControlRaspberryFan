#include <iostream>
#include <fstream>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define LOG_PATH "/tmp/fanspped.log"
#define _FANPIN 8
using namespace std;

double GetCpuTempera();
int initWiringPi();
void showInfo();

int main()
{
	showInfo();
	ofstream log(LOG_PATH,ios::out | ios::app);
	if(!log.is_open())
	{
		cout<<"Cant open file :"<<LOG_PATH<<endl;
	}
	double temp = 0;
	bool Fifty_Flag = false;
	if (initWiringPi() < 0)
	{
		return -1;
	}
	while (true)
	{
		temp = GetCpuTempera();
		if (temp >= 42 )
			cout << "Cpu temperature is : \033[0;31m" <<temp <<"°C \033[0m";
		else
			cout << "Cpu temperature is : \033[1;32m" <<temp <<"°C \033[0m";
		if (Fifty_Flag)
		{
			if (temp < 39.0)
			{
				Fifty_Flag = false;
				delay(1000);
				softPwmWrite(_FANPIN, 0);
			}
			else
			{
				delay(1000);
			}
		}
		else
		{				
			if (temp < 39.0)
			{
				delay(1000);
				softPwmWrite(_FANPIN, 0);
			}
			if (temp >40.0 && temp < 41.0)
			{
				softPwmWrite(_FANPIN, 60);
				//log << "  set fan speed 60"<<endl;
			}
			else if (temp >=41.0 && temp < 42.0)
			{
				softPwmWrite(_FANPIN, 70);
				//log << "  set fan speed 70"<<endl;
				//cout << "speed 70" <<flush;
				//log.clear();
				//log.seekg(0, ios::beg);
			}
			else if (temp >=42.0 && temp < 43.0)
			{
				softPwmWrite(_FANPIN, 80);
				//log << "  set fan speed 80"<<endl;
				//cout << " speed 80 " <<flush;
				//log.clear();
				//log.seekg(0, ios::beg);
			}
			else if (temp >=43.0 && temp < 45.0)
			{
				softPwmWrite(_FANPIN, 90);
				//log << "  set fan speed 90"<<endl;
				//cout << "Speed 90" <<flush;
				//log.clear();
				//log.seekg(0, ios::beg);
			}
			else if (temp > 45.0)
			{
				softPwmWrite(_FANPIN, 100);
				//log << "  set fan speed 100"<<endl;
				//cout << "Speed 100" <<flush;
				Fifty_Flag = true ;
				//log.clear();
				//log.seekg(0, ios::beg);
				delay(5000);
			}
		}
		delay(2000);
		cout<<flush<<"\r";
	}
	return 0;
}

double GetCpuTempera()
{
	ifstream fin(TEMP_PATH, ios_base::in);
	if (!fin.is_open())
	{
		cout << "Cant open file: " << TEMP_PATH << endl;
		return -1;
	}
	int temp;
	fin >> temp;
	double tem = temp / 1000.0;
	fin.clear();
	fin.seekg(0, ios::beg);
	return tem;
}

int initWiringPi()
{
	if (wiringPiSetup() != 0)
	{
		cout << "WiringPi setup failed" << flush << " \r";
		return -1;
	}
	if (softPwmCreate(_FANPIN, 0, 100) != 0)
	{
		cout << "WiringPi setup failed" << flush << " \r";
		return -2;
	}
	return 0;
}


void showInfo()
{
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "        Project Name : AutoControlRaspberryfan" << endl;
	cout << "        Author       : BDZNH" << endl;
	cout << "        Project URL  : https://github.com/BDZNH/AutoControlRaspberryFan" << endl;
	cout << "        what is this : Auto control raspberry fan with 5V. Turn the fan" << endl;
	cout << "                       when the temperaure is high than 45°C, turn off " << endl;
	cout << "                       fan when the CPU temperature is lower than 40°C." << endl;
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "\n\n\n" << endl;
}
