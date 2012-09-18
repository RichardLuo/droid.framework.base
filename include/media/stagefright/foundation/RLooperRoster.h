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

#ifndef R_LOOPER_ROSTER_H_

#define R_LOOPER_ROSTER_H_

#include <media/stagefright/foundation/RLooper.h>
#include <utils/KeyedVector.h>

namespace android {

struct ARequest;

struct RLooperRoster {
    RLooperRoster();

    RLooper::handler_id registerHandler(
            const sp<RLooper> looper, const sp<RHandler> &handler);

    void unregisterHandler(RLooper::handler_id handlerID);

    void postRequest(const sp<ARequest> &msg, int64_t delayUs = 0);
    void deliverRequest(const sp<ARequest> &msg);

    sp<RLooper> findLooper(RLooper::handler_id handlerID);

    void cancelRequest(const sp<ARequest> &req);

private:
    struct HandlerInfo {
        wp<RLooper> mLooper;
        wp<RHandler> mHandler;
    };

    Mutex mLock;
    KeyedVector<RLooper::handler_id, HandlerInfo> mHandlers;
    RLooper::handler_id mNextHandlerID;

    DISALLOW_EVIL_CONSTRUCTORS(RLooperRoster);
};

}  // namespace android

#endif  // R_LOOPER_ROSTER_H_
