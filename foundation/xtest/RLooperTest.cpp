/*
 * Copyright (C) 2012 The X-Live Project
 *
 * @author Richard Luo, cdominant7@gmail.com
 * @date   2012-05-07 16:00:48
 * 
 * 
 ****************************************************************** 
 */

#define LOG_TAG "RLooperTest"
#include <utils/Log.h>
#include <utils/RefBase.h>
#include <media/stagefright/foundation/RLooper.h>
#include <media/stagefright/foundation/RHandler.h>
#include <media/stagefright/foundation/ARequest.h>

using namespace android;

struct TestHandler : public RHandler {
    int mCount;

    TestHandler() : mCount(0)
    {}

    void post()
    {
        if (mCount++ < 20)
            (new ARequest(mCount, id()))->post();
    }

    void start()
    {
        post();
    }

  protected:

    virtual ~TestHandler() {}

    virtual void onRequestReceived(const sp<ARequest> &msg)
    {
        LOGE("got one request:%d", msg->what() );
        post();
    }

    DISALLOW_EVIL_CONSTRUCTORS(TestHandler);

};


int main (int argc, char *argv[])
{
    LOGE("hello world \n");
    sp<RLooper> looper = new RLooper;
    looper->start();
    sp<TestHandler> handler = new TestHandler;
    looper->registerHandler(handler);
    handler->start();

    sp<ARequest> req = new ARequest(99, handler->id());
    req->post(1000*1000*2);
    sleep(1);
    req->cancel();
    sleep(9);
    
    return 0;
}
