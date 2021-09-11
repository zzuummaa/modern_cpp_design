#include "extended_strategy.h"

int main() {
    WidgetManager<PrototypeCreator> advancedWidgetManager;
    advancedWidgetManager.switchPrototype(new Widget());

    return 0;
}