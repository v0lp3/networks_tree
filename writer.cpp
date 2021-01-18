using std::string;
using std::vector;
using std::__cxx11::to_string;

class writer
{

private:
    netman *tree;
    vector<netent> entities;

    /// init functions ///

    /* Creates output directories */
    const void init_dir(string dir)
    {
        string command = "mkdir " + dir + " >> /dev/null 2>&1";
        system(&command[0]);
    }

    /* Service recursive procedure: routing discovery */
    const void _init_route(string route, subnet *net)
    {
        if (net && net->gateway.first)
        {
            subnet *lan = tree->get_net_by_gateway(net->gateway.first->name, net->level);

            if (lan) // not subnet level 1
            {
                for (auto &router : tree->get_routers_by_net(lan))
                {
                    if (router->name != net->gateway.first->name)
                        search_by_netface(router)->routes.push_back(route + " via " + net->gateway.first->address);
                }

                _init_route(route, lan); //write route in gateway of gateway
            }
        }
    }

    /* Initialize entities vector with routing info */
    const void init_route()
    {
        for (int i = tree->max_subnet_level; i > 1; i--)
        {
            for (auto &net : tree->get_nets_by_level(i))
            {
                string route = net->first_addr + "/" + to_string(net->prefix);
                _init_route(route, net);
            }
        }
    }

    /* Initialize vector of entities */
    const void init_interfaces()
    {
        string path = "";

        for (int i = 1; i <= tree->max_subnet_level; i++)
        {
            netent entity; // related to file to output

            for (auto &net : tree->get_nets_by_level(i))
            {
                entity.path = path + net->name + "/";

                for (auto &dev : tree->get_all_devices(net))
                {
                    entity.name = dev->name;

                    if (dev->router)
                        entity.interfaces = tree->get_netface_by_gateway(dev, net->prefix); // merge netface of same entity

                    else // single interface
                        entity.interfaces.push_back(make_pair(dev, net->prefix));

                    if (!(dev->name == DEFROUTER && net->level > 1)) // mantain gateway level 1
                        entities.push_back(entity);

                    entity.interfaces.clear(); // flush out from other entity interfaces
                }
            }
            path = entity.path; // set deeper path
        }
    }

    /// write functions ///

    /* Writes any interface lines in a file */
    const void write_interface(ofstream &file, string interface_name, netface *dev, int prefix)
    {
        file << "allow-hotplug " << interface_name << endl;
        file << "iface " << interface_name << " inet static" << endl;
        file << "address " << dev->address << "/" << prefix << endl;

        if (dev->gateway && dev->gateway->address.length() > 0)
            file << "gateway " << dev->gateway->address << endl;

        file << endl;
    }

    /* Writes any route line */
    const void write_route(ofstream &file, string route)
    {
        file << "up /bin/ip route add " << route << endl;
    }

    /* Writes loopback lines */
    const void write_loopback(ofstream &file)
    {
        file << "auto lo" << endl;
        file << "iface lo inet loopback" << endl;
        file << endl;
    }

    /* Writes configuration interface file */
    const void write_interfaces(bool include_loopback)
    {
        for (auto &entity : entities)
        {
            int i = 0;
            init_dir(entity.path);
            ofstream file(entity.path + entity.name);

            file << "source /etc/network/interfaces.d/*" << endl
                 << endl;

            if (include_loopback)
                write_loopback(file);

            for (auto &interface : entity.interfaces)
                write_interface(file, DEFIFACE + to_string(i++), interface.first, interface.second);

            for (auto &route : entity.routes)
                write_route(file, route);
        }
    }

    /* Search entity by pointer in entities vector */
    netent *search_by_netface(netface *dev)
    {
        for (auto &ent : entities)
        {
            for (auto &interface : ent.interfaces)
            {
                if (interface.first == dev)
                    return &ent;
            }
        }
        return NULL;
    }

public:
    writer(netman *tree)
    {
        this->tree = tree;
        init_interfaces();
        init_route();
    }

    const void write()
    {
        string input;

        cout << "Include loopback interface? (y/N): ";
        cin >> input;

        (input == "y") ? write_interfaces(true) : write_interfaces(false);

        netview::print_info("all files writted");
    }
};