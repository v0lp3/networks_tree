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
	netface *init_netface(const bool is_router, const string dev_name, const string address)
	{
		netface *device = new netface;
		device->name = dev_name;
		device->router = is_router;
		device->address = address;
		return device;
	}

	/* Service procedure to allocate subnet*/
	const void _add_subnetwork(netbitn *node, const int level, const string net_name, bool &allocated)
	{
		if (node && node->net == NULL && !(allocated || node->level < level))
		{
			// Subnetwork shall not shares its addresses with other net
			if (node->level == level && node->sx == NULL && node->dx == NULL)
			{
				node->net = new subnet;
				node->net->name = net_name;
				allocated = true;
			}

			else // traversing search of candidate
			{
				if (node->sx == NULL)
					node->sx = init_netbitn(node->level - 1);

				_add_subnetwork(node->sx, level, net_name, allocated);

				if (node->dx == NULL)
					node->dx = init_netbitn(node->level - 1);

				_add_subnetwork(node->dx, level, net_name, allocated);
			}
		}
	}

	/* Function that check and index the subnet */
	const void index_subnet(subnet *net)
	{
		if (get_net_by_name(net->name) == NULL)
			subnetworks->push_back(net);
	}

	/* Service procedure to sets subnets info */
	const void _set_netmasks(netbitn *node, const string path)
	{
		if (node) // node exists
		{
			if (node->net != NULL && !(node->defined)) // node can represent any subnetwork
			{
				node->net->prefix = prefix_len + path.length();
				node->net->first_addr = bin_to_ip(complete_address(base_addr, path, 0));
				node->net->last_addr = bin_to_ip(complete_address(base_addr, path, 1));
				node->net->devices = new netface *[get_bound(node->net->prefix)]; // array positon represents the interface

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
		base_addr = get_bin_prefix(addr, prefix_len);
		this->prefix_len = prefix_len;

		// structures init
		root = init_netbitn(MAX_ADDR_LEN - prefix_len); // root level = max bits available
		subnetworks = new vector<subnet *>();
	}

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

	/* Returns device by name */
	netface *get_dev_by_name(const string dev_name, const subnet *net)
	{
		for (int i = 0; i < get_bound(net->prefix); i++)
		{
			if (net->devices[i] && net->devices[i]->name == dev_name)
				return net->devices[i];
		}
		return NULL;
	}

	/* Allocates address space in network tree  */
	const int add_subnetwork(const int max_hosts, const string net_name)
	{
		if (get_net_by_name(net_name) == NULL)
		{
			const int level = log2(max_hosts) + 1;
			bool allocated = false; // multi-allocation avoidance

			_add_subnetwork(root, level, net_name, allocated);
			set_netmasks();

			return 0;
		}

		return -1;
	}

	/* Sets subnetworks info in the structures*/
	const void set_netmasks()
	{
		const string path = ""; //additional mask
		_set_netmasks(root, path);
	}

	/* Returns vector of subnetworks pointers*/
	const vector<subnet *> *get_all_nets()
	{
		return subnetworks;
	}

	/* Returns vector of all routers in subnetwork */
	vector<netface *> *get_all_routers(const subnet *net)
	{
		vector<netface *> *routers = new vector<netface *>();

		for (int i = 0; i < get_bound(net->prefix); i++)
		{
			if (net->devices[i] && net->devices[i]->router)
				routers->push_back(net->devices[i]);
		}

		return routers;
	}

	/* Deletes all subnetworks */
	const void clear_all()
	{
		root = init_netbitn(MAX_ADDR_LEN - prefix_len);
		subnetworks->clear();
	}

	/* Add devices to the subnetwork */
	const int attach_device(const string net_name, const string dev_name, const bool router)
	{
		subnet *sel_subnet = get_net_by_name(net_name);

		if (sel_subnet == NULL) // subnet not exists
			return -1;

		else if (get_dev_by_name(dev_name, sel_subnet) != NULL) //device already exists
			return -2;

		else if (get_all_routers(sel_subnet)->empty() && !router)
			return -3;

		else
		{
			int interface; // avoid broadcast address

			do
				interface = rand() % (get_bound(sel_subnet->prefix) - 1);
			while (sel_subnet->devices[interface] != NULL); // the address is free

			// generation of a valid ipv4 address in the range
			string address = get_bin_prefix(sel_subnet->first_addr, sel_subnet->prefix) + get_fixed_length(interface, MAX_ADDR_LEN - sel_subnet->prefix);
			sel_subnet->devices[interface] = init_netface(router, dev_name, bin_to_ip(address));

			if (router == false) //set gateway automatically with random router in subnet
			{
				vector<netface *> routers = *get_all_routers(sel_subnet);
				sel_subnet->devices[interface]->gateway = routers[rand() % routers.size()]->address;
			}

			return 0;
		}
	}
};