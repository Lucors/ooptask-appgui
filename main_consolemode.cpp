#include <iostream>
#include <string>
#include <cstdlib>
#include "bones.h"
using namespace std;

/* Справка по проекту:
 * -> Большинство обработанных исключений не возвращают строку с ошибкой,
 * а сразу выводят её в поток cout в формате " <> Ошибка ..."
 * -> Класс Catalog содержит в себе практически все необходимые "прослойки"
 * (методы, которые обращаются к методам вложенных в Catalog списков),
 * однако есть нереализованные прослойки и для доступа к методам вложенных классов
 * необходимо использовать метод getList"listname" для получения указателя на нужный экземпляр класса
 * -> Общие обозначения: GC - Graphic Cards, GPU - gpu, MRER - Manufacturer, MMR - Memory
 * -> Интерфейсы и реализация классов хранятся в файлах bones.h и bones.cpp соответственно
 * -> Файл func.h отвечает за большинство функций в проекте, таких как запись/чтение
 * -> Для работы с xml используется парсер TinyXML, а для работы с бд используется библиотека sqlite3
*/

int main_ConsoleMode () {
    setlocale(LC_ALL, "rus");
    Catalog lib;
    string tmpIn, fileName;

        cout << "Видеокарты!" << endl;
        cout << "help - показать справку" << endl;
        cout << endl;
        while (true){
            cout << ">> ";
            getline(cin, tmpIn);
            tmpIn = tmpIn.substr(0, tmpIn.find_first_of(" "));
            funcs::wordToLow(tmpIn);
            if (tmpIn == "data"){
                cout << lib.printAllInfo();
                cout << endl;
                continue;
            }
            if (tmpIn == "clear"){
                lib.clearAll();
                cout << " <> Списки очищены" << endl;
                cout << endl;
                continue;
            }
            if (tmpIn == "rsql"){
                do {
                    cout << "Введите название файла: " << endl;
                    cout << ">> ";
                    getline(cin, fileName);
                    if (cin.eof()){
                        cout << "Чтение прервано" << endl;
                        cin.clear();
                        break;
                    }
                }
                while (!lib.readFromSQL(fileName));
                cout << "Чтение из базы данных завершено" << endl;
                cout << endl;
                continue;
            }
            if (tmpIn == "rxml"){
                do {
                    cout << "Введите название файла: " << endl;
                    cout << ">> ";
                    getline(cin, fileName);
                    if (cin.eof()){
                        cout << "Чтение прервано" << endl;
                        cin.clear();
                        break;
                    }
                }
                while (!lib.readFromXML(fileName));
                cout << "Чтение из xml завершено" << endl;
                cout << endl;
                continue;
            }
            if (tmpIn == "wsql"){
                cout << "Введите название файла: " << endl;
                cout << ">> ";
                getline(cin, fileName);
                if (cin.eof()){
                    cout << "Запись прервана" << endl;
                    cout << endl;
                    cin.clear();
                    continue;
                }
                if (lib.writeToSQL(fileName)){
                    cout << "Запись в базу данных SQLite завершена" << endl;
                }
                else {
                    cout << " <> Запись в бд SQLite не произведена" << endl;
                }
                cout << endl;
                continue;
            }
            if (tmpIn == "wxml"){
                cout << "Введите название файла: " << endl;
                cout << ">> ";
                getline(cin, fileName);
                if (cin.eof()){
                    cout << "Запись прервана" << endl;
                    cout << endl;
                    cin.clear();
                    continue;
                }
                lib.writeToXML(fileName);
                cout << "Запись в xml завершена" << endl;
                cout << endl;
                continue;
            }
            if (tmpIn == "demo"){
                //Название, код ГП, КУП(Кол-во унив. пр-сов), частота, производитель
                lib.addNewGPU("Radeon RX 560", 1, 896, 1175, "AMD");
                lib.addNewGPU("GeForce GT 710", 2, 192, 954, "NVIDIA");
                lib.addNewGPU("Intel Dual GPU 1321", 3, 673, 1102, "INTEL");
                //Название, код ПРОИЗ, год основания, сайт
                lib.addNewMRER("Palit", 1, 1998, "palit.com");
                lib.addNewMRER("MSI", 2, 1987, "msi.com");
                lib.addNewMRER("NSpeed", 3, 2013, "nspd.com");
                //Код ПАМЯТИ, объем, тип, пропуск. способность
                lib.addNewMMR(1, 4, "GDDR3", 12.8, 0);
                lib.addNewMMR(2, 3, "GDDR5", 40, 0);
                lib.addNewMMR(3, 9, "DDR2", 10.2, 0);
                //Название, код ВК, код Произ., код ГП, код Памяти
                lib.addNewGC("PASSIVE", 1, 1, 1, 1);
                lib.addNewGC("Silent LP", 2, 2, 2, 2);
                lib.addNewGC("Power UP", 3, 1, 1, 1);

                cout << lib.printAllInfo();
                lib.deleteGPU(2);
                lib.deleteMRER(2);
                lib.deleteMMR(2);
                lib.deleteGC(99);
                lib.deleteGPU(99);
                lib.deleteMRER(99);
                lib.deleteMMR(99);
                lib.deleteGC(3);
                lib.deleteGPU(3);
                lib.deleteMRER(3);
                lib.deleteMMR(3);
                lib.setGCcode(3,5);
                lib.setGCcode(2,1);
                lib.setGCcode(1,9);
                lib.setGPUcode(1,8);
                cout << lib.printAllInfo();
                continue;
            }
            if (tmpIn == "help"){
                cout << "Режим чтения: rxml - из файла xml, rsql - из файла SQLite" << endl;
                cout << "Режим вывода: wxml - в файл xml, wsql - в файл SQLite" << endl;
                cout << "data - для вывода информации, demo - демо. режим" << endl;
                cout << "help - показать эту справку, clear - очистить списки" << endl;
                cout << "exit - для выхода" << endl;
                cout << endl;
                continue;
            }
            if (tmpIn == "exit" || cin.eof()){
                cout << endl;
                break;
            }
            else {
                cout << "Ошибка!" << endl;
                cout << endl;
            }
        }

    //in the end it doesn't even matter
    return 0;
}
