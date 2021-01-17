//Set of primitive functions to handle ip address

using std::cin;
using std::string;
using std::__cxx11::to_string;

class netutil
{
public:
    /* Parses string to int  */
    static const int parse_int(const string number)
    {
        int integer = 0; //max 255

        for (int i = 0; i < number.length(); i++)
            integer += (number[number.length() - 1 - i] - '0') * pow(10, i);

        return (integer < 256) ? integer : -1;
    }

    /* Converts base 2 (ip address) to base 10 */
    static const string bin_to_ip(const string bits)
    {
        if (bits.length() != MAX_ADDR_LEN)
        {
            return ERROR_LEN;
        }

        string address = ""; //x.x.x.x

        int exponent = 0;
        int octet = 0; // ->  ...00001010 => x.x.x.10

        for (int pos = MAX_ADDR_LEN - 1; pos >= 0; pos--)
        {
            octet += (bits[pos] - '0') * pow(2, exponent++);

            if (exponent == 8) // new octet to parse
            {
                string tmp_addr = to_string(octet) + address;

                address = (pos > 0) ? "." + tmp_addr : tmp_addr;

                exponent = 0;
                octet = 0;
            }
        }

        return address;
    }

    /* Converts base 10 to base 2 */
    static const string int_to_bin(int integer)
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
    static const string get_fixed_length(const int integer, int fixed_length)
    {
        string bits = int_to_bin(integer); // es. 10 => |1010| < 8

        // 0 => 00000000
        int bound = fixed_length - bits.length();

        for (int i = 0; i < bound; i++)
            bits = '0' + bits;

        return bits;
    }

    /* Returns 4 binary octets */
    static const string ip_to_bin(const string address)
    {
        string bits = "";
        string tmp_number = "";

        //Parses number until reaches dot
        for (int i = 0; i < address.length(); i++)
        {
            if (address[i] == '.')
            {
                bits += get_fixed_length(parse_int(tmp_number), 8); // octet
                tmp_number = "";
            }
            else
                tmp_number += address[i];
        }

        bits += get_fixed_length(parse_int(tmp_number), 8);

        return (bits.length() == MAX_ADDR_LEN) ? bits : ERROR_LEN;
    }

    /* Returns number of address in network */
    static const int get_bound(const int prefix_length)
    {
        return pow(2, MAX_ADDR_LEN - prefix_length);
    }

    /* Returns ip address prefix in base 2*/
    static const string get_bin_prefix(const string address, const int prefix_len)
    {
        string addr_bin = ip_to_bin(address); // conversion to binary
        return addr_bin.substr(0, prefix_len);
    }

    /* Returns the minimum number of levels needed */
    static const int get_total_level(const int max_devices)
    {
        double levels = log2(max_devices);
        return (levels > int(levels)) ? levels + 1 : levels;
    }

    /* Returns 4 octets address. mode 0: first address of the network, mode 1: last address */
    static const string complete_address(const string base_address, const string path, const bool mode)
    {
        string address = base_address + path;             // extension of the prefix
        int remain_bit = MAX_ADDR_LEN - address.length(); // remain bit are available for hosts

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
    static const void parse_address_input(string &range, int &prefix)
    {
        string tmp; // entire input
        cin >> tmp;

        for (int i = 0; i < tmp.length(); i++)
        {
            if (tmp[i] == '/')
            {
                range = tmp.substr(0, i);
                prefix = stoi(tmp.substr(i + 1, tmp.length()));
            }
        }
    }
};