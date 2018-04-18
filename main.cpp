#include<iostream>
#include<fstream>
#include<unistd.h>
#include <wiringPi.h>
#include <softPwm.h>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define _FANPIN 8
#define MAX_SIZE 32
using namespace std;

float GetCpuTempure();
int initWiringPi();

int main()
{
    float temp=0;
    bool Fan_is_open=false;
    if (initWiringPi() < 0)
    {
        return -1;
    }
    while(true)
    {
        temp = GetCpuTempure();
        cout << "Cpu tempure is : " <<temp << flush << "°C \r";
        if(Fan_is_open)
        {
            if(temp < 40.0)
            {
                Fan_is_open=false;
                softPwmWrite(_FANPIN, 0);
                continue;
            }
        }
        else
        {
            if(temp > 45.0)
            {
                Fan_is_open=true;
                softPwmWrite(_FANPIN, 100);
            }
        }
        sleep(1);
    }
    return 0;
}

float GetCpuTempure()
{
    ifstream fin(TEMP_PATH,ios_base::in);
    if(!fin.is_open())
    {
        cout<<"Cant open file: "<<TEMP_PATH<<endl;
        return -1;
    }
    int temp;
    fin >> temp;
    float tem=temp/1000.0;
    fin.clear();
    fin.seekg(0,ios::beg);
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
