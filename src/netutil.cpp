#include "netutil.hh"

using namespace std;

const string netutil::int_to_bin(int integer)
{
    string bin = "";

    while (integer > 0)
    {
        bin = to_string(integer % 2) + bin;
        integer /= 2;
    }

    return bin;
}

const string netutil::get_fixed_length(const int integer, const int fixed_length)
{
    string bits = int_to_bin(integer); // es. 10 => |1010| < 8

    // 0 => 00000000 or 0=> 0000000000000000
    int bound = fixed_length - bits.length();

    for (int i = 0; i < bound; i++)
        bits = '0' + bits;

    return bits;
}

const string netutil::get_bin_prefix(const string address, const int prefix_len)
{
    string addr_bin = ip_to_bin(address); // conversion to binary
    return addr_bin.substr(0, prefix_len);
}

const int netutil::get_total_level(const int max_devices)
{
    double levels = log2(max_devices);
    return (levels > int(levels)) ? levels + 1 : levels;
}

const string netutil::complete_address(const string base_address, const string path, const bool mode)
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