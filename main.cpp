/******************************************************************************


-------------------------------------------------------------------------------
        Project Name : AutoControlRaspberryfan                                 
        Author       : BDZNH                                                   
        Project URL  : https://github.com/BDZNH/AutoControlRaspberryFan        
        what is this : Auto control raspberry fan with 5V. Turn the fan        
                       when the temperaure is high than 45°C, turn off         
                       fan when the CPU temperature is lower than 39°C.        
-------------------------------------------------------------------------------



******************************************************************************/




#include <iostream>
#include <fstream>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <ctime>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define LOG_PATH "/tmp/RaspberrypiFanSpeed.log"
#define PID_PATH "/var/run/autocontrolfan.pid"
#define _FANPIN 8
#define min(x,y) (x<=y?x:y)
using namespace std;

void GetCpuTempera(ifstream &fin,double &temp);
int initWiringPi();
void showInfo();
void SaveLog(ofstream &log, double &temp, int FanSpeed, time_t &time_cur);

int main()
{
	showInfo();
	ofstream log(LOG_PATH);
	if (!log.is_open())
	{
		cout << "Can't open file : " << LOG_PATH << endl;
	}
	log.close();
	ifstream fin(TEMP_PATH, ios_base::in);
	if (!fin.is_open())
	{
		cout << "Can't open file : " << TEMP_PATH << endl;
		return -1;
	}
	ofstream pid(PID_PATH);
	if (!pid.is_open())
	{
		cout << "Can't open file : " << PID_PATH << endl;
	}
	pid << getpid() << endl;
	pid.close();
	pid.clear();
	time_t time_cur;
	double temp = 0;
	int Fan_Speed = 0;
	bool Forty_five_Flag = false;
	if (initWiringPi() < 0)
	{
		return -1;
	}

	while (true)
	{
		GetCpuTempera(fin,temp);
		if (temp >= 42)
			cout << "Cpu temperature is : \033[0;31m" << temp << "°C   \033[0m" << flush;
		else
			cout << "Cpu temperature is : \033[1;32m" << temp << "°C   \033[0m" << flush;
		if (Forty_five_Flag)
		{
			if (temp < 39.0)
			{
				Forty_five_Flag = false;
				Fan_Speed = 0;
				softPwmWrite(_FANPIN, Fan_Speed);
				SaveLog(log, temp, Fan_Speed, time_cur);
			}
			else
			{
				SaveLog(log, temp, Fan_Speed, time_cur);
			}
			sleep(1);
		}
		else
		{
			if (temp < 39.0)
			{
				sleep(1);
				Fan_Speed = 0;
				softPwmWrite(_FANPIN, Fan_Speed);
				SaveLog(log, temp, Fan_Speed, time_cur);
			}
			else if (temp >= 40.0 && temp <= 45.0)
			{
				Fan_Speed = min(((((int)temp - 40) * 10) + 60),100);
				softPwmWrite(_FANPIN, Fan_Speed);
				SaveLog(log, temp, Fan_Speed, time_cur);
				sleep(2);
			}
			else if (temp > 45.0)
			{
				Fan_Speed = 100;
				softPwmWrite(_FANPIN, Fan_Speed);
				Forty_five_Flag = true;
				SaveLog(log, temp, Fan_Speed, time_cur);
				sleep(5);
			}
		}
		sleep(1);
		cout << "\r";
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
		cout << "softPwmcreat setup failed" << flush << " \r";
		return -2;
	}
	return 0;
}



void showInfo()
{
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "        Project Name : AutoControlRaspberryfan                                 " << endl;
	cout << "        Author       : BDZNH                                                   " << endl;
	cout << "        Project URL  : https://github.com/BDZNH/AutoControlRaspberryFan        " << endl;
	cout << "        what is this : Auto control raspberry fan with 5V. Turn the fan        " << endl;
	cout << "                       when the temperaure is high than 45°C, turn off         " << endl;
	cout << "                       fan when the CPU temperature is lower than 39°C.        " << endl;
	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "\n\n\n" << endl;
}

void SaveLog(ofstream &log, double &temp, int Fan_Speed, time_t &time_cur)
{
	log.open(LOG_PATH,ios_base::out);
	if (!log.is_open())
	{
		cout << "Can't open file : " << LOG_PATH << endl;
	}
	time(&time_cur);
	log << ctime(&time_cur) << "CPU temperature is : " << temp << "°C \nSet fan speed to " << Fan_Speed << endl;
	log.close();
	log.clear();
}