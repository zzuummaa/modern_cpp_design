//
// Created by Stepan on 07.09.2021.
//

#ifndef MODERN_CPP_DESIGN_SMART_PTR_H
#define MODERN_CPP_DESIGN_SMART_PTR_H

#include <stdexcept>

template<
    typename T,
    template<typename> typename CheckingPolicy,
    template<typename> typename ThreadingModel
>
class SmartPtr
    : public CheckingPolicy<T>,
      public ThreadingModel<SmartPtr<T, CheckingPolicy, ThreadingModel>> {

    typedef ThreadingModel<SmartPtr<T, CheckingPolicy, ThreadingModel>> ThreadingModelType;
    typedef CheckingPolicy<T> CheckingPolicyType;

public:
    explicit SmartPtr(T *aPtr) {
        typename ThreadingModelType::Lock guard(*this);
        CheckingPolicyType::check(aPtr);
        mPtr = aPtr;
    }

    T& operator->() {
        typename ThreadingModelType::Lock guard(*this);
        CheckingPolicyType::check(mPtr);
        return *mPtr;
    }

    const T& operator->() const {
        typename ThreadingModelType::Lock guard(*this);
        CheckingPolicyType::check(mPtr);
        return *mPtr;
    }

private:
    T* mPtr;
};

template<typename T>
class NoChecking {
public:
    static void check(T*) {}
};

template<typename T>
class EnforceNotNull {
public:
    static void check(T*& ptr) {
        if (ptr == nullptr) throw std::invalid_argument("mPtr is null");
    }
};

template<typename T>
class SingleThread{
public:
    class Lock {
    public:
        explicit Lock(const T&) {};
    };
};

#endif //MODERN_CPP_DESIGN_SMART_PTR_H
