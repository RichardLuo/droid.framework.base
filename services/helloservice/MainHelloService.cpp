/******************************************************************
 * @file   MainHelloService.cpp
 * @author Richard Luo, cdominant7@gmail.com
 * @date   2011/07/18 02:22:55
 * 
 * @brief  Did he have passion?
 * 
 ****************************************************************** 
 */

#define LOG_TAG "Main/HelloService"

#include "HelloService.h"

int main(int argc, char *argv[])
{
    android::HelloService::publishAndJoinThreadPool();
	return 0;
}
