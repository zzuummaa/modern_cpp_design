//
// Created by Stepan on 04.09.2021.
//

#ifndef MODERN_CPP_DESIGN_1_5_STRATEGY_CLASS_STRATEGY_H
#define MODERN_CPP_DESIGN_1_5_STRATEGY_CLASS_STRATEGY_H

class Widget{};

class Gadget{};

template <template <typename Created> typename CreationPolicy>
class WidgetManager : public CreationPolicy<Widget> {
public:
    void doSomething() {
        Gadget* gadget = CreationPolicy<Gadget>().create();
    }

    void switchPrototype(Widget* pNewPrototype) {
        CreationPolicy<Widget>& policy = *this;
        delete policy.getPrototype();
        policy.setPrototype(pNewPrototype);
    }
};

template<typename T>
class OpNewCreator {
public:
    static T* create() {
        return new T();
    }
};

#endif //MODERN_CPP_DESIGN_1_5_STRATEGY_CLASS_STRATEGY_H
