// Binary tree that manage network bits node (netbitn)

#include <cmath>
#include <vector>
#include "netdef.h"
#include "netutil.cpp"

using std::string;
using std::vector;

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
	netbitn *init_netbitn(const int level)
	{
		netbitn *node = new netbitn;
		node->net = NULL;
		node->sx = NULL;
		node->dx = NULL;
		node->level = level;
		node->defined = false; // always false if it has been traversed
		return node;
	}

	/* Returns initialized network device */
	netface *init_netface(const bool is_router, const string dev_name, subnet *subnet, const string address)
	{
		netface *device = new netface;
		device->name = dev_name;
		device->router = is_router;
		device->address = address;
		return device;
	}

	/* Function that check and index the subnet */
	const void index_subnet(subnet *net)
	{
		if (get_net_by_name(net->name) == NULL)
			subnetworks->push_back(net);
	}

	/// Service procedures ///

	/* Service procedure to allocate subnet*/
	const void _add_subnet(netbitn *node, const int level, const int netlevel, const string net_name, bool &allocated)
	{
		if (node && node->net == NULL && !(allocated || node->level < level))
		{
			// Subnetwork shall not shares its addresses with other net
			if (node->level == level && node->sx == NULL && node->dx == NULL)
			{
				node->net = new subnet;
				node->net->name = net_name;
				node->net->level = netlevel;
				node->net->addressable = NULL;
				node->net->gateway = std::make_pair((netface *)NULL, (netface *)NULL);
				allocated = true;
			}

			else // traversing search of candidate
			{
				if (node->sx == NULL)
					node->sx = init_netbitn(node->level - 1);

				_add_subnet(node->sx, level, netlevel, net_name, allocated);

				if (node->dx == NULL)
					node->dx = init_netbitn(node->level - 1);

				_add_subnet(node->dx, level, netlevel, net_name, allocated);
			}
		}
	}

	/* Service procedure to sets subnets info */
	const void _set_netmasks(netbitn *node, const string path)
	{
		if (node) // node exists
		{
			if (node->net != NULL && !(node->defined)) // node can represent any subnetwork
			{
				node->net->prefix = prefix_len + path.length();
				node->net->first_addr = netutil::bin_to_ip(netutil::complete_address(base_addr, path, 0));
				node->net->last_addr = netutil::bin_to_ip(netutil::complete_address(base_addr, path, 1));
				node->net->addressable = new netface *[netutil::get_bound(node->net->prefix)]; // array positon represents the interface

				for (int i = 0; i < netutil::get_bound(node->net->prefix); i++)
					node->net->addressable[i] = NULL;

				node->defined = true; // avoid multiple-redefinition

				index_subnet(node->net); // avoid research in entire tree
			}

			_set_netmasks(node->sx, path + '0');
			_set_netmasks(node->dx, path + '1');
		}
	}

