//
// Created by Stepan on 04.09.2021.
//

#ifndef MODERN_CPP_DESIGN_EXTENDED_STRATEGY_H
#define MODERN_CPP_DESIGN_EXTENDED_STRATEGY_H

#include "strategy.h"

template<typename T>
class PrototypeCreator {
public:
    T* create() {
        return mPrototype == nullptr ? nullptr : mPrototype->clone();
    }

    T* getPrototype() const {
        return mPrototype;
    }

    void setPrototype(T* aPrototype) {
        mPrototype = aPrototype;
    }

protected:
    ~PrototypeCreator() = default;

private:
    T* mPrototype;
};

#endif //MODERN_CPP_DESIGN_EXTENDED_STRATEGY_H
