#include "netman.cpp"
#include "netview.cpp"

using namespace std;

class core
{
private:
    netman *tree;

    /* Prints info */
    const void print_header();

    /* Prints any device in formatted way */
    const void print_dev(const netface *dev);

    /* Prints recursively path from subnet level 1 to desired subnet */
    const void print_gateway_ancient(const subnet *net);

    /* Prints any subnetwork in formatted way */
    const void print_net(const subnet *net);

    /* Prints all subnetwork in formatted way */
    const void show_command();

    /* Prints the list of commands */
    const void help_command();

    /* Adds new subnet to networks tree */
    const void add_command();

    /* Remove target in network tree */
    const void remove_command();

    /* Handles device assignation to subnet */
    const void assign_command();

    /* Initialize networks tree*/
    const int init_tree();

public:
    /* Returns -1 if initialization fail */
    const int init();

    /* Parse user input */
    const void execute_command();
};