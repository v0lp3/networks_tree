#define MAX_ADDR_LEN 32
#define ERROR_LEN "BITS LENGTH ERROR"

//output style
#define RSCLR "\033[0m"
#define REDBG "\033[7;31m"
#define BLUBG "\033[7;34m"
#define WHIGB "\033[7;37m"
#define GRCLR "\033[1;32m"


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