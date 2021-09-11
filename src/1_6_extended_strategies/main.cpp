#include "extended_strategy.h"

int main() {
    WidgetManager<PrototypeCreator> widgetManager;
    widgetManager.setPrototype(new Widget());

    return 0;
}