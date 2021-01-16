#include "netman.cpp"
using namespace std;

class core
{

private:
    netman *tree;

    /* Prints formatted strings */
    ostream &format_output(const string line, const string style)
    {
        return cout << style << line << RSCLR;
    }

    /* Prints formatted error message/info*/
    const void print_line(const string attribute, const string value, const string bg)
    {
        format_output(attribute, bg);
        format_output('\t' + value, GRCLR) << endl;
    }

    /* Handles device assignation to subnet */
    const void assign_device()
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
            ret_code = tree->attach_device(subnet_name, dev_name, true);
        else
            ret_code = tree->attach_device(subnet_name, dev_name, false);

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

        default:
            print_line("system:", "address assigned to device", BLUBG);
            break;
        }
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

    /* Prints the list of commands */
    const void print_commands()
    {
        cout << "List of commands: " << endl;
        cout << "\n";
        print_line("help  ", "Show list of commands", BLUBG);
        print_line("show  ", "Show all subnetworks", BLUBG);
        print_line("add   ", "Add new subnetwork", BLUBG);
        print_line("clear ", "Delete all subnetworks", BLUBG);
        print_line("assign", "Popolate subnetwork", BLUBG);
        cout << "\n";
    }

    /* Initialize networks tree*/
    const int init_tree()
    {
        string tmp; // entire input

        string range; //ip
        int prefix;

        cout << "Input the range of address available on the network (es: x.x.x.x/x): ";
        cin >> tmp;

        for (int i = 0; i < tmp.length(); i++)
        {
            if (tmp[i] == '/')
            {
                range = tmp.substr(0, i);
                prefix = stoi(tmp.substr(i + 1, tmp.length()));
            }
        }

        if (prefix > MAX_ADDR_LEN)
        {
            print_line("error", "Invalid prefix", REDBG);
            return -1;
        }

        else
            tree = new netman(range, prefix);

        cout << endl;
        return 0;
    }

    /* Prints any device in formatted way */
    const void print_dev(netface *dev)
    {
        print_line(dev->name + ':', "", GRCLR);
        print_line("\taddress:", dev->address, REDCLR);
        if (dev->router)
            print_line("\ttype:", "\trouter", REDCLR);
        else
        {
            print_line("\tgateway:", dev->gateway, REDCLR);
            print_line("\ttype:", "\thost", REDCLR);
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
        for (int i = 0; i < pow(2, MAX_ADDR_LEN - net->prefix); i++)
        {
            if (net->devices[i])
                print_dev(net->devices[i]);
        }
    }

    /* Prints all subnetwork in formatted way */
    const void show_nets()
    {
        vector<subnet *> nets = *tree->get_all_net();

        for (auto &net : nets)
            print_net(net);
    }

    /* Adds new subnet to networks tree */
    const void add_subnet()
    {
        int max_hosts;
        string net_name;

        cout << "Input subnet name: ";
        cin >> net_name;

        cout << "Input max hosts: ";
        cin >> max_hosts;

        if (tree->add_subnetwork(max_hosts, net_name) == -1)
            print_line("error:", "subnet name already in use", REDBG);
    }

    /* Deletes all subnetworks in the network */
    const void clear_subnets()
    {
        tree->clear_all();
        print_line("system:", "all subnetworks deleted", BLUBG);
    }

public:
    /* Returns -1 if initialization fail */
    const int init()
    {
        print_header();
        return init_tree();
    }

    /* Parse user input */
    const void execute_command()
    {
        print_commands();
        while (1)
        {
            string command;
            cout << "\nInput command: ";
            cin >> command;

            if (command == "show")
                show_nets();

            else if (command == "help")
                print_commands();

            else if (command == "add")
                add_subnet();

            else if (command == "clear")
                clear_subnets();

            else if (command == "assign")
                assign_device();

            else
                print_line("error:", "invalid command", REDBG);
        }
    }
};