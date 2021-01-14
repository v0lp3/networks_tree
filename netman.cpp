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
	netbitn *create_netbitn(int level)
	{
		netbitn *node = new netbitn;
		node->net = NULL;
		node->sx = NULL;
		node->dx = NULL;
		node->level = level;
		return node;
	}

	/* Service procedure to allocate subnet*/
	void _add_subnetwork(netbitn *node, int level, string net_name, bool &allocated)
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
					node->sx = create_netbitn(node->level - 1);

				_add_subnetwork(node->sx, level, net_name, allocated);

				if (node->dx == NULL)
					node->dx = create_netbitn(node->level - 1);

				_add_subnetwork(node->dx, level, net_name, allocated);
			}
		}
	}

	/* Service procedure to sets subnets info */
	void _set_netmasks(netbitn *node, string path)
	{
		if (node) // node exists
		{
			if (node->net != NULL) // node represent any subnetwork
			{
				node->net->prefix = prefix_len + path.length();
				node->net->first_addr = bin_to_ip(complete_octet(base_addr, path, 0));
				node->net->last_addr = bin_to_ip(complete_octet(base_addr, path, 1));
				subnetworks->push_back(node->net); // indexing for avoid research
			}

			_set_netmasks(node->sx, path + '0');
			_set_netmasks(node->dx, path + '1');
		}
	}

public:
	netman(string addr, int prefix_len)
	{
		base_addr = get_bin_prefix(addr, prefix_len);
		this->prefix_len = prefix_len;

		// structures init
		root = create_netbitn(MAX_ADDR_LEN - prefix_len); // root level = max bit available
		subnetworks = new vector<subnet *>();
	}

	/* Allocates address space in network tree  */
	void add_subnetwork(int max_hosts, string net_name)
	{

		int level = log2(max_hosts) + 1;
		bool allocated = false; // multi-allocation avoidance

		_add_subnetwork(root, level, net_name, allocated);
	}

	/* Sets subnetworks info in the structures*/
	void set_netmasks()
	{
		string path = "";
		_set_netmasks(root, path);
	}

	/* Returns vector of subnetworks pointers*/
	vector<subnet *> *get_all()
	{
		set_netmasks();
		return subnetworks;
	}
};
