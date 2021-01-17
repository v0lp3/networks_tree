#define MAX_ADDR_LEN 32
#define ERROR_LEN "BITS LENGTH ERROR"
#define DEFROUTER "router0"

//output style
#define RSCLR "\033[0m"
#define REDBG "\033[7;31m"
#define BLUBG "\033[7;34m"
#define WHIBG "\033[7;37m"
#define GRCLR "\033[1;32m"
#define REDCLR "\033[1;31m"

struct interface
{
    bool router;
    std::string name;
    std::string address;
    std::string netname;
    struct interface *gateway;
};

typedef interface netface;

struct net
{
    int prefix;
    int level;
    netface **addressable;
    std::pair<netface *, netface *> gateway;
    std::string name;
    std::string first_addr;
    std::string last_addr;
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