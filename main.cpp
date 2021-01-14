#include <iostream>
#include "core.cpp"
using namespace std;

int main()
{

	core main;

	if (main.init() != -1)
	{
		main.execute_command();
	}
}
