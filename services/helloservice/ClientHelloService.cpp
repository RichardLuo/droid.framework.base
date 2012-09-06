/******************************************************************
 * @file   ClientHelloService.cpp
 * @author Richard Luo, cdominant7@gmail.com
 * @date   2011/07/18 02:38:29
 * 
 * @brief  Did he have passion?
 * 
 ****************************************************************** 
 */

#define LOG_TAG "ClientHelloService"

#include <binder/IServiceManager.h>
#include <utils/Log.h>

#include "HelloService.h"

using namespace android;

int main(int argc, char *argv[])
{
	LOGI("Hello client is now starting");

    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder;
    sp<IHelloServer> shw;

    do {
        binder = sm->getService(String16("HelloService"));
        if (binder != 0)
            break;
        LOGW("HelloWorld not published, waiting...");
        usleep(500000); // 0.5 s
    } while(true);

	LOGI("Hello client is now trying");

    shw = interface_cast<IHelloServer>(binder);
    shw->sayHello();
	
	LOGI("Hello client is now exiting");

	return(0);
}
