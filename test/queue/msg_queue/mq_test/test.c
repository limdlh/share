//#include "msginfo.h"	//队列中的元素
#include "cyclefifo.h"

int main(int argc, char* argv[]){
	//CMsginfo msginfo;
	//msginfo.setSid("0x0078");
	//CMsginfo *p_msginfo = &msginfo;
		
	//char* pp = (char*) p_msginfo;
	
	//CMsginfo *pt = (CMsginfo *) pp;
		
	printf("--------------------------------------------------------------------");
	Cfqueue recvQueue;
	cfqInit(&recvQueue, 10);
	cfqPush(&recvQueue, "limd");

	//CMsginfo msginfo2;
	//msginfo2.setSid("0x0080");
	//cfqPush(&recvQueue, (char*) &msginfo2);
	
	char* qq = cfqPop(&recvQueue);
	
	//CMsginfo *qt = (CMsginfo *) qq;
	//std::string str_get2=qt->getSid();
	//printf("in main 1 sid = %s", str_get2);
	//std::cout << "3elements in queue = " << cfqGetQlength(&recvQueue) << std::endl;

	//std::string str_get3=((CMsginfo *) cfqPop(&recvQueue))->getSid();
	//std::cout << "in main 1 sid=" << str_get3 << std::endl;
	//std::cout << "3elements in queue = " << cfqGetQlength(&recvQueue) << std::endl;
	printf("in main cfqGetQlength(&recvQueue) = %d", cfqGetQlength(&recvQueue));

	cfqDestory(&recvQueue);
	return 0;
}

