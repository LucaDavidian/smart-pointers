#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

template <typename T>
class UniquePtr;

template <typename T>
class UniquePtr
{
    template <class> friend class UniquePtr;  // friend template (for generalized move-constructor and move-assignment)
public:
    explicit UniquePtr(T *rawPointer) : mRawPointer(rawPointer) {}
    
    //UniquePtr(UniquePtr const &other) = delete;  // deleted copy-constructor (by default if move operation is declared)
    UniquePtr(UniquePtr &&other);                  // move constructor

    template <typename U>
    UniquePtr(UniquePtr<U> &&other);               // generalized move-constructor
    
    ~UniquePtr();    // destructor

    //UniquePtr &operator=(const UniquePtr &other) = delete;  // deleted copy-assignment operator (by default if move operation is declared)
    UniquePtr &operator=(UniquePtr &&other);                  // move-assignment operator
    
    template <typename U>
    UniquePtr &operator=(UniquePtr<U> &&other);               // generalized move-assignment operator
    
    T *operator->() { return static_cast<const UniquePtr&>(*this).operator->(); }  // member access operator
    T &operator*() { return *static_cast<const UniquePtr&>(*this); }               // dereference operator
    T *operator->() const { return mRawPointer; }  // member access operator - const version
    T &operator*() const { return *mRawPointer; }  // dereference operator - const version
private:
    T *mRawPointer;
};

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) : mRawPointer(other.mRawPointer)
{
    other.mRawPointer = nullptr;
}

template <typename T>
template <typename U>
UniquePtr<T>::UniquePtr(UniquePtr<U> &&other) : mRawPointer(other.mRawPointer)
{
    other.mRawPointer = nullptr;
}

template <typename T>
UniquePtr<T>::~UniquePtr()
{
    delete mRawPointer;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other)
{
    T *temp = mRawPointer;
    mRawPointer = other.mRawPointer;
    other.mRawPointer = temp;

    return *this;
}

template <typename T>
template <typename U>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr<U> &&other)
{
    delete mRawPointer;
    mRawPointer = other.mRawPointer;
    other.mRawPointer = nullptr;

    return *this;
}

#endif  // UNIQUE_PTR_H