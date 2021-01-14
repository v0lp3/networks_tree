#define MAX_ADDR_LEN 32
#define ERROR_LEN "BITS LENGTH ERROR"

typedef struct
{
    int prefix;
    std::string name;
    std::string first_addr;
    std::string last_addr;

} subnet;

struct bitnode
{
    subnet *net;
    struct bitnode *sx;
    struct bitnode *dx;
    int level;
};

typedef bitnode netbitn;