
#include "ref_cnt.h"
#include "scoped_refptr.h"

class WeakReference {
public:
    class Flag : public RefCnt {
    public:
        Flag() : is_valid(true) {};
        ~Flag() {};
        bool is_valid;
    };
    WeakReference() {}
    explicit WeakReference(const Flag* flag) : flag_(flag) {}
    ~WeakReference() {}

    bool is_valid() const { return flag_ && flag_->is_valid; }
private:
    scoped_refptr<const Flag> flag_;
};

template <typename T>
class WeakPtr {
public:
    WeakPtr() : ptr_(0) {}

    T* get() const { return ref_.is_valid() ? ptr_ : 0; }
    operator T*() const { return get(); }
    T* operator->() const {
        return get();
    }

    WeakPtr(const WeakReference& ref, T* ptr) : ref_(ref), ptr_(ptr) {}

private:
    WeakReference ref_;
    T* ptr_;
};

class WeakReferenceOwner {
public:
    WeakReferenceOwner() {}
    ~WeakReferenceOwner() {
        if (flag_) {
            flag_->is_valid = false;
            flag_ = 0;
        }
    }
    WeakReference GetRef() const {
        if (!HasRefs()) {
            flag_ = new WeakReference::Flag();
        }
        return WeakReference(flag_);
    }
    bool HasRefs() const {
        return flag_.get();
    }
private:
    mutable scoped_refptr<WeakReference::Flag> flag_;
};

template <class T>
class SupportsWeakPtr {
public:
    WeakPtr<T> AsWeakPtr() {
        return WeakPtr<T>(weak_reference_owner_.GetRef(), static_cast<T*>(this));
    }
private:
    WeakReferenceOwner weak_reference_owner_;
};

#if 0
/////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


class A : public SupportsWeakPtr<A> {
public:
    void run() {
        printf("run !\n");
    }
};


void * _proc(void * arg)
{
    const WeakPtr<A>& a = ((A*)arg)->AsWeakPtr();
    while (true) {
        if (a) {
            a->run();
        } else {
            printf("a has been deleted!\n");
        }
        sleep(1);
    }
    return NULL;
}

int main(int argc, char * argv[])
{
    A * a = new A();
    
    pthread_t   h;
    pthread_create(&h, NULL, _proc, (void*)a);
    sleep(10);
    delete a;
    sleep(10);

    return 0;
}
#endif


