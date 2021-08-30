#ifndef SHARED_PTR_H
#define SHARED_PTR_H

template <typename T>  // forward declaration
class SharedPtr;

template <typename T>  // non-member swap function in same namespace
void swap(SharedPtr<T> &a, SharedPtr<T> &b)
{
    a.Swap(b);  // calls swap member function
}

template <typename T>
class SharedPtr
{
    template <typename> friend class SharedPtr;  // friend template (for generalized constructor and assignment)
public:
    explicit SharedPtr(T *rawPointer) : mRawPointer(rawPointer), mReferenceCount(new unsigned int(1)) {}
    
    SharedPtr(const SharedPtr &other);    // copy-constructor
    SharedPtr(SharedPtr &&other);         // move-constructor

    template <typename U>
    SharedPtr(const SharedPtr<U> &other); // generalized copy-constructor
    template <typename U>
    SharedPtr(SharedPtr<U> &&other); // generalized move-constructor

    ~SharedPtr();    // destructor

    SharedPtr &operator=(const SharedPtr &other);     // copy-assignment operator
    SharedPtr &operator=(const SharedPtr &&other);    // move-assignment operator

    template <typename U>
    SharedPtr &operator=(const SharedPtr<U> &other);  // generalized copy-assignment operator
    template <typename U>
    SharedPtr &operator=(SharedPtr<U> &&other);       // generalized move-assignment operator

    T *operator->() { return static_cast<SharedPtr const&>(*this).operator->(); }  // member access operator
    T &operator*() { return *static_cast<SharedPtr const&>(*this); }               // dereference operator
    T *operator->() const { return mRawPointer; }  // member access operator - const version
    T &operator*() const { return *mRawPointer; }  // dereference operator - const version

    void Swap(SharedPtr &other);    // public swap member function
private:
    T *mRawPointer;
    unsigned int *mReferenceCount;
};

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &other) : mRawPointer(other.mRawPointer), mReferenceCount(other.mReferenceCount)
{
    (*mReferenceCount)++;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T> &&other) : mRawPointer(other.mRawPointer), mReferenceCount(other.mReferenceCount)
{
    other.mRawPointer = nullptr;
}

template <typename T>
template <typename U>
SharedPtr<T>::SharedPtr(const SharedPtr<U> &other) : mRawPointer(other.mRawPointer), mReferenceCount(other.mReferenceCount)
{
    (*mReferenceCount)++;
}

template <typename T>
template <typename U>
SharedPtr<T>::SharedPtr(SharedPtr<U> &&other) : mRawPointer(other.mRawPointer), mReferenceCount(other.mReferenceCount)
{
    other.mRawPointer = nullptr;
}

template <typename T>
SharedPtr<T>::~SharedPtr()
{
    (*mReferenceCount)--;

    if (!*mReferenceCount)
        delete mRawPointer;
}

template <typename T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr const &other)
{
    SharedPtr temp(other);    // copy-and-swap
    Swap(temp);

    return *this;
}

template <typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr &&other)
{
    Swap(std::move(other));

    return *this;
}

template <typename T>
template <typename U>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<U> &other)
{
    SharedPtr temp(other);
    Swap(temp);

    return *this;
}    

template <typename T>
template <typename U>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<U> &&other)
{
    SharedPtr temp(std::move(other));
    Swap(temp);

    return *this;
} 

template <typename T>
void SharedPtr<T>::Swap(SharedPtr &other)
{
    T *tempPtr = other.mRawPointer;
    unsigned int *tempCount = other.mReferenceCount;

    other.mRawPointer = mRawPointer;
    other.mReferenceCount = mReferenceCount;

    mRawPointer = tempPtr;
    mReferenceCount = tempCount;
}

#endif  // SHARED_PTR_H