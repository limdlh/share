#include "log.h"

debug_type = STD_OUT;
debug_levet = LEVEL_LOG;

//NO_OUTPUT, WRITE_FILE, STD_OUT



int main(int argc, char **argv)
{
    int   fd =0;
	if (debug_type == WRITE_FILE){
		fd=log_model("log.txt");
	    printf("-----------------------------------------------------------> \n");
	}

    INFO("I'm Info");
    WARN("I'm Warning");
    DEBUG("I'm Debug");
    ERROR("I'm Error");
	FATAL("I'm FATAL");
	LOG("I'm LOG");
	LOG("I'm Test, length is 22");
	LOG("I'm LOG");
    DEBUG("I'm Debug");
    ERROR("I'm Error");
	LOG("I'm Test, length is 22 I'm Test, length is 22 I'm Test, length is 22I'm Test, length is 22 I'm Test, length is 22 I'm Test, length is 22");
    LOG("I'm Test, length is 22");
	LOG("I'm LOG");
	LOG("Hello! I'm Test, length is 29");
	
    close(fd);
    return 0;
}

