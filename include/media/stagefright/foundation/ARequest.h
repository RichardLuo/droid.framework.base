/*
 * Copyright (C) 2012 The X-Live Project
 *
 * @author Richard Luo, cdominant7@gmail.com
 * @date   2012-09-17 22:32:57
 * 
 * 
 ****************************************************************** 
 */

#ifndef _AREQUEST_H
#define _AREQUEST_H


#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/foundation/RLooper.h>
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>

namespace android {

struct AString;

struct ARequest : public RefBase {

    ARequest(uint32_t what = 0, RLooper::handler_id target = 0);

    void setWhat(uint32_t what);
    uint32_t what() const;

    void setTarget(RLooper::handler_id target);
    RLooper::handler_id target() const;

    void post(int64_t delayUs = 0);

    void cancel();

    //protected:
    virtual ~ARequest();

private:
    uint32_t mWhat;
    RLooper::handler_id mTarget;

    DISALLOW_EVIL_CONSTRUCTORS(ARequest);
};

}  // namespace android


#endif /* _AREQUEST_H */



