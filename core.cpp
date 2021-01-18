#include "netman.cpp"
#include "netview.cpp"

using namespace std;

class core
{

private:
    netman *tree;

    /// Print functions ///

    /* Prints info */
    const void print_header()
    {
        system(" clear ");
        netview::format_output("  ## Networks Tree // ", WHIBG) << endl;
        netview::print_line("author:", "Andrea Maugeri", REDCLR);
        cout << "\n";
    }

    /* Prints any device in formatted way */
    const void print_dev(netface *dev)
    {
        if (dev)
        {
            netview::format_output(dev->name + ':', GRCLR) << endl;
            ;
            netview::print_line("\taddress:", dev->address, REDCLR);

            (dev->router) ? netview::print_line("\ttype:", "\trouter", REDCLR) : netview::print_line("\ttype:", "\thost", REDCLR);

            if (dev->router == false)
                netview::print_line("\tgateway:", dev->gateway->address, REDCLR);
        }
    }

    /* Prints recursively path from subnet level 1 to desired subnet */
    const void print_gateway_ancient(subnet *net)
    {
        if (net && net->gateway.first)
            print_gateway_ancient(tree->get_net_by_gateway(net->gateway.first->name, net->level));

        netview::format_output(net->name, WHIBG) << " >> ";
    }

    /* Prints any subnetwork in formatted way */
    const void print_net(subnet *net)
    {
        cout << "\n";

        if (net->gateway.first) // subnet level > 1
        {
            print_gateway_ancient(tree->get_net_by_gateway(net->gateway.first->name, net->level));
            netview::format_output(net->gateway.first->name, WHIBG) << " >> "; // router name
        }

        netview::format_output(net->name, WHIBG) << endl; // subnetwork name

        // range of address available
        netview::format_output("addresses: ", GRCLR);
        cout << net->first_addr << " - " << net->last_addr << endl;

        netview::format_output("mask: ", GRCLR) << net->prefix << endl; // total prefix length

        netview::format_output("level: ", GRCLR) << net->level << endl; // subnet level

        // list of attached devices
        for (int i = 0; i < netutil::get_bound(net->prefix); i++)
            print_dev(net->addressable[i]);
    }

    /// Commands ///

    /* Prints all subnetwork in formatted way */
    void show_command()
    {
        vector<subnet *> nets = *tree->get_all_nets();

        for (auto &net : nets)
            print_net(net);
    }

    /* Prints the list of commands */
    void help_command()
    {
        cout << "List of commands: " << endl;
        cout << "\n";
        netview::print_line("help  ", "Show list of commands", BLUBG);
        netview::print_line("show  ", "Show all subnetworks", BLUBG);
        netview::print_line("add   ", "Add new subnetwork", BLUBG);
        netview::print_line("assign", "Popolate subnetwork", BLUBG);
        netview::print_line("remove", "Remove device e/o subnetwork", BLUBG);
        netview::print_line("write ", "Generate all interfaces file", BLUBG);
        cout << "\n";
    }

    /* Adds new subnet to networks tree */
    void add_command()
    {
        int max_devices;
        string net_name;

        // subnet level > 1
        string gateway_name;
        string domain_name;

        cout << "Input subnet name: ";
        cin >> net_name;

        cout << "Input max devices: ";
        cin >> max_devices;

        if (tree->get_all_nets()->size() > 0)
        {
            cout << "Input subnet domain name: ";
            cin >> domain_name;

            cout << "Input gateway router name: ";
            cin >> gateway_name;
        }

        int ret_code = tree->add_subnet(max_devices, net_name, gateway_name, domain_name);

        if (ret_code == 0)
            tree->add_dev(net_name, DEFROUTER, true);

        netview::evaluate_add(ret_code);
    }

    /* Remove target in network tree */
    void remove_command()
    {
        string input;
        string subnet_name;
        int ret_code = -2;
        cout << "Input [net|dev|all]: ";
        cin >> input;

        if (input == "net") //subnet
        {
            cout << "Input subnetwork name: ";
            cin >> subnet_name;

            ret_code = tree->remove_net_by_name(subnet_name);
        }

        else if (input == "dev") //device
        {
            string dev_name;

            cout << "Input subnetwork name: ";
            cin >> subnet_name;

            cout << "Input device name: ";
            cin >> dev_name;

            ret_code = tree->remove_dev_by_name(dev_name, subnet_name);
        }

        else if (input == "all")
            ret_code = tree->set_subnets_empty();

        netview::evaluate_remove(ret_code);
    }

    /* Handles device assignation to subnet */
    void assign_command()
    {
        string subnet_name;
        string dev_name;
        string is_router;

        int ret_code;

        cout << "Input subnet name: ";
        cin >> subnet_name;

        cout << "Device is router? (y/N): ";
        cin >> is_router;

        cout << "Input device name: ";
        cin >> dev_name;

        if (is_router == "y")
            ret_code = tree->add_dev(subnet_name, dev_name, true);
        else
            ret_code = tree->add_dev(subnet_name, dev_name, false);

        netview::evaluate_assign(ret_code);
    }

    /* Initialize networks tree*/
    const int init_tree()
    {
        string range; //ip
        int prefix;

        cout << "Input the range of address available on the network (es: x.x.x.x/x): ";

        netutil::parse_address_input(range, prefix);

        tree = (prefix > MAX_ADDR_LEN) ? NULL : new netman(range, prefix);

        if (tree == NULL)
        {
            netview::print_error("invalid prefix");
            return -1;
        }

        return 0;
    }

public:
    /* Returns -1 if initialization fail */
    const int init()
    {
        print_header();

        if (init_tree() == -1)
        {
            return -1;
        }

        return 0;
    }

    /* Returns tree instance */
    netman *get_tree_instance()
    {
        return tree;
    }

    /* Parse user input */
    const netman *execute_command()
    {
        help_command();
        while (1)
        {
            string command;
            cout << "\nInput command: ";
            cin >> command;

            if (command == "show")
                show_command();

            else if (command == "help")
                help_command();

            else if (command == "add")
                add_command();

            else if (command == "assign")
                assign_command();

            else if (command == "remove")
                remove_command();

            else if (command == "write")
                return tree;

            else
                netview::print_error("invalid command");
        }
        return 0;
    }
};