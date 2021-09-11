#include "smart_ptr.h"
#include "strategy.h"

typedef SmartPtr<Widget, NoChecking, SingleThread> WidgetPtr;
typedef SmartPtr<Widget, EnforceNotNull, SingleThread> NotNullWidgetPtr;

int main() {
    WidgetPtr widgetPtr(new Widget());
    {
        WidgetPtr::Lock guard(widgetPtr);
    }

    NotNullWidgetPtr notNullWidgetPtr(nullptr);
}