
#include "logging.h"


int main()
{
    logset logsetting;
    if(getlogset(&logsetting) == false){
        ERROR("get log set fail!");
        return -1;
    }

	if(initlogging(&logsetting) < 0){
        ERROR("init logging data fail!");
    }

    /*  test code bellow  */
	int i;
	for(int i = 0; i < 1; i++){
	    DEBUG("logsetting->filepath: %s", logsetting.filepath);
		DEBUG("logsetting->loglevel: %d", logsetting.loglevel);
		DEBUG("logsetting->maxfilelen: %ld", logsetting.maxfilelen);
		DEBUG("logsetting->outputtype: %d", logsetting.outputtype);
		
		LOG("Hello, I'm LOG");
		DEBUG("Hello, I'm Debug");
	    INFO("Hello, I'm Info");
	    WARN("Hello, I'm Warning");
	    ERROR("Hello, I'm Error");
		FATAL("Hello, I'm FATAL--------------> i: %d", i);
	}
	/*  test code above  */

    destorylogging();
    
    return 0;
}

