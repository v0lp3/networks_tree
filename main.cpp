#include <iostream>
#include "core.cpp"
using namespace std;

int main()
{
	core system;

	if (system.init() != -1)
		system.execute_command();
}