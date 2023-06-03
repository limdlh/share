#include "redis_adapter.h"
#include "data_switch.h"
#include "itwconfig.h"
#include "zhelpers.hpp"
#include <sys/stat.h>
#include <fcntl.h>



int daemonize();
bool createPubProxy(CTX_ITW* ctx_itw, void *pub_clients, void *pub_server);
void create_publish_process(ITWCONFIG* configer);
void* pub_Data(void* args);




