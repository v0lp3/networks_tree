//Set of primitive functions to handle ip address

using namespace std;

class netutil
{
public:
    virtual string bin_to_ip(string bits) = 0;

    /* Converts base 10 to base 2 */
    string int_to_bin(int integer)
    {
        string bin = "";

        while (integer > 0)
        {
            bin = to_string(integer % 2) + bin;
            integer /= 2;
        }

        return bin;
    }

    /* Converts base 10 to fixed-length base 2 */
    string get_fixed_length(int integer, int fixed_length)
    {
        string bits = int_to_bin(integer); // es. 10 => |1010| < 8

        // 0 => 00000000 or 0=> 0000000000000000
        int bound = fixed_length - bits.length();

        for (int i = 0; i < bound; i++)
            bits = '0' + bits;

        return bits;
    }

    virtual string ip_to_bin(string address) = 0;

    /* Returns number of address in network */
    virtual int get_bound(int prefix_length) = 0;
    /* Returns ip address prefix in base 2*/
    string get_bin_prefix(string address, int prefix_len)
    {
        string addr_bin = ip_to_bin(address); // conversion to binary
        return addr_bin.substr(0, prefix_len);
    }
    

    /* Returns the minimum number of levels needed */
    int get_total_level(int max_devices)
    {
        double levels = log2(max_devices);
        return (levels > int(levels)) ? levels + 1 : levels;
    }

    virtual int get_remain_bit(int addr_len) = 0;

    /* Returns 4 octets address. mode 0: first address of the network, mode 1: last address */
    string complete_address(string base_address, string path, bool mode)
    {
        string address = base_address + path;              // extension of the prefix
        int remain_bit = get_remain_bit(address.length()); // remain bit are available for hosts

        for (int i = 0; i < remain_bit; i++)
        {
            if (mode == 0)
                address += '0';

            else
                address += '1';
        }
        return address;
    }

    /* Parses address with mask */
    static void parse_address_input(string &range, int &prefix)
    {
        string input; // entire input
        cin >> input;

        for (int i = 0; i < input.length(); i++)
        {
            if (input[i] == '/')
            {
                range = input.substr(0, i);
                prefix = stoi(input.substr(i + 1, input.length()));
            }
        }
    }
};