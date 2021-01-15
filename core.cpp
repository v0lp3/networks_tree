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
    void print_line(const string attribute, const string value, const string bg)
    {
        format_output(attribute, bg);
        format_output('\t' + value, GRCLR) << endl;
    }

    void assign_device()
    {
        string subnet_name;
        cout << "Input subnet name: ";
        cin >> subnet_name;
        subnet *sel = tree->get_by_name(subnet_name);

        if (sel == NULL)
            print_line("error", "subnet doesn't exists", REDBG);

        else
        {
             print_line("info", "work in progress", BLUBG);
        }
    }

    /* Prints info */
    void print_header()
    {
        system(" clear ");
        cout << "## Networks Tree //" << endl;
        cout << "\n";
        print_line("credits:", "Andrea Maugeri", REDCLR);
        print_line("status: ", "Work in progress", REDCLR);
        cout << "\n";
    }

    /* Prints the list of commands */
    void print_commands()
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
    int init_tree()
    {
        string tmp;
        string range;
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

        // basic error handling
        if (prefix > 32)
        {
            print_line("error", "Invalid prefix", REDBG);
            return -1;
        }

        else
            tree = new netman(range, prefix);

        cout << endl;
        return 0;
    }

    /* Prints any subnetwork in formatted way */
    void print_net(subnet *net)
    {
        cout << "\n";
        format_output(net->name, WHIBG) << endl;
        format_output("addresses: ", GRCLR);
        cout << net->first_addr << " - " << net->last_addr << endl;
        format_output("mask: ", GRCLR) << net->prefix << endl;
    }

    /* Prints all subnetwork in formatted way */
    void show_nets()
    {
        vector<subnet *> nets = *tree->get_all();

        for (auto &net : nets)
            print_net(net);
    }

    /* Adds new subnet to networks tree */
    void add_subnet()
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

    void clear_subnets()
    {

        tree->clear_all();
        print_line("system:", "all subnetworks deleted", REDBG);
    }

public:
    core() {}

    /* Returns -1 if initialization fail */
    int init()
    {
        print_header();
        return init_tree();
    }

    /* Parse user input */
    void execute_command()
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