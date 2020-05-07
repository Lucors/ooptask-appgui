#ifndef BONES_H
#define BONES_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
//Библиотека SQLite из пакета sqlite3
#include <sqlite3.h>

using std::string;
using std::vector;
using std::to_string;

class Catalog;


//Путь к папке с файлами. По-умолчанию, файлы в папке с исполняемым файлом.
const string FPATH = "";
//Предупреждение о неиспользуемой переменной
static Catalog *currentCatalog;


namespace funcs {
    //Заполнения вывода n-нным кол-во символов char
    string fillTab (int, char);
    //Преобр. строки в нижний регистр
    void wordToLow (string &);
    //Проверка строка == число
    bool isWordDigit (string);
    //Проверка имени файла (имя, расширение)
    string fileNameCheck (string, string);

    bool isEmpty (const char*);
}


/////////////////////
//Родительский класс
/////////////////////
class Base {
protected:
    int *code;
    string *name;
public:
    Base();
    virtual ~Base() = 0;
    virtual void setName(string);
    virtual void setCode(int);
    virtual string *getName();
    virtual int *getCode();
    virtual string writeInfoToXML() = 0;
    virtual string printSQLquery() = 0;
};

/////////////////////
//Класс GPU
/////////////////////
class GPU : public Base {
private:
    //Кол-во универсальных процессов? (Number of universal processors)
    int *nup;
    //Штат. частота (MHz)
    int *freq;
    //Производитель (AMD, NVIDIA, INTEL, OTHER)
    string *mrer;
    //Далее служебные поля:
    string *image;
public:
    GPU(string, int, int, int, string);
    GPU(GPU*);
    GPU();
    ~GPU();
    void defPointers();
    void setGPUInfo(string, int, int, int, string);
    void setNUP(int);
    void setFreq(int);
    void setMRER(string);
    void setImage();
    string getInfo();
    vector<string> getVarsVector();
    int *getNUP();
    int *getFreq();
    string *getMRER();
    string *getImagePath();
    //XML
    string writeInfoToXML();
    //SQL
    string printSQLquery();
};

/////////////////////
//Класс MRER
/////////////////////
class MRER : public Base {
private:
    //Год основания (Надеемся, что smallint sql равносилен short)
    unsigned short *fYear;
    //Строка сайт производителя
    string *site;
    //Далее служебные поля:
    string *image;
public:
    MRER(string, int, unsigned short, string);
    MRER(MRER*);
    MRER();
    ~MRER();
    void defPointers();
    void setMRERInfo(string, int, unsigned short, string);
    void setFYear(unsigned short);
    void setName(string);
    void setSite(string);
    void setImage();
    string getInfo();
    vector<string> getVarsVector();
    unsigned short *getFYear();
    string *getSite();
    string *getImagePath();
    //XML
    string writeInfoToXML();
    //SQL
    string printSQLquery();
};

/////////////////////
//Класс MMR
/////////////////////
class MMR : public Base {
private:
    //Объём памяти
    unsigned short *memory;
    //Пропуск. способность
    double *bandwidth;
    //Штат. частота (MHz)
    int *freq;
    //Тип памяти
    string *type;
public:
    MMR(int, unsigned short, string, double, int);
    MMR(MMR*);
    MMR();
    ~MMR();
    void defPointers();
    void setMMRInfo(int, unsigned short, string, double, int);
    void setMemory(unsigned short);
    void setBandW(double);
    void setType(string);
    void setFreq(int);
    string getInfo();
    vector<string> getVarsVector();
    unsigned short *getMemory();
    double *getBandW();
    string *getType();
    int *getFreq();
    //XML
    string writeInfoToXML();
    //SQL
    string printSQLquery();
};


/////////////////////
//BASELIST TEMPLATE
/////////////////////
//Шаблон базового списка
//используется на простых классах
//Manufacturer(MRER), Memory(MMR), GPU
//Родитель для GCardList
////////////////////
template < typename T >
class BaseList {
protected:
    vector < T* > list;
    string objName = "Объект";
public:
    friend Catalog;
    BaseList (){
    }
    BaseList (BaseList *copyIt){
        for (T *iter : *copyIt->getList()){
            list.push_back(new T(iter));
        }
    }
    ~BaseList (){
    }
    T *at (const int index){
        if (list.size() > 0){
            if(index <= list.size()){
                return list[index];
            }
        }
        return nullptr;
    }
    T *findByCode (int code) {
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (*list[i]->getCode() == code){
                return list[i];
            }
        }
        throw (this->objName + " с кодом " + to_string(code) + " не найден(а)");
    }
    vector < T* > *getList(){
        return &this->list;
    }
    void addNew (T *tmpCard){
        list.push_back(tmpCard);
    }
    void clearAll () {
        for (T *iter : list){
            delete iter;
        }
        //lol here is list of nullptr
        list.clear();
    }
    int getSize (){
        return static_cast<int>(list.size());
    }
    void deleteElement (int code){
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (*list[i]->getCode() == code){
                delete list[i];
                list.erase(list.begin() + i);
                return;
            }
        }
        throw this->objName + " с кодом " + to_string(code) + " отсутствует в списке";
    }
    string printAll(){
        string tmpStr;
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            tmpStr += list[i]->getInfo();
            tmpStr += "\n";
        }
        return tmpStr;
    }
    string printXML(){
        string strOut;
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            strOut += list[i]->writeInfoToXML();
        }
        return strOut;
    }
    string createSQLquery(int position){
        string strOut = "";
        for (int i = 0; i < static_cast<int>(list.size()); i++){
            if (i == position){
                strOut = list[i]->printSQLquery();
            }
        }
        return strOut;
    }
    bool checkCode(int code){
        for (int i = 0; i < list.size(); i++){
            if (*list[i]->getCode() == code){
                return false;
            }
        }
        return true;
    }
    void setObjName (string objName){
        this->objName = objName;
    }
    string getObjName (){
        return this->objName;
    }
    void copy(BaseList<T> *copyList){
        clearAll();
        for (T *it : *copyList->getList()){
            list.push_back(new T(it));
        }
    }
};

