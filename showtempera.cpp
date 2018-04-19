#include<iostream>
#include<fstream>
#include<unistd.h>
#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
using namespace std;

float GetCpuTempure();

int main()
{
	float temp;
	while(true)
	{
		temp = GetCpuTempure();
		if (temp >= 42 )
			cout << "Cpu temperature is : \033[0;31m" <<temp << flush << "°C                                           \033[0m";
		else
			cout << "Cpu temperature is : \033[1;32m" <<temp << flush << "°C                                           \033[0m";
		sleep(1);
		cout<<"\r";
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
