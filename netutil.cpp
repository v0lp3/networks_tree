//Set of primitive functions to handle ip address

using std::string;
using std::__cxx11::to_string;

/* Parses string to int  */
const int parse_int(const string number)
{
    int integer = 0; //max 255

    for (int i = 0; i < number.length(); i++)
        integer += (number[number.length() - 1 - i] - '0') * pow(10, i);

    return (integer < 256) ? integer : -1;
}

/* Converts base 2 (ip address) to base 10 */
const string bin_to_ip(const string bits)
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
const string int_to_bin(int integer)
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
const string complete_octet(const int integer)
{
    string octet = int_to_bin(integer); // es. 10 => |1010| < 8

    // 0 => 00000000
    int bound = 8 - octet.length();

    for (int i = 0; i < bound; i++)
        octet = '0' + octet;

    return octet;
}

/* Returns 4 binary octets */
const string ip_to_bin(const string address)
{
    string bits = "";
    string tmp_number = "";

    //Parses number until reaches dot
    for (int i = 0; i < address.length(); i++)
    {
        if (address[i] == '.')
        {
            bits += complete_octet(parse_int(tmp_number));
            tmp_number = "";
        }
        else
            tmp_number += address[i];
    }

    bits += complete_octet(parse_int(tmp_number));

    return (bits.length() == MAX_ADDR_LEN) ? bits : ERROR_LEN;
}

/* Returns ip address prefix in base 2*/
const string get_bin_prefix(const string address, const int prefix_len)
{
    string addr_bin = ip_to_bin(address);
    return addr_bin.substr(0, prefix_len);
}

/* Returns 4 octets address. mode 0: first address of the network, mode 1: last address */
const string complete_address(const string base_address, const string path, const bool mode)
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