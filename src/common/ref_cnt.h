
#pragma once


class RefCnt {
public:
    RefCnt() : mRefCnt(1) {}
    virtual ~RefCnt() {}
    void ref() const { mRefCnt++; }
    void unref() const {
        if (mRefCnt-- == 1) {
            mRefCnt = 1;
            delete this;
        }
    }
private:
    mutable int mRefCnt;
};


