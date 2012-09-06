/*
 * Copyright (C) 2012 The X-Live Project
 *
 * @author Richard Luo, cdominant7@gmail.com
 * @date   2012-05-07 16:00:48
 * 
 * 
 ****************************************************************** 
 */

#define LOG_TAG "ALooperTest"
#include <utils/Log.h>
#include <utils/RefBase.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AMessage.h>

#include <sys/types.h>
#include "UDPPusher.h"

using namespace android;

struct TestHandler : public AHandler {
    int mCount;

    TestHandler() : mCount(0)
    {}

    void post()
    {
        if (mCount++ < 20)
            (new AMessage(mCount, id()))->post();
    }

    void start()
    {
        post();
    }

  protected:

    virtual ~TestHandler() {}

    virtual void onMessageReceived(const sp<AMessage> &msg)
    {
        LOGE("got one message:%d", msg->what() );
        post();
    }

    DISALLOW_EVIL_CONSTRUCTORS(TestHandler);

};


int main (int argc, char *argv[])
{
    LOGE("hello world \n");
    sp<ALooper> looper = new ALooper;
    looper->start();
    sp<TestHandler> handler = new TestHandler;
    looper->registerHandler(handler);
    handler->start();
    sleep(10);
    return 0;
}
