//Set of primitive functions to handle ip address

using namespace std;

class netutil
{
public:
    /* Converts base 10 to base 2 */
    const string int_to_bin(int integer);

    /* Converts base 10 to fixed-length base 2 */
    const string get_fixed_length(const int integer, const int fixed_length);

    /* Returns ip address prefix in base 2*/
    const string get_bin_prefix(const string address, const int prefix_len);

    /* Returns the minimum number of levels needed */
    const int get_total_level(const int max_devices);

    /* Returns 4 octets address. mode 0: first address of the network, mode 1: last address */
    const string complete_address(const string base_address, const string path, const bool mode);

    /* Converts bits to address */
    virtual string bin_to_ip(const string bits) = 0;

    /* Returns number of address in network */
    virtual int get_bound(const int prefix_length) = 0;

    /* Returns number of bits that are available for hosts */
    virtual int get_remain_bit(const int addr_len) = 0;

    /* Converts address to bits */
    virtual string ip_to_bin(const string address) = 0;

    /* Parses address with mask */
    const static void parse_address_input(string &range, int &prefix)
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