public:
	netman(const string addr, const int prefix_len)
	{
		base_addr = netutil::get_bin_prefix(addr, prefix_len);
		this->prefix_len = prefix_len;

		// structures init
		root = init_netbitn(MAX_ADDR_LEN - prefix_len); // root level = max bits available
		subnetworks = new vector<subnet *>();
	}

	/// getter //

	/* Returns subnetwork by name */
	subnet *get_net_by_name(const string name)
	{
		for (auto &net : *subnetworks)
		{
			if (net->name == name)
				return net;
		}

		return NULL;
	}

	/* Returns device by name and subnet */
	netface *get_dev_by_name(const string dev_name, const subnet *net)
	{
		for (int i = 0; i < netutil::get_bound(net->prefix); i++)
		{
			if (net->addressable[i] && net->addressable[i]->name == dev_name)
				return net->addressable[i];
		}
		return NULL;
	}

	/* Returns devie by name and level */
	subnet *get_net_by_gateway(const string dev_name, int level)
	{
		for (auto &net : *subnetworks)
		{
			for (int i = 0; i < netutil::get_bound(net->prefix); i++)
			{
				if (net->addressable[i] && net->addressable[i]->name == dev_name && net->level == level - 1)
					return net;
			}
		}
		return NULL;
	}

	/* Returns all devices addressed in subnet*/
	vector<netface *> get_all_devices(subnet *net)
	{
		vector<netface *> *addressable = new vector<netface *>();

		for (int i = 0; i < netutil::get_bound(net->prefix); i++)
		{
			if (net->addressable[i])
				addressable->push_back(net->addressable[i]);
		}
		return *addressable;
	}

	/* Returns vector of subnetworks pointers*/
	const vector<subnet *> *get_all_nets()
	{
		return subnetworks;
	}

	/* Returns numbers of subnetworks created */
	const int get_nets_count()
	{
		return subnetworks->size();
	}

	/* Returns vector of all routers in subnetwork */
	vector<netface *> *get_all_routers(const subnet *net)
	{
		vector<netface *> *routers = new vector<netface *>();

		for (int i = 0; i < netutil::get_bound(net->prefix); i++)
		{
			if (net->addressable[i] && net->addressable[i]->router)
				routers->push_back(net->addressable[i]);
		}

		return routers;
	}

	/// setter ///

	/* Sets subnetworks info in the structures*/
	const void set_netmasks()
	{
		const string path = ""; //additional mask
		_set_netmasks(root, path);
	}

	/* Deletes all subnetworks */
	const void set_subnets_empty()
	{
		root = init_netbitn(MAX_ADDR_LEN - prefix_len);
		subnetworks->clear();
	}

	/* Set upper level gateway router */
	const void set_gateway(const string net_name, const string gateway_name, int current_level)
	{
		subnet *net = get_net_by_name(net_name);
		netface *gateway = get_dev_by_name(gateway_name, get_net_by_gateway(gateway_name, current_level));

		net->gateway.first = gateway;
	}

	/// adder ///

	/* Allocates address space in network tree  */
	const int add_subnet(const int max_addressable, const int netlevel, const string net_name, const string gateway_name, int current_level)
	{
		if (get_net_by_name(net_name) == NULL)
		{
			const int level = netutil::get_total_level(max_addressable);
			bool allocated = false; // multi-allocation avoidance

			_add_subnet(root, level, netlevel, net_name, allocated);
			set_netmasks();

			if (current_level > 1)
				set_gateway(net_name, gateway_name, current_level);

			return 0;
		}

		return -1;
	}

	/* Add addressable to the subnetwork */
	const int add_dev(const string net_name, const string dev_name, const bool router)
	{
		subnet *sel_subnet = get_net_by_name(net_name);

		if (sel_subnet == NULL) // subnet not exists
			return -1;

		else if (get_dev_by_name(dev_name, sel_subnet) != NULL) //device already exists
			return -2;

		else
		{
			int interface; // avoid broadcast address
			int attempts = 0;

			do
			{
				if (attempts++ > netutil::get_bound(sel_subnet->prefix))
					return -4;

				interface = rand() % (netutil::get_bound(sel_subnet->prefix) - 1); // last address is broadcast

			} while (sel_subnet->addressable[interface] != NULL); // the address is free

			// generation of a valid ipv4 address in the range
			string address = netutil::get_bin_prefix(sel_subnet->first_addr, sel_subnet->prefix) + netutil::get_fixed_length(interface, MAX_ADDR_LEN - sel_subnet->prefix);
			sel_subnet->addressable[interface] = init_netface(router, dev_name, sel_subnet, netutil::bin_to_ip(address));

			if (router && sel_subnet->gateway.second == NULL)
				sel_subnet->gateway.second = sel_subnet->addressable[interface];

			if (router == false) //set gateway automatically with random router in subnet
			{
				vector<netface *> routers = *get_all_routers(sel_subnet);
				sel_subnet->addressable[interface]->gateway = routers[rand() % routers.size()];
			}

			else if (sel_subnet->gateway.second != sel_subnet->addressable[interface])
				sel_subnet->addressable[interface]->gateway = sel_subnet->gateway.second;
		}
		return 0;
	}
};