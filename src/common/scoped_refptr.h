
#pragma once


template <class T>
class scoped_refptr {
public:
    scoped_refptr(): ptr_(0) {}
    scoped_refptr(T* p) : ptr_(p) {
        if (ptr_)
            ptr_->ref();
    }
    scoped_refptr(const scoped_refptr<T>& o) : ptr_(o.ptr_) {
        if (ptr_)
            ptr_->ref();
    }

    ~scoped_refptr() {
        if (ptr_)
            ptr_->unref();
    }

    T* get() const { return ptr_; }
    operator T*() const { return ptr_; }
    T* operator->() const {
        return ptr_;
    }

    scoped_refptr<T>& operator=(T* p) {
        if (p)
            p->ref();
        T* old_ptr = ptr_;
        ptr_ = p;
        if (old_ptr)
            old_ptr->unref();
        return *this;
    }

private:
    T* ptr_;
};



