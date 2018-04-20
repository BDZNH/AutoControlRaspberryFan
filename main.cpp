#include<iostream>
#include<fstream>
#include<unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define _FANPIN 8
using namespace std;

double GetCpuTempera();
int initWiringPi();
void showInfo();

int main()
{
	showInfo();
	double temp = 0;
	bool Fan_is_open = false;
	if (initWiringPi() < 0)
	{
		return -1;
	}
	while (true)
	{
		temp = GetCpuTempera();
		if (temp >= 42 )
			cout << "Cpu temperature is : \033[0;31m" <<temp << flush << "°C \033[0m";
		else
			cout << "Cpu temperature is : \033[1;32m" <<temp << flush << "°C \033[0m";
		if (Fan_is_open)
		{
			if (temp < 39.0)
			{
				Fan_is_open = false;
				sleep(10);
				softPwmWrite(_FANPIN, 0);
			}
			sleep(10);
		}
		else
		{	
			if (temp >40 && temp < 41)
			{
				Fan_is_open = true;
				softPwmWrite(_FANPIN, 60);
				cout << "set fan speed 60"<<flush;
				sleep(5);
			}
			if (temp >=41 && temp < 42)
			{
				Fan_is_open = true;
				softPwmWrite(_FANPIN, 70);
				cout << "set fan speed 70"<<flush;
				sleep(5);
			}
			if (temp >=42 && temp < 43)
			{
				Fan_is_open = true;
				softPwmWrite(_FANPIN, 80);
				cout << "set fan speed 80"<<flush;
				sleep(5);
			}
			if (temp >=44 && temp < 45)
			{
				Fan_is_open = true;
				softPwmWrite(_FANPIN, 90);
				cout << "set fan speed 90"<<flush;
				sleep(5);
			}
			if (temp > 45.0)
			{
				Fan_is_open = true;
				softPwmWrite(_FANPIN, 100);
				cout << "set fan speed 100"<<flush;
				sleep(60);
			}
		}
		sleep(1);
		cout<<"\r";
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