/////////////////////
//Класс GCARD
/////////////////////
class GCard : public Base {
private:
    string *correctName;
    string *image;
    int *codeMRER, *codeGPU, *codeMMR;
    GPU *currGPU = nullptr;
    MRER *currMRER = nullptr;
    MMR *currMMR = nullptr;
public:
    GCard (string, int, GPU*, MRER*, MMR*);
    GCard ();
    GCard (GCard*);
    ~GCard();
    void defPointers();
    //SET Функции
    void setGCInfo (string, int, GPU*, MRER*, MMR*);
    void setName(string);
    void setImg (string);
    void setPtrMRER (MRER*);
    void setPtrGPU (GPU*);
    void setPtrMMR (MMR*);
    //GET Функции
    string getInfo();
    vector<string> getVarsVector();
    string *getImg();
    string *getCorrectName();
    int *getCodeMRER();
    int *getCodeGPU();
    int *getCodeMMR();
//    //FIND Функции
//    void findGPU(BaseList <GPU>*);
//    void findMRER(BaseList <MRER>*);
//    void findMMR(BaseList <MMR>*);
    //XML
    string writeInfoToXML ();
    //SQL
    string printSQLquery ();
};


/////////////////////
//Класс GCARDLIST
/////////////////////
class GCardList : public BaseList <GCard> {
private:
public:
    friend Catalog;
    GCardList ();
    GCardList (GCardList *, Catalog*);
    ~GCardList ();
    void hardCopy(GCardList *, Catalog*);
    GCard *gpuBindCheck (int);
    GCard *mrerBindCheck (int);
    GCard *mmrBindCheck (int);
    void checkPtrsVars();
};



/////////////////////
//Класс CATALOG
/////////////////////
class Catalog {
private:
    BaseList <GPU> *listGPU;
    BaseList <MRER> *listMRER;
    BaseList <MMR> *listMMR;
    GCardList *listGC;
public:
    friend GCardList;
    Catalog ();
    Catalog (Catalog*);
    ~Catalog ();
    void setObjNames();
    void copy(Catalog*);
    //Проверки
    bool checkListGPU ();
    bool checkListMRER ();
    bool checkListMMR ();
    bool checkListGC ();
    bool getSettingStat ();
    void checkGCPtrsVars();
    //Добавление элемента
    bool addNewGC (string = "Неизвестно", int = 0, int = 0, int = 0, int = 0);
    bool addNewGPU (string = "Неизвестно", int = 0, int = 0, int = 0, string = "Неизвестно");
    bool addNewMRER (string = "Неизвестно", int = 0, unsigned short = 0, string = "Неизвестно");
    bool addNewMMR (int = 0, unsigned short = 0, string = "Неизвестно", double = 0.0, int = 0);
    //SET LIST
    void setListGPU (BaseList <GPU>*);
    void setListMRER (BaseList <MRER>*);
    void setListMMR (BaseList <MMR>*);
    void setListGC (GCardList*);
    //Установка поля CODE для ANY
    bool setGCcode (int, int);
    bool setGPUcode (int, int);
    bool setMRERcode (int, int);
    bool setMMRcode (int, int);
    bool setGCcode (GCard*, int);
    bool setGPUcode (GPU*, int);
    bool setMRERcode (MRER*, int);
    bool setMMRcode (MMR*, int);
    //find by (any)code
    GCard *findGCByCodeGPU(int);
    GCard *findGCByCodeMRER(int);
    GCard *findGCByCodeMMR(int);
    //Установка поля CODEANY для GC
    bool setGCcodeGPU (int, int);
    bool setGCcodeMRER (int, int);
    bool setGCcodeMMR (int, int);
    bool setGCcodeGPU (GCard*, int);
    bool setGCcodeMRER (GCard*, int);
    bool setGCcodeMMR (GCard*, int);
    //Очистка
    void listGPUclear ();
    void listGCclear ();
    void listMRERclear ();
    void listMMRclear ();
    void clearAll ();
    //Удаление
    bool deleteGC (int);
    bool deleteGPU (int);
    bool deleteMRER (int);
    bool deleteMMR (int);
    //Запрос размера
    int getListGCsize ();
    int getListGPUsize ();
    int getListMRERsize ();
    int getListMMRsize ();
    //Запрос списков
    BaseList <GPU> *getListGPU ();
    BaseList <MRER> *getListMRER();
    BaseList <MMR> *getListMMR ();
    GCardList *getListGC();
    //Запрос информации
    string getGPUInfo(int);
    string getGCInfo(int);
    string getMRERInfo(int);
    string getMMRInfo(int);
    //Вывод
    string printListGC ();
    string printListGP ();
    string printListMRER ();
    string printListMMR ();
    string printAllInfo ();
    //XML
    bool readFromXML(string);
    void writeToXML(string);
    //SQL
    bool readFromSQL(string);
    template < typename T >
    void execLibSQLquery (T inputLib, sqlite3 *);
    bool writeToSQL(string);
};

#endif
