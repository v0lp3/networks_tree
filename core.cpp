#include "netman.cpp"
#include <iostream>
using namespace std;

class core
{

private:
    netman *tree;

    ostream &format_output(ostream &os, const string line, const string style)
    {
        os << style << line << RSCLR;
        return os;
    }

    void print_line(const string attribute, const string value, const string bg)
    {
        format_output(cout, attribute, bg);
        format_output(cout, '\t' + value, GRCLR) << endl;
    }

    void print_header()
    {
        system(" clear ");
        cout << "## Network Tree //" << endl;
        cout << "\n";
        print_line("credits:", "Andrea Maugeri", REDBG);
        print_line("version:", "0.0.0.1 (alpha)", REDBG);
        print_line("status: ", "Work in progress", REDBG);
        cout << "\n";
    }

    void print_commands()
    {

        cout << "List of commands: " << endl;
        cout << "\n";
        print_line("help", "Show list of commands", BLUBG);
        print_line("show", "Show all subnetworks", BLUBG);
        print_line("add ", "Add new subnetwork", BLUBG);
        cout << "\n";
    }

    int gen_tree()
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

        if (prefix > 32)
        {
            print_line("error", "Invalid prefix", REDBG);
            return -1;
        }

        else
        {

            tree = new netman(range, prefix);
        }

        cout << endl;
        return 0;
    }

    void print_net(subnet *net)
    {

        cout << "\n";
        format_output(cout, net->name, WHIGB) << endl;
        format_output(cout, "addresses: ", GRCLR);
        cout << net->first_addr << " - " << net->last_addr << endl;
        format_output(cout, "mask: ", GRCLR) << net->prefix << endl;
    }

    void show_nets()
    {

        vector<subnet *> nets = *tree->get_all();

        for (auto &net : nets)
        {
            print_net(net);
        }
    }

    void add_subnet()
    {
        int max_hosts;
        string net_name;

        cout << "Input subnet name: ";
        cin >> net_name;
        cout << "Input max hosts: ";
        cin >> max_hosts;

        tree->add_subnetwork(max_hosts, net_name);
    }

public:
    core() {}

    int init()
    {
        print_header();
        return gen_tree();
    }

    void execute_command()
    {
        print_commands();
        while (1)
        {
            string command;
            cout << "\nInput command: ";
            cin >> command;

            if (command == "show")
            {
                show_nets();
            }

            else if (command == "help")
            {
                print_commands();
            }

            else if (command == "add")
            {
                add_subnet();
            }

            else
            {
                print_line("error:", "invalid command", REDBG);
            }
        }
    }
};