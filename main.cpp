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
#include <string>
#include <unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <ctime>

const unsigned int _FANPIN = 8;
const std::string TEMP_PATH = "/sys/class/thermal/thermal_zone0/temp";
const std::string LOG_PATH = "/tmp/RaspberrypiFanSpeed.log";
const std::string PID_PATH = "/var/run/autocontrolfan.pid";


inline int min(int x, int y)
{
	return x <= y ? x : y;
}

void GetCpuTempera(std::ifstream &fin,double &temp);
int initWiringPi();
void showInfo();
void SaveLog(std::ofstream &log, double &temp, int FanSpeed, time_t &time_cur);

int main()
{
	showInfo();

	
	std::ofstream log(LOG_PATH.c_str());
	if (!log.is_open())
	{
		std::cout << "Can't open file : " << LOG_PATH << std::endl;
	}
	log.close();
	std::ifstream fin(TEMP_PATH.c_str(), std::ios_base::in);
	if (!fin.is_open())
	{
		std::cout << "Can't open file : " << TEMP_PATH << std::endl;
		return -1;
	}
	std::ofstream pid(PID_PATH.c_str());
	if (!pid.is_open())
	{
		std::cout << "Can't open file : " << PID_PATH << std::endl;
	}
	pid << getpid() << std::endl;
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
			std::cout << "Cpu temperature is : \033[0;31m" << temp << "°C   \033[0m" << std::flush;
		else
			std::cout << "Cpu temperature is : \033[1;32m" << temp << "°C   \033[0m" << std::flush;
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
		std::cout << "\r";
	}
	return 0;
}

void GetCpuTempera(std::ifstream &fin, double &temp)
{
	fin >> temp;
	temp = temp / 1000.0;
	fin.clear();
	fin.seekg(0, std::ios::beg);
}

int initWiringPi()
{
	if (wiringPiSetup() != 0)
	{
		std::cout << "WiringPi setup failed" << std::flush << " \r";
		return -1;
	}
	if (softPwmCreate(_FANPIN, 0, 100) != 0)
	{
		std::cout << "softPwmcreat setup failed" << std::flush << " \r";
		return -2;
	}
	return 0;
}



void showInfo()
{
	std::cout << "-------------------------------------------------------------------------------" << std::endl;
	std::cout << "        Project Name : AutoControlRaspberryfan                                 " << std::endl;
	std::cout << "        Author       : BDZNH                                                   " << std::endl;
	std::cout << "        Project URL  : https://github.com/BDZNH/AutoControlRaspberryFan        " << std::endl;
	std::cout << "        what is this : Auto control raspberry fan with 5V. Turn the fan        " << std::endl;
	std::cout << "                       when the temperaure is high than 45°C, turn off         " << std::endl;
	std::cout << "                       fan when the CPU temperature is lower than 39°C.        " << std::endl;
	std::cout << "-------------------------------------------------------------------------------" << std::endl;
	std::cout << "\n\n\n" << std::endl;
}

void SaveLog(std::ofstream &log, double &temp, int Fan_Speed, time_t &time_cur)
{
	log.open(LOG_PATH.c_str(), std::ios_base::out);
	if (!log.is_open())
	{
		std::cout << "Can't open file : " << LOG_PATH << std::endl;
	}
	time(&time_cur);
	log << ctime(&time_cur) << "CPU temperature is : " << temp << "°C \nSet fan speed to " << Fan_Speed << std::endl;
	log.close();
	log.clear();
}