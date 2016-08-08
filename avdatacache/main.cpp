/*
 * main.cpp
 *
 *  Created on: 2016年8月4日
 *      Author: lixiang 21588
 */
#include "avDataManage.h"
#include <signal.h>

static avDataManage* pav=NULL;
static void signalINTHandler(int signum)
{
    DBG();
	delete pav;
	exit(-1);
}

int main(){
    struct sigaction sigAction;

    /* insure a clean shutdown if user types ctrl-c */
    sigAction.sa_handler = signalINTHandler;
    sigAction.sa_flags = 0;
    sigemptyset((sigset_t *)&sigAction.sa_mask);
    sigaddset((sigset_t *)&sigAction.sa_mask, SIGINT);
    sigaction(SIGINT, &sigAction, NULL);
    sigaction(SIGSEGV, &sigAction, NULL);
    sigaction(SIGKILL, &sigAction, NULL);
    pav= new avDataManage();
    DBG();
    if(pav->init()<0){
    	return -1;
    }
    DBG();
	while(1){
		pav->saveOneAvdataToCach();
	}
	delete pav;
	return 0;
}



