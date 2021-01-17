using namespace std;

class netview
{

public:
    /* Prints formatted strings */
    static ostream &format_output(const string line, const string style)
    {
        return cout << style << line << RSCLR;
    }

    /* Print formatted line message */
    static const void print_line(const string attribute, const string value, const string bg)
    {
        format_output(attribute, bg);
        format_output('\t' + value, GRCLR) << endl;
    }

    /* Prints system info message */
    static const void print_info(const string info)
    {
        print_line("system:", info, BLUBG);
    }

    /* Prints system error message */
    static const void print_error(const string info)
    {
        print_line("error:", info, REDBG);
    }

    static const void evaluate_add(int ret_code)
    {
        switch (ret_code)
        {
        case 0:
            print_info("device added to subnet");
            break;
        case -1:
            print_error("subnet name already in use");
            break;

        case -2:
            print_error("gateway net unavailable");
            break;
        default:
            print_error("unknown error, device not added");
            break;
        }
    }

    static const void evaluate_remove(int ret_code)
    {
        switch (ret_code)
        {
        case 1:
            print_info("all subnetworks removed");
            break;

        case -1:
            print_error("device not found");
            break;

        case -2:
            print_error("subnetwork not found");
            break;

        default:
            print_info("target removed");
            break;
        }
    }

    static const void evaluate_assign(int ret_code)
    {
        switch (ret_code)
        {
        case -1:
            print_error("subnet doesn't exists");
            break;

        case -2:
            print_error("device name already in use");
            break;

        case -3:
            print_error("add at least one router first");
            break;

        case -4:
            print_error("no address available");
            break;

        default:
            print_info("address assigned to device");
            break;
        }
    }
};