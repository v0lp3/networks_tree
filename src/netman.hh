#include <cmath>
#include <vector>
#include "netdef.hh"
#include "netutil.cpp"
#include "netutil4.cpp"
#include "netutil6.cpp"

// Binary tree that manage network bits node (netbitn)

using namespace std;

class netman
{
private:
    netbitn *root;

    int prefix_len;
    string base_addr;

    vector<subnet *> *subnetworks; // subnetworks indexing

    /* Returns initialized network bit node */
    netbitn *init_netbitn(const int level);

    /* Returns initialized network device */
    netface *init_netface(const bool is_router, const string dev_name, subnet *subnet, const string address);

    /* Function that check and index the subnet */
    const void index_subnet(subnet *net);

    /* Service procedure to allocate subnet*/
    const void _add_subnet(netbitn *node, const int level, const string net_name, bool &allocated);

    /* Service procedure to sets subnets info */
    const void _set_netmasks(netbitn *node, const string path);

public:
    // lazy init (ipv4 | ipv6)
    netutil *utility;
    int max_addr_len;

    int max_subnet_level;

    netman(const string addr, const int prefix_len, bool ipv6);

    /* Returns subnetwork by name */
    subnet *get_net_by_name(const string name);

    /* Returns device by name and level */
    subnet *get_net_by_gateway(const string dev_name, const int level);

    /* Returns vector of subnetworks by level */
    const vector<subnet *> get_nets_by_level(const int level);

    /* Returns vector of subnetworks pointers*/
    const vector<subnet *> *get_all_nets();

    /* Returns all subnetworks by related gateway router */
    const vector<pair<netface *, int>> get_netface_by_gateway(netface *dev, const int prefix);

    /* Returns device by name and subnet */
    netface *get_dev_by_name(const string dev_name, const subnet *net);

    /* Returns all devices addressed in subnet*/
    const vector<netface *> get_all_devices(const subnet *net);

    /* Returns vector of all routers in subnetwork */
    const vector<netface *> get_routers_by_net(const subnet *net);

    /* Sets subnetworks info in the structures*/
    const void set_netmasks();

    /* Deletes all subnetworks */
    const int set_subnets_empty();

    /* Sets subnetwork level*/
    const int set_net_level(const string net_name, const int level);

    /* Set upper level gateway router */
    const int set_gateway(const string net_name, const string gateway_name, const subnet *domain);

    /* Allocates address space in network tree  */
    const int add_subnet(const int max_addressable, const string net_name, const string gateway_name, const string domain_name);

    /* Add addressable to the subnetwork */
    const int add_dev(const string net_name, const string dev_name, const bool router);

    /* Remove devices in subnetwork by name */
    const int remove_dev_by_name(const string dev_name, const string net_name);

    /* Remove subnetworks by name */
    const int remove_net_by_name(const string net_name);
};