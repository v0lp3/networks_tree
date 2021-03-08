#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "core.cpp"

using namespace std;

int main()
{
	core system;

	if (system.init() == -1)
	{
		netview::print_error("init error");
		return -1;
	}

	system.execute_command();
}