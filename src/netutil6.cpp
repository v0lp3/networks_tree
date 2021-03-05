using namespace std;

class netutil6 : public netutil
{
private:
    string bin_to_ipv6(string bits)
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
                hex_n = 0;
                exponent = 0;
                sstream.str("");
            }
        }
        return address;
    }

public:
    int parse_ipv6_quartet(string number)
    {
        int integer;

        stringstream sstream;
        sstream << hex << number;
        sstream >> integer;

        return integer;
    }

    string bin_to_addr(string bits)
    {
        if (bits.length() != MAX_IPV6_LEN)
            return ERROR_LEN;

        return bin_to_ipv6(bits);
    }

    /* Returns 128 bit */
    string ip_to_bin(string address)
    {
        string bits = "";
        string tmp_hex = "";
        string zero_hex = ""; // contains omitted zero if used short notation
        int quartet_count = 0;

        for (int i = 0; i < address.length() - 1; i++)
            if (i > 0 && address[i] == ':' && address[i + 1] != ':' && address[i - 1] != ':')
                quartet_count++;

        // 8 quartet => xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx
        int remain_quartet = 6 - bitcount;
        for (int i = 0; i < address.length(); i++)
        {
            if (i < address.length() - 1 && address[i] == ':' && address[i + 1] == ':')
            {
                while (remain_quartet > 0)
                {
                    zero_hex += get_fixed_length(0, 16);
                    remain_quartet--;
                }
            }

            else if (address[i] == ':')
            {
                bits += get_fixed_length(parse_ipv6_quartet(tmp_hex), 16);
                bits += zero_hex;
                tmp_hex = "";
                zero_hex = "";
            }

            else
                tmp_hex += address[i];
        }

        bits += get_fixed_length(parse_ipv6_quartet(tmp_hex), 16);

        return (bits.length() == MAX_IPV6_LEN) ? bits : ERROR_LEN;
    }

    int get_bound(int prefix_length)
    {
        return pow(2, MAX_IPV6_LEN - prefix_length);
    }

    int get_remain_bit(int addr_len)
    {
        return MAX_IPV6_LEN - addr_len;
    }
};