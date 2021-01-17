#include <iostream>
#include <fstream>
#include "core.cpp"
#include "writer.cpp"

using namespace std;

int main()
{
	core system;

	if (system.init() != -1)
		system.execute_command();

	writer controller(system.get_tree_instance());

	controller.write_interface("enp0s3", true);
}