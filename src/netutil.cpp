//Set of primitive functions to handle ip address

using namespace std;

class netutil
{
public:
    /* Parses ipv4 (e.g. 255) to int  */
    static const int parse_ipv4_octet(const string number)
    {
        int integer = 0; //max 255

        for (int i = 0; i < number.length(); i++)
            integer += (number[number.length() - 1 - i] - '0') * pow(10, i);

        return (integer < 256) ? integer : -1;
    }

    static const int parse_ipv6_quartet(const string number)
    {
        int integer;

        stringstream sstream;
        sstream << hex << number;
        sstream >> integer;

        return integer;
    }

    static const string bin_to_addr(const string bits, bool ipv6)
    {
        if (bits.length() != MAX_IPV4_LEN || bits.length() != MAX_IPV6_LEN)
            return ERROR_LEN;

        if (ipv6)
            return bin_to_ipv6(const string bits);

        else
            return bin_to_ipv4(const string bits);
    }

    /* ipv4 ip builder */
    static const string bin_to_ipv4(const string bits)
    {
        string address = ""; //x.x.x.x

        int exponent = 0;
        int octet = 0; // ->  ...00001010 => x.x.x.10

        for (int pos = MAX_IPV4_LEN - 1; pos >= 0; pos--)
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

    static const string bin_to_ipv6(const string bits)
    {
        string address = "";
        stringstream sstream;
        int exponent = 0;
        int hex_n = 0;

        for (int pos = MAX_IPV6_LEN - 1; pos >= 0; pos--)
        {
            hex_n += (bits[pos] - '0') * pow(2, exponent++);

            if (exponent == 16)
            {
                sstream << hex << hex_n;
                string quartet = sstream.str();
                address = quartet + ":" + address;
                exponent = 0;
            }
        }
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
    static const string get_fixed_length(const int integer, const int fixed_length)
    {
        string bits = int_to_bin(integer); // es. 10 => |1010| < 8

        // 0 => 00000000 or 0=> 0000000000000000
        const int bound = fixed_length - bits.length();

        for (int i = 0; i < bound; i++)
            bits = '0' + bits;

        return bits;
    }

    /* Returns 4 binary octets */
    static const string ipv4_to_bin(const string address)
    {
        string bits = "";
        string tmp_number = "";

        //Parses number until reaches dot
        for (int i = 0; i < address.length(); i++)
        {
            if (address[i] == '.')
            {
                bits += get_fixed_length(parse_ipv4_octet(tmp_number), 8); // octet
                tmp_number = "";
            }
            else
                tmp_number += address[i];
        }

        bits += get_fixed_length(parse_ipv4_octet(tmp_number), 8);

        return (bits.length() == MAX_ADDR_LEN) ? bits : ERROR_LEN;
    }

    /* Returns 128 bit */
    static string ipv6_to_bin(string address)
    {
        string bits = "";
        string tmp_hex = "";
        int bitcount = 0;

        for (int i = 0; i < address.length() - 1; i++)
            if (i > 0 && address[i] == ":" && address[i + 1] != ":" && address[i - 1] != ":")
                bitcount++;

        //Parses number until reaches dot
        for (int i = 0; i < address.length(); i++)
        {
            if (i < address.length() - 1 && address[i] == ":" && address[i] == ":")
            {
                int remain_quartet = 5 - bitcount;

                while (remain_quartet != 0)
                {
                    bits += get_fixed_length(0, 16);
                    remain_quartet--;
                }
            }

            if (address[i] == ':')
            {
                bits += get_fixed_length(parse_ipv6_quartet(tmp_number), 16);
                tmp_hex = "";
            }
            else
                tmp_hex += address[i];
        }

        bits += get_fixed_length(parse_ipv6_quartet(tmp_number), 16);

        return (bits.length() == MAX_ADDR_LEN) ? bits : ERROR_LEN;
    }

    /* Returns number of address in network */
    static const int get_bound(const int prefix_length, bool ipv6)
    {
        if (ipv6)
            return pow(2, MAX_IPV6_LEN - prefix_length);
        else
            return pow(2, MAX_IPV4_LEN - prefix_length);
    }

    /* Returns ip address prefix in base 2*/
    static const string get_bin_prefix(const string address, const int prefix_len)
    {
        const string addr_bin = ip_to_bin(address); // conversion to binary
        return addr_bin.substr(0, prefix_len);
    }

    /* Returns the minimum number of levels needed */
    static const int get_total_level(const int max_devices)
    {
        const double levels = log2(max_devices);
        return (levels > int(levels)) ? levels + 1 : levels;
    }

    /* Returns 4 octets address. mode 0: first address of the network, mode 1: last address */
    static const string complete_address(const string base_address, const string path, const bool mode, bool ipv6)
    {
        string address = base_address + path; // extension of the prefix
        int remain_bit; // remain bit are available for hosts

        if (ipv6)
            remain_bit = MAX_IPV6_LEN - address.length(); 

        else
            remain_bit = MAX_IPV4_LEN - address.length();

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