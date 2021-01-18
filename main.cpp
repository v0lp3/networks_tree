#include <iostream>
#include <fstream>
#include "core.cpp"
#include "writer.cpp"

using namespace std;

int main()
{
	core system;

	if (system.init() == -1)
	{
		netview::print_error("init error");
		return -1;
	}

	const netman *networks_tree = system.execute_command();

	writer controller(system.get_tree_instance());

	controller.write();
}
