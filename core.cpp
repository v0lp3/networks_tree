#include "netman.cpp"
using namespace std;

class core
{

private:
    netman *tree;

    //statefull class
    int current_level;
    int count;

    /// Print function ///

    /* Prints formatted strings */
    ostream &format_output(const string line, const string style)
    {
        return cout << style << line << RSCLR;
    }

    /* Prints info */
    const void print_header()
    {
        system(" clear ");
        cout << "## Networks Tree //" << endl;
        cout << "\n";
        print_line("credits:", "Andrea Maugeri", REDCLR);
        print_line("status: ", "Work in progress", REDCLR);
        cout << "\n";
    }

    /* Prints any device in formatted way */
    const void print_dev(netface *dev)
    {
        if (dev)
        {
            print_line(dev->name + ':', "", GRCLR);
            print_line("\taddress:", dev->address, REDCLR);

            (dev->router) ? print_line("\ttype:", "\trouter", REDCLR) : print_line("\ttype:", "\thost", REDCLR);

            if (dev->gateway && dev->gateway->address.length() > 0)
                print_line("\tgateway:", dev->gateway->address, REDCLR);
        }
    }

    /* Prints any subnetwork in formatted way */
    const void print_net(subnet *net)
    {
        cout << "\n";

        format_output(net->name, WHIBG) << endl; // subnetwork name

        // range of address available
        format_output("addresses: ", GRCLR);
        cout << net->first_addr << " - " << net->last_addr << endl;

        format_output("mask: ", GRCLR) << net->prefix << endl; // total prefix length

        // list of all devices attached
        for (int i = 0; i < netutil::get_bound(net->prefix); i++)
            print_dev(net->addressable[i]);
    }

    /* Prints formatted error message/info*/
    const void print_line(const string attribute, const string value, const string bg)
    {
        format_output(attribute, bg);
        format_output('\t' + value, GRCLR) << endl;
    }

    /// Commands ///

    /* Prints all subnetwork in formatted way */
    const void show_command()
    {
        vector<subnet *> nets = *tree->get_all_nets();

        for (auto &net : nets)
            print_net(net);
    }

    /* Prints the list of commands */
    const void help_command()
    {
        cout << "List of commands: " << endl;
        cout << "\n";
        print_line("help  ", "Show list of commands", BLUBG);
        print_line("show  ", "Show all subnetworks", BLUBG);
        print_line("add   ", "Add new subnetwork", BLUBG);
        print_line("clear ", "Delete all subnetworks", BLUBG);
        print_line("assign", "Popolate subnetwork", BLUBG);
        print_line("write ", "Generate all interfaces file", BLUBG);
        cout << "\n";
    }

    /* Adds new subnet to networks tree */
    const void add_command()
    {
        if (count == 0)
            current_level++;

        count = (count + 1) % current_level;

        int max_devices;
        string net_name;
        string gateway_name;

        cout << "Input subnet name: ";
        cin >> net_name;

        cout << "Input max devices: ";
        cin >> max_devices;

        if (current_level > 1)
        {
            cout << "Input gateway net name: ";
            cin >> gateway_name;
        }

        if (current_level == 0 || tree->check_level(gateway_name, current_level))
        {
            if (tree->add_subnetwork(max_devices, current_level, net_name) == -1)
                print_line("error:", "subnet name already in use", REDBG);
            else
                tree->add_dev(net_name, DEFROUTER, true);
        }
        else
            print_line("error:", "gateway net unavailable", REDBG);
    }

    /* Deletes all subnetworks in the network */
    const void clear_command()
    {
        tree->set_subnets_empty();
        print_line("system:", "all subnetworks deleted", BLUBG);
    }

    /* Handles device assignation to subnet */
    const void assign_command()
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

        switch (ret_code)
        {
        case -1:
            print_line("error:", "subnet doesn't exists", REDBG);
            break;

        case -2:
            print_line("error:", "device name already in use", REDBG);
            break;

        case -3:
            print_line("error:", "add at least one router first", REDBG);
            break;

        case -4:
            print_line("error:", "no address available", REDBG);
            break;

        default:
            print_line("system:", "address assigned to device", BLUBG);
            break;
        }
    }

    /* Writes configuration files */
    const void write_command()
    {
        print_line("system: ", "all files writed", BLUBG);
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
            print_line("error", "Invalid prefix", REDBG);
            return -1;
        }

        return 0;
    }

    // const void print_gateway_ancient(subnet *net)
    // {
    //     if (net && net->gateway)
    //         print_gateway_ancient(net->gateway);

    //     format_output(net->name, WHIBG) << " >> ";
    // }

public:
    core()
    {
        current_level = 0;
        count = 0;
    }

    /* Returns -1 if initialization fail */
    const int
    init()
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
    const int execute_command()
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

            else if (command == "clear")
                clear_command();

            else if (command == "assign")
                assign_command();

            else if (command == "write")
            {
                write_command();
                return 1;
            }

            else
                print_line("error:", "invalid command", REDBG);
        }
        return 0;
    }
};