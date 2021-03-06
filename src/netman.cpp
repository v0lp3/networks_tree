// Binary tree that manage network bits node (netbitn)

#include "netman.hh"

using namespace std;

netbitn *netman::init_netbitn(const int level)
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
netface *netman::init_netface(const bool is_router, const string dev_name, subnet *subnet, const string address)
{
	netface *device = new netface;
	device->name = dev_name;
	device->router = is_router;
	device->address = address;
	return device;
}

/* Function that check and index the subnet */
const void netman::index_subnet(subnet *net)
{
	if (get_net_by_name(net->name) == NULL)
		subnetworks->push_back(net);
}

/// Service procedures ///

/* Service procedure to allocate subnet*/
const void netman::_add_subnet(netbitn *node, const int level, const string net_name, bool &allocated)
{
	if (node && node->net == NULL && !(allocated || node->level < level))
	{
		// Subnetwork shall not shares its addresses with other net
		if (node->level == level && node->sx == NULL && node->dx == NULL)
		{
			node->net = new subnet;
			node->net->name = net_name;
			node->net->addressable = NULL;
			node->net->gateway = make_pair((netface *)NULL, (netface *)NULL);
			allocated = true;
		}

		else // traversing search of candidate
		{
			if (node->sx == NULL)
				node->sx = init_netbitn(node->level - 1);

			_add_subnet(node->sx, level, net_name, allocated);

			if (node->dx == NULL)
				node->dx = init_netbitn(node->level - 1);

			_add_subnet(node->dx, level, net_name, allocated);
		}
	}
}

/* Service procedure to sets subnets info */
const void netman::_set_netmasks(netbitn *node, const string path)
{
	if (node) // node exists
	{
		if (node->net != NULL && !(node->defined)) // node can represent any subnetwork
		{
			node->net->prefix = prefix_len + path.length();
			node->net->first_addr = utility->bin_to_ip(utility->complete_address(base_addr, path, 0));
			node->net->last_addr = utility->bin_to_ip(utility->complete_address(base_addr, path, 1));
			node->net->addressable = new netface *[utility->get_bound(node->net->prefix)]; // array positon represents the interface

			for (int i = 0; i < utility->get_bound(node->net->prefix); i++)
				node->net->addressable[i] = NULL;

			node->defined = true; // avoid multiple-redefinition

			index_subnet(node->net); // avoid research in entire tree
		}

		_set_netmasks(node->sx, path + '0');
		_set_netmasks(node->dx, path + '1');
	}
}

netman::netman(const string addr, const int prefix_len, bool ipv6)
{

	if (ipv6)
	{
		netutil6 *util = new netutil6;
		max_addr_len = MAX_IPV6_LEN;
		utility = util;
	}

	else
	{
		netutil4 *util = new netutil4;
		max_addr_len = MAX_IPV4_LEN;
		utility = util;
	}

	base_addr = utility->get_bin_prefix(addr, prefix_len);
	this->prefix_len = prefix_len;
	// structures init
	root = init_netbitn(max_addr_len - prefix_len); // root level = max bits available
	subnetworks = new vector<subnet *>();
}

/// getter //

/* Returns subnetwork by name */
subnet *netman::get_net_by_name(const string name)
{
	for (auto &net : *subnetworks)
	{
		if (net->name == name)
			return net;
	}

	return NULL;
}

/* Returns all devices addressed in subnet*/
const vector<netface *> netman::get_all_devices(const subnet *net)
{
	vector<netface *> *addressable = new vector<netface *>();

	for (int i = 0; i < utility->get_bound(net->prefix); i++)
	{
		if (net->addressable[i])
			addressable->push_back(net->addressable[i]);
	}
	return *addressable;
}

/* Returns device by name and subnet */
netface *netman::get_dev_by_name(const string dev_name, const subnet *net)
{
	for (auto &dev : get_all_devices(net))
	{
		if (dev->name == dev_name)
			return dev;
	}
	return NULL;
}

/* Returns devie by name and level */
subnet *netman::get_net_by_gateway(const string dev_name, const int level)
{
	for (auto &net : *subnetworks)
	{
		if (get_dev_by_name(dev_name, net) && net->level == level - 1)
			return net;
	}
	return NULL;
}

/* Returns all subnetworks by related gateway router */
const vector<pair<netface *, int>> netman::get_netface_by_gateway(netface *dev, const int prefix)
{
	vector<pair<netface *, int>> interfaces;

	interfaces.push_back(make_pair(dev, prefix));

	for (auto &net : *subnetworks)
	{
		if (net->gateway.first == dev)
			interfaces.push_back(make_pair(net->gateway.second, net->prefix));
	}
	return interfaces;
}

/* Returns vector of subnetworks pointers*/
const vector<subnet *> *netman::get_all_nets()
{
	return subnetworks;
}

