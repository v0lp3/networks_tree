#define MAX_IPV4_LEN 32
#define MAX_IPV6_LEN 128
#define ERROR_LEN "BITS LENGTH ERROR"
#define DEFROUTER "gateway"
#define DEFIFACE "enp0s"
#define OUTPUTDIR "output/"

//output style
#define RSCLR "\033[0m"
#define REDBG "\033[7;31m"
#define BLUBG "\033[7;34m"
#define WHIBG "\033[7;37m"
#define GRCLR "\033[1;32m"
#define REDCLR "\033[1;31m"

using namespace std;

struct interface
{
    bool router;
    string name;
    string address;
    string netname;
    struct interface *gateway;
};

typedef interface netface; // virtual entity

typedef struct
{
    string name;
    vector<pair<netface *, int>> interfaces;
    vector<string> routes;
    string path;
} netent; // phisical entity

struct net
{
    int prefix;
    int level;
    netface **addressable;
    pair<netface *, netface *> gateway;
    string name;
    string first_addr;
    string last_addr;
};

typedef net subnet;

struct bitnode
{
    subnet *net;
    struct bitnode *sx;
    struct bitnode *dx;
    int level;
    bool defined;
};

typedef bitnode netbitn;