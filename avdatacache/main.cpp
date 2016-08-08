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
	delete pav;
	return;
}

int main(){
    struct sigaction sigAction;

    /* insure a clean shutdown if user types ctrl-c */
    sigAction.sa_handler = signalINTHandler;
    sigAction.sa_flags = 0;
    sigemptyset((sigset_t *)&sigAction.sa_mask);
    sigaddset((sigset_t *)&sigAction.sa_mask, SIGINT);
    sigaction(SIGINT, &sigAction, NULL);
    pav= new avDataManage();
    pav->init();
	while(1){
		pav->saveOneAvdataToCach();
	}
	delete pav;
	return 0;
}



