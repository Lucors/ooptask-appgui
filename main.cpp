//Local
#include "appgui.h"

int main(int argc, char *argv[]){
    QApplication *app = new QApplication(argc, argv);

    AppGui mainWidget("Каталог видеокарт");
    mainWidget.show();

    return app->exec();
}
