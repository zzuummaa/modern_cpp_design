#include "extended_strategy.h"

int main() {
    // Не хотим делать деструктор стратегии виртуальным. Нужна защита от удаления по указателю на базовый класс.
    auto* widgetManager = new WidgetManager<PrototypeCreator>();
    PrototypeCreator<Widget>* pPrototypeCreator = widgetManager;
    // Ошибка компиляции.
    delete pPrototypeCreator;

    return 0;
}