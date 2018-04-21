#include <iostream>
#include <fstream>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define LOG_PATH "/tmp/RaspberrypiFanSpeed.log"
#define _FANPIN 8
using namespace std;

void GetCpuTempera(ifstream &fin,double &temp);
int initWiringPi();
void showInfo();

int main()
{
	showInfo();
	ofstream log(LOG_PATH, ios::out);
	if (!log.is_open())
	{
		cout << "Cant open file :" << LOG_PATH << endl;
	}
	ifstream fin(TEMP_PATH, ios_base::in);
	if (!fin.is_open())
	{
		cout << "Cant open file: " << TEMP_PATH << endl;
		return -1;
	}
	double temp = 0;
	bool Forty_five_Flag = false;
	if (initWiringPi() < 0)
	{
		return -1;
	}
	while (true)
	{
		GetCpuTempera(fin,temp);
		if (temp >= 42)
			cout << "Cpu temperature is : \033[0;31m" << temp << "¡ãC \033[0m";
		else
			cout << "Cpu temperature is : \033[1;32m" << temp << "¡ãC \033[0m";
		if (Forty_five_Flag)
		{
			if (temp < 39.0)
			{
				Forty_five_Flag = false;
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
			else if (temp >= 41.0 && temp < 42.0)
			{
				softPwmWrite(_FANPIN, 70);
				//log << "  set fan speed 70"<<endl;
				//cout << "speed 70" <<flush;
				//log.clear();
				//log.seekg(0, ios::beg);
			}
			else if (temp >= 42.0 && temp < 43.0)
			{
				softPwmWrite(_FANPIN, 80);
				//log << "  set fan speed 80"<<endl;
				//cout << " speed 80 " <<flush;
				//log.clear();
				//log.seekg(0, ios::beg);
			}
			else if (temp >= 43.0 && temp < 45.0)
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
				Forty_five_Flag = true;
				//log.clear();
				//log.seekg(0, ios::beg);
				delay(5000);
			}
		}
		delay(2000);
		cout << flush << "\r";
	}
	return 0;
}

void GetCpuTempera(ifstream &fin, double &temp)
{
	fin >> temp;
	temp = temp / 1000.0;
	fin.clear();
	fin.seekg(0, ios::beg);
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
	cout << "                       when the temperaure is high than 45¡ãC, turn off " << endl;
	cout << "                       fan when the CPU temperature is lower than 40¡ãC." << endl;
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "\n\n\n" << endl;
}