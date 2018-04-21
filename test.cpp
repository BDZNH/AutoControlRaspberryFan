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
int main ()
{
	if (initWiringPi() < 0)
	{
		return -1;
	}
	int a;
	while(cin>>a && a>0)
	{
		softPwmWrite(_FANPIN, a);
	}
	softPwmWrite(_FANPIN, 0);
	cin>>a;
	return 0;
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

