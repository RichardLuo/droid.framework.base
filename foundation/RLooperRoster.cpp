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
#define LOG_TAG "RLooperRoster"
#include <utils/Log.h>

#include "RLooperRoster.h"

#include "ADebug.h"
#include "RHandler.h"
#include "ARequest.h"

namespace android {

RLooperRoster::RLooperRoster()
    : mNextHandlerID(1) {
}

RLooper::handler_id RLooperRoster::registerHandler(
        const sp<RLooper> looper, const sp<RHandler> &handler) {
    Mutex::Autolock autoLock(mLock);

    if (handler->id() != 0) {
        CHECK(!"A handler must only be registered once.");
        return INVALID_OPERATION;
    }

    HandlerInfo info;
    info.mLooper = looper;
    info.mHandler = handler;
    RLooper::handler_id handlerID = mNextHandlerID++;
    mHandlers.add(handlerID, info);

    handler->setID(handlerID);

    return handlerID;
}

void RLooperRoster::unregisterHandler(RLooper::handler_id handlerID) {
    Mutex::Autolock autoLock(mLock);

    ssize_t index = mHandlers.indexOfKey(handlerID);

    if (index < 0) {
        return;
    }

    const HandlerInfo &info = mHandlers.valueAt(index);

    sp<RHandler> handler = info.mHandler.promote();

    if (handler != NULL) {
        handler->setID(0);
    }

    mHandlers.removeItemsAt(index);
}

void RLooperRoster::postRequest(
        const sp<ARequest> &req, int64_t delayUs) {
    Mutex::Autolock autoLock(mLock);

    ssize_t index = mHandlers.indexOfKey(req->target());

    if (index < 0) {
        LOGW("failed to post message. Target handler not registered.");
        return;
    }

    const HandlerInfo &info = mHandlers.valueAt(index);

    sp<RLooper> looper = info.mLooper.promote();

    if (looper == NULL) {
        LOGW("failed to post message. "
             "Target handler %d still registered, but object gone.",
             req->target());

        mHandlers.removeItemsAt(index);
        return;
    }

    looper->post(req, delayUs);
}

void RLooperRoster::cancelRequest(const sp<ARequest> &req)
{
    Mutex::Autolock autoLock(mLock);

    ssize_t index = mHandlers.indexOfKey(req->target());

    if (index < 0) {
        LOGW("failed to post request. Target handler not registered.");
        return;
    }

    const HandlerInfo &info = mHandlers.valueAt(index);

    sp<RLooper> looper = info.mLooper.promote();

    if (looper == NULL) {
        LOGW("failed to post request. "
             "Target handler %d still registered, but object gone.",
             req->target());

        mHandlers.removeItemsAt(index);
        return;
    }

    looper->cancel(req);
}


void RLooperRoster::deliverRequest(const sp<ARequest> &req) {
    sp<RHandler> handler;

    {
        Mutex::Autolock autoLock(mLock);

        ssize_t index = mHandlers.indexOfKey(req->target());

        if (index < 0) {
            LOGW("failed to deliver message. Target handler not registered.");
            return;
        }

        const HandlerInfo &info = mHandlers.valueAt(index);
        handler = info.mHandler.promote();

        if (handler == NULL) {
            LOGW("failed to deliver message. "
                 "Target handler %d registered, but object gone.",
                 req->target());

            mHandlers.removeItemsAt(index);
            return;
        }
    }

    handler->onRequestReceived(req);
}

sp<RLooper> RLooperRoster::findLooper(RLooper::handler_id handlerID) {
    Mutex::Autolock autoLock(mLock);

    ssize_t index = mHandlers.indexOfKey(handlerID);

    if (index < 0) {
        return NULL;
    }

    sp<RLooper> looper = mHandlers.valueAt(index).mLooper.promote();

    if (looper == NULL) {
        mHandlers.removeItemsAt(index);
        return NULL;
    }

    return looper;
}

}  // namespace android
