#include <cmath>
#include <vector>
#include "netdef.h"
#include "netutil.cpp"

// Binary tree that manage network bits node (netbitn)

using namespace std;

class netman
{

private:
    netbitn *root;

    // main network info
    int prefix_len;
    string base_addr;

    // subnetorks indexing
    vector<subnet *> *subnetworks;

    /// init functions ///

    /* Returns empty network bit node */
    netbitn *init_netbitn(const int level);
    netface *init_netface(const bool is_router, const string dev_name, subnet *subnet, const string address);
    const void index_subnet(subnet *net);
    const void _add_subnet(netbitn *node, const int level, const string net_name, bool &allocated);
    const void _set_netmasks(netbitn *node, const string path);

public:
    int max_subnet_level; //statefull

    netman(const string addr, const int prefix_len);
    
    subnet *get_net_by_name(const string name);
    subnet *get_net_by_gateway(const string dev_name, const int level);
    netface *get_dev_by_name(const string dev_name, const subnet *net);
    const vector<netface *> get_all_devices(const subnet *net);

    const vector<subnet *> get_nets_by_level(const int level);
    const vector<subnet *> *get_all_nets();
    const vector<pair<netface *, int>> get_netface_by_gateway(netface *dev, const int prefix);

    const void set_netmasks();
    const int set_subnets_empty();
    const int set_net_level(const string net_name, const int level);
    const int set_gateway(const string net_name, const string gateway_name, const subnet *domain);
    const vector<netface *> get_routers_by_net(const subnet *net);


    const int add_subnet(const int max_addressable, const string net_name, const string gateway_name, const string domain_name);
    const int add_dev(const string net_name, const string dev_name, const bool router);
    
    const int remove_dev_by_name(const string dev_name, const string net_name);
    const int remove_net_by_name(const string net_name);
};