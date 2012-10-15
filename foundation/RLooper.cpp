/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "RLooper"
#include <utils/Log.h>

#include <sys/time.h>

#include "RLooper.h"

#include "RHandler.h"
#include "RLooperRoster.h"
#include "ARequest.h"

namespace android {

RLooperRoster gRLooperRoster;

struct RLooper::LooperThread : public Thread {
    LooperThread(RLooper *looper, bool canCallJava)
        : Thread(canCallJava),
          mLooper(looper) {
    }

    virtual bool threadLoop() {
        return mLooper->loop();
    }

protected:
    virtual ~LooperThread() {}

private:
    RLooper *mLooper;

    DISALLOW_EVIL_CONSTRUCTORS(LooperThread);
};

// static
int64_t RLooper::GetNowUs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (int64_t)tv.tv_sec * 1000000ll + tv.tv_usec;
}

RLooper::RLooper()
    : mRunningLocally(false) {
}

RLooper::~RLooper() {
    stop();
}

void RLooper::setName(const char *name) {
    mName = name;
}

RLooper::handler_id RLooper::registerHandler(const sp<RHandler> &handler) {
    return gRLooperRoster.registerHandler(this, handler);
}

void RLooper::unregisterHandler(handler_id handlerID) {
    gRLooperRoster.unregisterHandler(handlerID);
}

status_t RLooper::start(
        bool runOnCallingThread, bool canCallJava, int32_t priority) {
    if (runOnCallingThread) {
        {
            Mutex::Autolock autoLock(mLock);

            if (mThread != NULL || mRunningLocally) {
                return INVALID_OPERATION;
            }

            mRunningLocally = true;
        }

        do {
        } while (loop());

        return OK;
    }

    Mutex::Autolock autoLock(mLock);

    if (mThread != NULL || mRunningLocally) {
        return INVALID_OPERATION;
    }

    mThread = new LooperThread(this, canCallJava);

    status_t err = mThread->run(
            mName.empty() ? "RLooper" : mName.c_str(), priority);
    if (err != OK) {
        mThread.clear();
    }

    return err;
}

status_t RLooper::stop() {
    sp<LooperThread> thread;
    bool runningLocally;

    {
        Mutex::Autolock autoLock(mLock);

        thread = mThread;
        runningLocally = mRunningLocally;
        mThread.clear();
        mRunningLocally = false;
    }

    if (thread == NULL && !runningLocally) {
        return INVALID_OPERATION;
    }

    if (thread != NULL) {
        thread->requestExit();
    }

    mQueueChangedCondition.signal();

    if (!runningLocally) {
        thread->requestExitAndWait();
    }

    return OK;
}

void RLooper::post(const sp<ARequest> &req, int64_t delayUs) {
    Mutex::Autolock autoLock(mLock);

    int64_t whenUs;
    if (delayUs > 0) {
        whenUs = GetNowUs() + delayUs;
    } else {
        whenUs = GetNowUs();
    }

    // LOGFL("mRequestQueue size:%d", mRequestQueue.size());

    List<Request>::iterator it = mRequestQueue.begin();
    while (it != mRequestQueue.end() && (*it).mWhenUs <= whenUs) {
        ++it;
    }

    Request request;
    request.mWhenUs = whenUs;
    request.mARequest = req;

    if (it == mRequestQueue.begin()) {
        mQueueChangedCondition.signal();
    }

    mRequestQueue.insert(it, request);
}

void RLooper::cancel(const sp<ARequest> &req)
{
    Mutex::Autolock autoLock(mLock);

    List<Request>::iterator it = mRequestQueue.begin();
    for (; it != mRequestQueue.end(); ++it) {
        if (it->mARequest == req) {
            break;
        }
    }

    if (it == mRequestQueue.end()) {
        LOGE("WARN: on cancel req, it's not in rlooper's queue!");
        return;
    }

    LOGW("request of [%d] has been canceled!", it->mARequest->what() );
    mRequestQueue.erase(it);

}


bool RLooper::loop() {
    Request request;

    {
        Mutex::Autolock autoLock(mLock);
        if (mThread == NULL && !mRunningLocally) {
            return false;
        }
        if (mRequestQueue.empty()) {
            mQueueChangedCondition.wait(mLock);
            return true;
        }
        const int64_t whenUs = (*mRequestQueue.begin()).mWhenUs;
        const int64_t nowUs = GetNowUs();

        if (whenUs > nowUs) {
            const int64_t delayUs = whenUs - nowUs;
            mQueueChangedCondition.waitRelative(mLock, delayUs * 1000ll);
            return true;
        }

        request = *mRequestQueue.begin();
        mRequestQueue.erase(mRequestQueue.begin());
    }

    gRLooperRoster.deliverRequest(request.mARequest);
    return true;
}

}  // namespace android