/* Returns vector of subnetworks by level */
const vector<subnet *> netman::get_nets_by_level(const int level)
{
	vector<subnet *> nets;

	for (auto &net : *subnetworks)
	{
		if (net->level == level)
			nets.push_back(net);
	}

	return nets;
}

/* Returns vector of all routers in subnetwork */
const vector<netface *> netman::get_routers_by_net(const subnet *net)
{
	vector<netface *> routers;

	for (auto &dev : get_all_devices(net))
	{
		if (dev->router)
			routers.push_back(dev);
	}

	return routers;
}

/// setter ///

/* Sets subnetworks info in the structures*/
const void netman::set_netmasks()
{
	const string path = ""; //additional mask
	_set_netmasks(root, path);
}

/* Deletes all subnetworks */
const int netman::set_subnets_empty()
{
	root = init_netbitn(max_addr_len - prefix_len);
	subnetworks->clear();
	return 1;
}

/* Sets subnetwork level*/
const int netman::set_net_level(const string net_name, const int level)
{
	subnet *net = get_net_by_name(net_name);

	if (net)
	{
		net->level = level;
		return 0;
	}

	return -1;
}

/* Set upper level gateway router */
const int netman::set_gateway(const string net_name, const string gateway_name, const subnet *domain)
{
	subnet *net = get_net_by_name(net_name);
	netface *gateway = get_dev_by_name(gateway_name, domain);

	if (net && gateway)
	{
		net->gateway.first = gateway;
		return 0;
	}
	return -2;
}

/// adder ///

/* Allocates address space in network tree  */
const int netman::add_subnet(const int max_addressable, const string net_name, const string gateway_name, const string domain_name)
{
	if (get_net_by_name(net_name) == NULL)
	{
		const int level = utility->get_total_level(max_addressable + 1);
		bool allocated = false; // multi-allocation avoidance
		subnet *domain = get_net_by_name(domain_name);

		if (get_all_nets()->size() == 0 || (domain && get_dev_by_name(gateway_name, domain))) //subnet level 1 or domain and gateway exists
		{
			_add_subnet(root, level, net_name, allocated);
			set_netmasks();

			if (get_all_nets()->size() > 1)
			{
				max_subnet_level = (domain->level + 1 > max_subnet_level) ? domain->level + 1 : max_subnet_level;
				set_net_level(net_name, domain->level + 1);
				return set_gateway(net_name, gateway_name, domain);
			}

			else
			{
				set_net_level(net_name, 1);
				max_subnet_level = 1;
			}

			return 0;
		}

		return -2;
	}

	return -1;
}

/* Add addressable to the subnetwork */
const int netman::add_dev(const string net_name, const string dev_name, const bool router)
{
	subnet *sel_subnet = get_net_by_name(net_name);

	if (sel_subnet == NULL) // subnet not exists
		return -1;

	else if (get_dev_by_name(dev_name, sel_subnet) != NULL) //device already exists
		return -2;

	else
	{
		int interface; // avoid broadcast address
		int attempt = 0;

		do
		{
			if (attempt++ > utility->get_bound(sel_subnet->prefix))
				return -4;

			interface = rand() % (utility->get_bound(sel_subnet->prefix) - 1); // last address is broadcast

		} while (sel_subnet->addressable[interface] != NULL); // the address is free

		// generation of a valid ipv4 address in the range
		string address = utility->get_bin_prefix(sel_subnet->first_addr, sel_subnet->prefix) + utility->get_fixed_length(interface, max_addr_len - sel_subnet->prefix);
		sel_subnet->addressable[interface] = init_netface(router, dev_name, sel_subnet, utility->bin_to_ip(address));

		if (router && sel_subnet->gateway.second == NULL) // set default gateway
			sel_subnet->gateway.second = sel_subnet->addressable[interface];

		if (router == false) //set gateway automatically with random router in subnet
			sel_subnet->addressable[interface]->gateway = sel_subnet->gateway.second;
	}
	return 0;
}

/// remover ///

/* Remove devices in subnetwork by name */
const int netman::remove_dev_by_name(const string dev_name, const string net_name)
{
	subnet *net = get_net_by_name(net_name);

	if (net)
	{
		for (int i = 0; i < utility->get_bound(net->prefix); i++)
		{
			if (net->addressable[i] && net->addressable[i]->name == dev_name)
			{
				net->addressable[i] = NULL;
				return 0;
			}
		}
		return -1;
	}
	return -2;
}

/* Remove subnetworks by name */
const int netman::remove_net_by_name(const string net_name)
{
	int i = 0;

	for (auto &net : *subnetworks)
	{
		if (net->name == net_name)
		{
			subnetworks->erase(subnetworks->begin() + i);
			return 0;
		}

		i++;
	}
	return -1;
}