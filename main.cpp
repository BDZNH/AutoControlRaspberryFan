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
		cout << "        Cpu temperature is : " << temp << flush << "°C \r";
		if (Fan_is_open)
		{
			if (temp < 40.0)
			{
				Fan_is_open = false;
				softPwmWrite(_FANPIN, 0);
				continue;
			}
		}
		else
		{
			if (temp > 45.0)
			{
				Fan_is_open = true;
				softPwmWrite(_FANPIN, 100);
			}
		}
		sleep(1);
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
