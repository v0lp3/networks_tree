using namespace std;
class netutil4 : public netutil
{
private:
    const string bin_to_ipv4(const string bits)
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

public:
    /* Parses ipv4 (e.g. 255) to int  */
    const int parse_ipv4_octet(const string number)
    {
        int integer = 0; //max 255

        for (int i = 0; i < number.length(); i++)
            integer += (number[number.length() - 1 - i] - '0') * pow(10, i);

        return (integer < 256) ? integer : -1;
    }

    /* ipv4 ip builder */
    string bin_to_ip(const string bits)
    {
        if (bits.length() != MAX_IPV4_LEN)
            return ERROR_LEN;
        return bin_to_ipv4(bits);
    }

    /* Returns 4 binary octets */
    string ip_to_bin(const string address)
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

        return (bits.length() == MAX_IPV4_LEN) ? bits : ERROR_LEN;
    }

    int get_bound(const int prefix_length)
    {
        return pow(2, MAX_IPV4_LEN - prefix_length);
    }

    int get_remain_bit(const int addr_len)
    {
        return MAX_IPV4_LEN - addr_len;
    }
};