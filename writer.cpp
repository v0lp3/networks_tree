using std::string;
using std::vector;

class writer
{

private:
    netman *tree;

    vector<pair<ofstream *, pair<netface *, int>>> *devices;

    /* Creates one folder for each lan */
    const void init_dir()
    {
        for (auto &net : *tree->get_all_nets())
        {
            string command = "mkdir " + net->name + "/";
            system(&command[0]);
        }
    }

    /* Creates one file for each device */
    const void init_files()
    {
        devices = new vector<pair<ofstream *, pair<netface *, int>>>;

        for (auto &net : *tree->get_all_nets())
        {
            for (auto &dev : tree->get_all_devices(net))
            {

                ofstream *file = new ofstream(net->name + "/" + dev->name);
                netface *device = dev;

                devices->push_back(make_pair(file, make_pair(dev, net->prefix)));
            }
        }
    }

    /* Writes loopback lines */
    const void write_loopback(ofstream *file)
    {

        *file << "auto lo" << endl;
        *file << "iface lo inet interface" << endl;
        *file << endl;
    }

public:
    writer(netman *tree)
    {

        this->tree = tree;

        init_dir();
        init_files();
    }

    /* Writes configuration interface file */
    const void write_interface(const string interface_name, bool include_loopback)
    {

        for (auto &file : *devices)
        {
            if (include_loopback)
                write_loopback(file.first);

            *file.first << "allow-hotplug " << interface_name << endl;
            *file.first << "iface " << interface_name << " inet static" << endl;
            *file.first << "address " << file.second.first->address << "/" << file.second.second << endl;

            if (file.second.first->gateway && file.second.first->gateway->address.length() > 0)
                *file.first << "gateway " << file.second.first->gateway->address << endl;
        }
    }
};