/*
 * main.cpp
 *
 *  Created on: 2016年8月4日
 *      Author: lixiang 21588
 */
#include "recordManage.h"
#include <signal.h>

static recordManage* pav=NULL;
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
    sigaddset((sigset_t *)&sigAction.sa_mask, SIGSEGV);
    sigaddset((sigset_t *)&sigAction.sa_mask, SIGKILL);
    sigaddset((sigset_t *)&sigAction.sa_mask, SIGTERM);
    sigaddset((sigset_t *)&sigAction.sa_mask, SIGQUIT);
    sigaction(SIGINT, &sigAction, NULL);
    sigaction(SIGSEGV, &sigAction, NULL);
    sigaction(SIGKILL, &sigAction, NULL);
    sigaction(SIGTERM, &sigAction, NULL);
    sigaction(SIGQUIT, &sigAction, NULL);
    pav= new recordManage();
    DBG();
    if(pav->init()<0){
    	return -1;
    }
    DBG();
    pav->run();
	delete pav;
	return 0;
}



