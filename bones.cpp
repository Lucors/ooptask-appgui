#include "bones.h"

//Библиотека Tinyxml
#include "tinyxml/tinystr.h"
#include "tinyxml/tinystr.cpp"
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinyxml.cpp"
#include "tinyxml/tinyxmlerror.cpp"
#include "tinyxml/tinyxmlparser.cpp"

using namespace std;
using namespace funcs;




string funcs::fillTab (int count, char sim){
    string str;
    for (int i = 0; i < count; i++){
        str += sim;
    }
    return str;
}
void funcs::wordToLow (string &word){
    for (size_t i = 0; i < word.size(); i++){
        word[i] = static_cast<char>(tolower(word[i]));
    }
}
bool funcs::isWordDigit (string word){
    for (size_t i = 0; i < word.size(); i++){
        if (!isdigit(word[i])){
            return false;
        }
    }
    return true;
}
string funcs::fileNameCheck (string fileName, string extension){
    size_t tmpSize = extension.size() + 1;
    if (fileName.size() > tmpSize){
        string tmpStr = fileName.substr(fileName.size()-tmpSize, tmpSize);
        if (tmpStr != ("." + extension)){
            fileName += ("." + extension);
        }
    }
    else {
        fileName += ("." + extension);
    }
    fileName = FPATH + fileName;

    return fileName;
}
bool funcs::isEmpty (const char * word) {
    if (word != nullptr){
        //Да-да, про sizeof не слышали
        string tmp = static_cast<const char*>(word);
        if (tmp.size() == 0){
            return true;
        }
    }
    return false;
}


/////////////////////
//BASE CLASS
/////////////////////
Base::Base (){
    code = new int(-1);
    name = new string("Неизвестно");
}
Base::~Base(){
}
void Base::setName (string name){
    *this->name = name;
}
void Base::setCode (int code){
    *this->code = code;
}
string *Base::getName (){ return name; }
int *Base::getCode (){ return code; }


/////////////////////
//GPU CLASS
/////////////////////
GPU::GPU(string name, int code, int nup, int freq,
         string mrer){
    defPointers();
    setGPUInfo(name, code, nup, freq, mrer);
}
GPU::GPU (GPU *copyIt){
    this->name = new string(*copyIt->getName());
    this->code = new int(*copyIt->getCode());
    this->nup = new int(*copyIt->getNUP());
    this->freq = new int(*copyIt->getFreq());
    this->mrer = new string(*copyIt->getMRER());
    this->image = new string(*copyIt->getImagePath());
}
GPU::GPU (){
    defPointers();
}
void GPU::defPointers(){
    this->name = new string("Неизвестно");
    this->code = new int(-1);
    this->nup = new int(-1);
    this->freq = new int(-1);
    this->mrer = new string("Неизвестно");
    this->image = new string("blank.png");
}
GPU::~GPU(){
    delete this->name;
    delete this->code;
    delete this->nup;
    delete this->freq;
    delete this->mrer;
    delete this->image;
}
void GPU::setGPUInfo (string name, int code, int nup, int freq,
                      string mrer){
    *this->name = name;
    *this->code = code;
    *this->nup = nup;
    *this->freq = freq;
    *this->mrer = mrer;
    setImage();
}
void GPU::setNUP (int nup){
    *this->nup = nup;
}
void GPU::setFreq (int freq){
    *this->freq = freq;
}
void GPU::setImage (){
    string tmpMRER = *mrer;
    wordToLow(tmpMRER);
    *this->image = "gpuBLANK.png";
    if (tmpMRER == "amd"){
        *this->image = "gpuAMD.png";
    }
    else {
        if (tmpMRER == "nvidia"){
            *this->image = "gpuNVIDIA.png";
        }
        else {
            if (tmpMRER == "intel"){
                *this->image = "gpuINTEL.png";
            }
        }
    }
}
void GPU::setMRER (string mrer){
    *this->mrer = mrer;
    setImage();
}
string GPU::getInfo (){
    string tmpStr;
    tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
    tmpStr += fillTab(6,' ') + "Название процессора: " + *name + "\n";
    tmpStr += fillTab(6,' ') + "Код Процессора: " + to_string(*code) + "\n";
    tmpStr += fillTab(6,' ') + "Кол-во универсальных пр-сов: " + to_string(*nup) + "\n";
    tmpStr += fillTab(6,' ') + "Штатная частота (МГц): " + to_string(*freq) + "\n";
    tmpStr += fillTab(6,' ') + "Производитель: " + *mrer + "\n";
    tmpStr += fillTab(6,' ') + "Путь к картинке: " + *image + "\n";
    tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
    return tmpStr;
}
vector<string> GPU::getVarsVector(){
    vector<string> output;
    output.push_back(to_string(*code));
    output.push_back(*name);
    output.push_back(to_string(*nup));
    output.push_back(to_string(*freq));
    output.push_back(*mrer);
    return output;
}
int *GPU::getNUP (){ return nup; }
int *GPU::getFreq (){ return freq; }
string *GPU::getMRER (){ return mrer; }
string *GPU::getImagePath (){ return image; }
string GPU::writeInfoToXML (){
    string strOut;

    strOut += "\t\t";
    strOut += "<gpu name=\"";
    strOut += *this->name + "\" code='";
    strOut += to_string(*this->code) + "'>\n";

    strOut += "\t\t\t";
    strOut += "<gpuInfo ";
    strOut += "nup=\"" + to_string(*this->nup) + "\" ";
    strOut += "freq=\"" + to_string(*this->freq) + "\" ";
    strOut += "mrer=\"" + *this->mrer + "\"/>\n";

    strOut += "\t\t";
    strOut += "</gpu>\n";

    return strOut;
}
string GPU::printSQLquery (){
    string strOut = "INSERT INTO GPU (name, code, nup, freq, mrer) VALUES (";
    strOut += "'" + *this->name + "', " + to_string(*this->code) + ", " + to_string(*this->nup);
    strOut += ", " + to_string(*this->freq) + ", '" + *this->mrer + "');";

    return strOut;
}



/////////////////////
//MRER CLASS
/////////////////////
MRER::MRER(string name, int code, unsigned short year, string site){
    defPointers();
    setMRERInfo(name, code, year, site);
}
MRER::MRER (MRER *copyIt){
    this->name = new string(*copyIt->getName());
    this->site = new string(*copyIt->getSite());
    this->code = new int(*copyIt->getCode());
    this->fYear = new unsigned short(*copyIt->getFYear());
    this->image = new string(*copyIt->getImagePath());
}
MRER::MRER (){
    defPointers();
}
void MRER::defPointers(){
    this->name = new string("Неизвестно");
    this->site = new string("blank.com");
    this->code = new int(-1);
    this->fYear = new unsigned short(0);
    this->image = new string("blank.png");
}
MRER::~MRER (){
    delete this->name;
    delete this->code;
    delete this->fYear;
    delete this->site;
    delete this->image;
}
void MRER::setMRERInfo (string name, int code, unsigned short year, string site){
    *this->name = name;
    *this->code = code;
    *this->fYear = year;
    *this->site = site;
    setImage();
}
void MRER::setFYear (unsigned short year){
    *this->fYear = year;
}
void MRER::setImage (){
    string tmpMR = *name;
    wordToLow(tmpMR);
    *this->image = tmpMR + ".png";
}
void MRER::setName(string name){
    *this->name = name;
    setImage();
}
void MRER::setSite (string site){
    *this->site = site;
}
string MRER::getInfo (){
    string tmpStr;
    tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
    tmpStr += fillTab(6,' ') + "Название компании: " + *name + "\n";
    tmpStr += fillTab(6,' ') + "Код Производителя: " + to_string(*code) + "\n";
    tmpStr += fillTab(6,' ') + "Год основания: " + to_string(*fYear) + "\n";
    tmpStr += fillTab(6,' ') + "Сайт: " + *site + "\n";
    tmpStr += fillTab(6,' ') + "Путь к картинке: " + *image + "\n";
    tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
    return tmpStr;
}
vector<string> MRER::getVarsVector(){
    vector<string> output;
    output.push_back(to_string(*code));
    output.push_back(*name);
    output.push_back(to_string(*fYear));
    output.push_back(*site);
    return output;
}
unsigned short *MRER::getFYear (){ return fYear; }
string *MRER::getSite (){ return site; }
string *MRER::getImagePath (){ return image; }
string MRER::writeInfoToXML(){
    string strOut;

    strOut += "\t\t";
    strOut += "<mrer name=\"";
    strOut += *this->name + "\" code=\"";
    strOut += to_string(*this->code) + "\">\n";

    strOut += "\t\t\t";
    strOut += "<mrerInfo ";
    strOut += "year=\"" + to_string(*this->fYear) + "\" ";
    strOut += "site=\"" + *this->site + "\"/>\n";

    strOut += "\t\t";
    strOut += "</mrer>\n";
    return strOut;
}
string MRER::printSQLquery (){
    string strOut = "INSERT INTO MRER (name, code, fYear, site) VALUES (";
    strOut += "'" + *this->name + "', " + to_string(*this->code) + ", " + to_string(*this->fYear);
    strOut += ", '" + *this->site + "');";

    return strOut;
}


/////////////////////
//MMR CLASS
/////////////////////
MMR::MMR(int code, unsigned short memory, string type, double bandwidth, int freq){
    defPointers();
    setMMRInfo(code, memory, type, bandwidth, freq);
}
MMR::MMR (MMR *copyIt){
    this->name = new string(*copyIt->getName());
    this->type = new string(*copyIt->getType());
    this->code = new int(*copyIt->getCode());
    this->memory = new unsigned short(*copyIt->getMemory());
    this->bandwidth = new double(*copyIt->getBandW());
    this->freq = new int(*copyIt->getFreq());
}
MMR::MMR (){
    defPointers();
}
MMR::~MMR (){
    delete this->name;
    delete this->code;
    delete this->memory;
    delete this->bandwidth;
    delete this->freq;
    delete this->type;
}
void MMR::defPointers(){
    this->name = new string("Неизвестно");
    this->type = new string("Неизвестно");
    this->code = new int(-1);
    this->memory = new unsigned short(0);
    this->bandwidth = new double(0.0);
    this->freq = new int(0);
}
void MMR::setMMRInfo (int code, unsigned short memory, string type,
                      double bandwidth, int freq){
    *this->name = "Неизвестно";
    *this->code = code;
    *this->memory = memory;
    *this->bandwidth = bandwidth;
    *this->type = type;
    *this->freq = freq;
}
void MMR::setMemory (unsigned short memory){
    *this->memory = memory;
}
void MMR::setBandW (double bandwidth){
    *this->bandwidth = bandwidth;
}
void MMR::setFreq (int freq){
    *this->freq = freq;
}
void MMR::setType (string type){
    *this->type = type;
}
string MMR::getInfo (){
    string tmpStr;
    tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
    tmpStr += fillTab(6,' ') + "Код Памяти: " + to_string(*code) + "\n";
    tmpStr += fillTab(6,' ') + "Тип памяти: " + *type + "\n";
    tmpStr += fillTab(6,' ') + "Объем (Гб): " + to_string(*memory) + "\n";
    tmpStr += fillTab(6,' ') + "Пропуск. способность (Гбайт/сек): " + to_string(*bandwidth) + "\n";
    tmpStr += fillTab(6,' ') + "Штатная частота (МГц): " + to_string(*freq) + "\n";
    tmpStr += fillTab(6,' ') + fillTab(38,'-') + "\n";
    return tmpStr;
}
vector<string> MMR::getVarsVector(){
    vector<string> output;
    output.push_back(to_string(*code));
    output.push_back(to_string(*memory));
    output.push_back(to_string(*bandwidth));
    output.push_back(to_string(*freq));
    output.push_back(*type);
    return output;
}
unsigned short *MMR::getMemory (){ return memory; }
double *MMR::getBandW () {return bandwidth; }
string *MMR::getType (){ return type; }
int *MMR::getFreq (){ return freq; }
string MMR::writeInfoToXML(){
    string strOut;

    strOut += "\t\t";
    strOut += "<mmr code=\"";
    strOut += to_string(*this->code) + "\">\n";

    strOut += "\t\t\t";
    strOut += "<mmrInfo ";
    strOut += "memory=\"" + to_string(*this->memory) + "\" ";
    strOut += "type=\"" + *this->type + "\" ";
    strOut += "bandwidth=\"" + to_string(*this->bandwidth) + "\" ";
    strOut += "freq=\"" +  to_string(*this->freq) + "\"/>\n";

    strOut += "\t\t";
    strOut += "</mmr>\n";
    return strOut;
}
string MMR::printSQLquery (){
    string bandWString = to_string(*this->bandwidth);
    size_t pos = bandWString.find(',');
    if (pos != string::npos){
        bandWString[pos] = '.';
    }
    string strOut = "INSERT INTO MMR (code, memory, type, bandwidth, freq) VALUES (";
    strOut += to_string(*this->code) + ", " + to_string(*this->memory);
    strOut += ", '" + *this->type + "', " + bandWString + ", " + to_string(*this->freq) + ");";

    return strOut;
}


/////////////////////
//GCARD CLASS
/////////////////////
//Устарело~
//    GCard::GCard (string name, int code, int codeMre,
//                  int codeGP, int codeMry, BaseList <GPU> *GPUList,
//                  BaseList <MRER> *MRERList, BaseList <MMR> *MMRList){
//        setGCInfo(name, code, codeMre, codeGP, codeMry,
//                  GPUList, MRERList, MMRList);
//	}
GCard::GCard (string name, int code, GPU *gpu, MRER *mrer, MMR *mmr){
    defPointers();
    setGCInfo(name, code, gpu, mrer, mmr);
}
GCard::GCard (GCard *copyIt){
    this->name = new string(*copyIt->getName());
    this->correctName = new string(*copyIt->getCorrectName());
    this->code = new int(*copyIt->getCode());
    this->image = new string(*copyIt->getImg());
}
GCard::GCard (){
    defPointers();
}
void GCard::defPointers(){
    this->name = new string("Неизвестно");
    this->correctName = new string("Неизвестно");
    this->code = new int(-1);
    this->image = new string("");
}
void GCard::setGCInfo (string name, int code, GPU* tmpGPU, MRER* tmpMRER, MMR* tmpMMR){
    *this->code = code;
    this->currGPU = tmpGPU;
    this->currMRER = tmpMRER;
    this->currMMR = tmpMMR;

    *this->correctName = name;
    setName(*correctName);
    this->codeGPU = currGPU->getCode();
    this->codeMRER = currMRER->getCode();
    this->codeMMR = currMMR->getCode();
}
GCard::~GCard (){
    delete this->name;
    delete this->code;
    delete this->correctName;
    delete this->image;
}
void GCard::setName(string name){
    *this->name = *currMRER->getName() + ' ' + *currGPU->getName() + ' ' + name;
}
void GCard::setImg (std::string demoImg){
    *this->image = demoImg;
}
//Нельзя просто изменить коды привязок, необходимо передать указатели на экземпляры
void GCard::setPtrMRER(MRER *mrer){
    if (mrer != nullptr){
        this->codeMRER = mrer->getCode();
        this->currMRER = mrer;
        if (currGPU != nullptr){
            setName(*correctName);
        }
    }
    else {
        cout << "<> Ошибка! ";
        cout << "nullptr в качестве MRER для GC с кодом " << this->code << endl;
        cout << " <> Привязка не произведена" << endl;
    }
}
void GCard::setPtrGPU (GPU *gpu){
    if (gpu != nullptr){
        this->codeGPU = gpu->getCode();
        this->currGPU = gpu;
        if (currMRER != nullptr){
            setName(*correctName);
        }
    }
    else {
        cout << "<> Ошибка! ";
        cout << "nullptr в качестве GPU для GC с кодом " << this->code << endl;
        cout << " <> Привязка не произведена" << endl;
    }
}
void GCard::setPtrMMR (MMR *mmr){
    if (mmr != nullptr){
        this->codeMMR = mmr->getCode();
        this->currMMR = mmr;
    }
    else {
        cout << "<> Ошибка! ";
        cout << "nullptr в качестве MMR для GC с кодом " << this->code << endl;
        cout << " <> Привязка не произведена" << endl;
    }
}
string GCard::getInfo (){
    string tmpStr;
    tmpStr += fillTab(44,'-') + "\n";
    tmpStr += "Название: " + *name + "\n";
    tmpStr += "Код Видеокарты: " + to_string(*code) + "\n";
    tmpStr += "Код Производ.: " + to_string(*codeMRER) + "\n";
    tmpStr += currMRER->getInfo();
    tmpStr += "Код Граф. процессора: " + to_string(*codeGPU) + "\n";
    tmpStr += currGPU->getInfo();
    tmpStr += "Код Памяти: " + to_string(*codeMMR) + "\n";
    tmpStr += currMMR->getInfo();
    tmpStr += fillTab(44,'-') + "\n\n";
    return tmpStr;
}
vector<string> GCard::getVarsVector(){
    vector<string> output;
    output.push_back(to_string(*code));
    output.push_back(*name);
    output.push_back(to_string(*codeMRER));
    output.push_back(to_string(*codeGPU));
    output.push_back(to_string(*codeMMR));
    output.push_back(*correctName);
    return output;
}
string *GCard::getImg (){ return image; }
string *GCard::getCorrectName(){ return correctName; };
int *GCard::getCodeMRER (){ return codeMRER; }
int *GCard::getCodeGPU (){ return codeGPU; }
int *GCard::getCodeMMR (){ return codeMMR; }
string GCard::writeInfoToXML(){
    string strOut;

    strOut += "\t\t";
    strOut += "<gc name=\"";
    strOut += *this->correctName + "\" code=\"";
    strOut += to_string(*this->code) + "\">\n";

    strOut += "\t\t\t";
    strOut += "<gcInfo ";
    strOut += "mrerCode=\"" + to_string(*this->codeMRER) + "\" ";
    strOut += "gpuCode=\"" + to_string(*this->codeGPU) + "\" ";
    strOut += "mmrCode=\"" + to_string(*this->codeMMR) + "\"/>\n";

    strOut += "\t\t";
    strOut += "</gc>\n";
    return strOut;
}
string GCard::printSQLquery (){
    string strOut = "INSERT INTO GC (name, code, mrerCode, gpuCode, mmrCode) VALUES (";
    strOut += "'" + *this->correctName + "', " + to_string(*this->code) + ", " + to_string(*this->codeMRER);
    strOut += ", " + to_string(*this->codeGPU) + ", " + to_string(*this->codeMMR) + ");";

    return strOut;
}


/////////////////////
//GCARDLIST CLASS
/////////////////////
GCardList::GCardList (){
}
GCardList::GCardList (GCardList *copyGCList, Catalog *currentCatalog){
    hardCopy(copyGCList, currentCatalog);
}
void GCardList::hardCopy(GCardList *copyGCList, Catalog *currentCatalog){
    for (GCard *iter : *copyGCList->getList()){
        GCard *tmpGC = new GCard(iter);
        GPU *tmpGPU = nullptr;
        try {
            tmpGPU = currentCatalog->listGPU->findByCode(*iter->getCodeGPU());
        }
        catch(...){}
        MRER *tmpMRER = nullptr;
        try {
            tmpMRER = currentCatalog->listMRER->findByCode(*iter->getCodeMRER());
        }
        catch(...){}
        MMR *tmpMMR = nullptr;
        try {
            tmpMMR = currentCatalog->listMMR->findByCode(*iter->getCodeMMR());
        }
        catch(...){}

        tmpGC->setPtrGPU(tmpGPU);
        tmpGC->setPtrMRER(tmpMRER);
        tmpGC->setPtrMMR(tmpMMR);

        this->list.push_back(tmpGC);
    }
}
GCardList::~GCardList(){
}
//Устарело~
//    void GCardList::refresh (BaseList <GPU> *GPUList, BaseList <MRER> *MRERList, BaseList <MMR> *MMRList){
//        for (int i = 0; i < static_cast<int>(list.size()); i++){
//            try {
//                list[i].findGPU(GPUList);
//                list[i].findMRER(MRERList);
//                list[i].findMMR(MMRList);
//            }
//            catch (string errLog){
//                cout << endl;
//                cout << errLog << endl;
//                cout << " <> Ошибка обновления привязок видеокарты с кодом " << list[i].getCode() << endl;
//                cout << endl;
//            }
//        }
//    }
GCard *GCardList::gpuBindCheck (int code){
    for (size_t i = 0; i < list.size(); i++){
        if (*list[i]->getCodeGPU() == code){
            return list[i];
        }
    }
    return nullptr;
}
GCard *GCardList::mrerBindCheck (int code){
    for (size_t i = 0; i < list.size(); i++){
        if (*list[i]->getCodeMRER() == code){
            return list[i];
        }
    }
    return nullptr;
}
GCard *GCardList::mmrBindCheck (int code){
    for (size_t i = 0; i < list.size(); i++){
        if (*list[i]->getCodeMMR() == code){
            return list[i];
        }
    }
    return nullptr;
}
void GCardList::checkPtrsVars(){
    for (GCard *it : list){
        it->setName(*it->getCorrectName());
    }
}



/////////////////////
//CATALOG CLASS
////////////////////
Catalog::Catalog (){
    listGPU = new BaseList <GPU>;
    listMRER = new BaseList <MRER>;
    listMMR = new BaseList <MMR>;
    listGC = new GCardList;
    setObjNames();
}
Catalog::Catalog (Catalog *copyIt){
    listGPU = new BaseList <GPU>(copyIt->getListGPU());
    listMRER = new BaseList <MRER>(copyIt->getListMRER());
    listMMR = new BaseList <MMR>(copyIt->getListMMR());
    listGC = new GCardList(copyIt->getListGC(), this);
    setObjNames();
}
Catalog::~Catalog(){
    listGC->clearAll();
    listGPU->clearAll();
    listMRER->clearAll();
    listMMR->clearAll();
}
void Catalog::copy(Catalog *copyIt){
    clearAll();
    listGPU->copy(copyIt->getListGPU());
    listMRER->copy(copyIt->getListMRER());
    listMMR->copy(copyIt->getListMMR());
    listGC->hardCopy(copyIt->getListGC(), this);
}
void Catalog::setObjNames(){
    listGC->setObjName("Видеокарта");
    listGPU->setObjName("Граф. процессор");
    listMRER->setObjName("Производитель");
    listMMR->setObjName("Память");
}
//CHECK
bool Catalog::checkListGPU (){
    if (listGPU == nullptr){
        return false;
    }
    else { return true;}
}
bool Catalog::checkListMRER (){
    if (listMRER == nullptr){
        return false;
    }
    else { return true;}
}
bool Catalog::checkListMMR (){
    if (listMMR == nullptr){
        return false;
    }
    else { return true;}
}
bool Catalog::checkListGC (){
    if (listGC == nullptr){
        return false;
    }
    else { return true;}
}
bool Catalog::getSettingStat (){
    if (!checkListGPU() && !checkListMMR() && !checkListMRER() && !checkListGC()){
        return false;
    }
    else {
        return true;
    }
}
void Catalog::checkGCPtrsVars(){
    listGC->checkPtrsVars();
}
//ADD NEW ELEMENT
bool Catalog::addNewGC (string name, int code, int codeMRER,
                        int codeGPU, int codeMMR){
    try {
        this->listGC->findByCode(code);
        cout << " <> Ошибка! Видеокарта с кодом " << code << " есть в списке" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            GPU *tmpGPU = listGPU->findByCode(codeGPU);
            MRER *tmpMRER = listMRER->findByCode(codeMRER);
            MMR *tmpMMR = listMMR->findByCode(codeMMR);
            GCard *tmpCard = new GCard(name, code, tmpGPU, tmpMRER, tmpMMR);
            this->listGC->addNew(tmpCard);
        }
        catch (string errLog){
            if (codeMRER == codeGPU == codeMMR == 0){
                if (listGPU->getSize() > 0
                        && listMRER->getSize() > 0
                        && listMMR->getSize() > 0){
                    GCard *tmpCard = new GCard(name, code, listGPU->at(0), listMRER->at(0), listMMR->at(0));
                    this->listGC->addNew(tmpCard);
                    return true;
                }
            }
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Видеокарта с кодом " << code << " не добавлена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::addNewGPU (string name, int code, int nup, int freq, string mrer){
    try {
        this->listGPU->findByCode(code);
        cout << " <> Ошибка! Граф. процессор с кодом " << code << " есть в списке" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        GPU *tmpGPU = new GPU(name, code, nup, freq, mrer);
        this->listGPU->addNew(tmpGPU);
    }
    return true;
}
bool Catalog::addNewMRER (string name, int code, unsigned short year, string site){
    try {
        this->listMRER->findByCode(code);
        cout << " <> Ошибка! Производитель с кодом " << code << " есть в списке" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        MRER *tmpMRER = new MRER(name, code, year, site);
        this->listMRER->addNew(tmpMRER);
    }
    return true;
}
bool Catalog::addNewMMR (int code, unsigned short memory, string type,
                         double bandwidth, int freq){
    try {
        this->listMMR->findByCode(code);
        cout << " <> Ошибка! Память с кодом " << code << " есть в списке" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        MMR *tmpMMR = new MMR(code, memory, type, bandwidth, freq);
        this->listMMR->addNew(tmpMMR);
    }
    return true;
}
//SET LIST
void Catalog::setListGPU (BaseList<GPU> *tmpList){
    this->listGPU = tmpList;
}
void Catalog::setListMRER (BaseList<MRER> *tmpList){
    this->listMRER = tmpList;
}
void Catalog::setListMMR (BaseList<MMR> *tmpList){
    this->listMMR = tmpList;
}
void Catalog::setListGC (GCardList *tmpList){
    this->listGC = tmpList;
}
//SET CODE FOR ANY
bool Catalog::setGCcode(int oldCode, int newCode){
    try {
        this->listGC->findByCode(newCode);
        cout << " <> Ошибка! Видеокарта с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            this->listGC->findByCode(oldCode)->setCode(newCode);
            cout << " <> Успешно сменен код " << oldCode << " у видеокарты на код " << newCode << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::setGPUcode(int oldCode, int newCode){
    GCard *tmpCard = nullptr;
    for (GCard *it : *listGC->getList()){
        if (*it->getCodeGPU() == oldCode){
            tmpCard = it;
        }
    }
    try {
        this->listGPU->findByCode(newCode);
        cout << " <> Ошибка! Граф. процессор с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            GPU *tmpGPU = this->listGPU->findByCode(oldCode);
            tmpGPU->setCode(newCode);
            if (tmpCard != nullptr){
                tmpCard->setPtrGPU(tmpGPU);
            }
            cout << " <> Успешно сменен код " << oldCode << " у граф. процессора на код " << newCode << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::setMRERcode(int oldCode, int newCode){
    GCard *tmpCard = nullptr;
    for (GCard *it : *listGC->getList()){
        if (*it->getCodeMRER() == oldCode){
            tmpCard = it;
        }
    }
    try {
        this->listMRER->findByCode(newCode);
        cout << " <> Ошибка! Производитель с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            MRER *tmpMRER = this->listMRER->findByCode(oldCode);
            tmpMRER->setCode(newCode);
            if (tmpCard != nullptr){
                tmpCard->setPtrMRER(tmpMRER);
            }
            this->listMRER->findByCode(oldCode)->setCode(newCode);
            cout << " <> Успешно сменен код " << oldCode << " у производителя на код " << newCode << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::setMMRcode(int oldCode, int newCode){
    GCard *tmpCard = nullptr;
    for (GCard *it : *listGC->getList()){
        if (*it->getCodeMMR() == oldCode){
            tmpCard = it;
        }
    }
    try {
        this->listMMR->findByCode(newCode);
        cout << " <> Ошибка! Память с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            MMR *tmpMMR = this->listMMR->findByCode(oldCode);
            tmpMMR->setCode(newCode);
            if (tmpCard != nullptr){
                tmpCard->setPtrMMR(tmpMMR);
            }
            this->listMMR->findByCode(oldCode)->setCode(newCode);
            cout << " <> Успешно сменен код " << oldCode << " у памяти на код " << newCode << endl;
            cout << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::setGCcode (GCard *obj, int newCode){
    try {
        this->listGC->findByCode(newCode);
        cout << " <> Ошибка! Видеокарта с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            obj->setCode(newCode);
            cout << " <> Успешно сменен код у видеокарты на код " << newCode << endl;
            cout << endl;
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::setGPUcode (GPU *obj, int newCode){
    GCard *tmpCard = nullptr;
    for (GCard *it : *listGC->getList()){
        if (*it->getCodeGPU() == *obj->getCode()){
            tmpCard = it;
        }
    }
    try {
        this->listGPU->findByCode(newCode);
        cout << " <> Ошибка! Граф. процессор с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            obj->setCode(newCode);
            cout << " <> Успешно сменен код у граф. проц. на код " << newCode << endl;
            cout << endl;
            if (tmpCard != nullptr){
                tmpCard->setPtrGPU(obj);
            }
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::setMRERcode (MRER *obj, int newCode){
    GCard *tmpCard = nullptr;
    for (GCard *it : *listGC->getList()){
        if (*it->getCodeMRER() == *obj->getCode()){
            tmpCard = it;
        }
    }
    try {
        this->listMRER->findByCode(newCode);
        cout << " <> Ошибка! Производитель с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            obj->setCode(newCode);
            cout << " <> Успешно сменен код у производителя на код " << newCode << endl;
            cout << endl;
            if (tmpCard != nullptr){
                tmpCard->setPtrMRER(obj);
            }
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
bool Catalog::setMMRcode (MMR *obj, int newCode){
    GCard *tmpCard = nullptr;
    for (GCard *it : *listGC->getList()){
        if (*it->getCodeMMR() == *obj->getCode()){
            tmpCard = it;
        }
    }
    try {
        this->listMMR->findByCode(newCode);
        cout << " <> Ошибка! Память с кодом " << newCode << " уже есть в списке" << endl;
        cout << " <> Смена кода не произведена" << endl;
        cout << endl;
        return false;
    }
    catch (...){
        try {
            obj->setCode(newCode);
            cout << " <> Успешно сменен код у памяти на код " << newCode << endl;
            cout << endl;
            if (tmpCard != nullptr){
                tmpCard->setPtrMMR(obj);
            }
        }
        catch (string errLog){
            cout << " <> Ошибка! ";
            cout << errLog << endl;
            cout << " <> Смена кода не произведена" << endl;
            cout << endl;
            return false;
        }
    }
    return true;
}
//find by (any)code
GCard *Catalog::findGCByCodeGPU(int codeGPU){
    for (GCard *it : listGC->list){
        if (*it->getCodeGPU() == codeGPU){
            return it;
        }
    }
    return nullptr;
}
GCard *Catalog::findGCByCodeMRER(int codeMRER){
    for (GCard *it : listGC->list){
        if (*it->getCodeMRER() == codeMRER){
            return it;
        }
    }
    return nullptr;
}
GCard *Catalog::findGCByCodeMMR(int codeMMR){
    for (GCard *it : listGC->list){
        if (*it->getCodeMMR() == codeMMR){
            return it;
        }
    }
    return nullptr;
}
//SET CODE FOR GC
bool Catalog::setGCcodeGPU (int i, int gpuCode){
    try {
        this->listGC->findByCode(i)->setPtrGPU(this->listGPU->findByCode(gpuCode));
    }
    catch (string errLog){
        cout << errLog << endl;
        cout << " <> Привязка не произведена" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::setGCcodeMRER (int i, int mrerCode){
    try {
        this->listGC->findByCode(i)->setPtrMRER(this->listMRER->findByCode(mrerCode));
    }
    catch (string errLog){
        cout << errLog << endl;
        cout << " <> Привязка не произведена" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::setGCcodeMMR (int i, int mmrCode){
    try {
        this->listGC->findByCode(i)->setPtrMMR(this->listMMR->findByCode(mmrCode));
    }
    catch (string errLog){
        cout << errLog << endl;
        cout << " <> Привязка не произведена" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::setGCcodeGPU (GCard *obj, int gpuCode){
    try {
        GPU *tmp = this->listGPU->findByCode(gpuCode);
        obj->setPtrGPU(tmp);
    }
    catch (string errLog){
        cout << errLog << endl;
        cout << " <> Привязка не произведена" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::setGCcodeMRER (GCard *obj, int mrerCode){
    try {
        MRER *tmp = this->listMRER->findByCode(mrerCode);
        obj->setPtrMRER(tmp);
    }
    catch (string errLog){
        cout << errLog << endl;
        cout << " <> Привязка не произведена" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::setGCcodeMMR (GCard *obj, int mmrCode){
    try {
        MMR *tmp = this->listMMR->findByCode(mmrCode);
        obj->setPtrMMR(tmp);
    }
    catch (string errLog){
        cout << errLog << endl;
        cout << " <> Привязка не произведена" << endl;
        cout << endl;
        return false;
    }
    return true;
}
//CLEAR
void Catalog::listGPUclear (){
    this->listGPU->clearAll();
}
void Catalog::listGCclear (){
    this->listGC->clearAll();
}
void Catalog::listMRERclear (){
    this->listMRER->clearAll();
}
void Catalog::listMMRclear (){
    this->listMMR->clearAll();
}
void Catalog::clearAll (){
    listGCclear();
    listGPUclear();
    listMMRclear();
    listMRERclear();
}
//DELETE ELEMENT
bool Catalog::deleteGC (int code){
    try {
        listGC->deleteElement(code);
        cout << " <> Успешно удалена видеокарта с кодом " << code << endl;
        cout << endl;
    }
    catch (string errLog){
        cout << " <> Ошибка! ";
        cout << errLog << endl;
        cout << " <> Удаление не произведено" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::deleteGPU (int code){
    try {
        GCard *tmpGC = listGC->gpuBindCheck(code);
        if (tmpGC != nullptr){
            throw "Граф. процессор с кодом " + to_string(code) +
                    " привязан к видеокарте с кодом " + std::to_string(*tmpGC->getCode());
        }
        listGPU->deleteElement(code);
        cout << " <> Успешно удален граф. процессор с кодом " << code << endl;
        cout << endl;
    }
    catch (string errLog){
        cout << " <> Ошибка! ";
        cout << errLog << endl;
        cout << " <> Удаление не произведено" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::deleteMRER (int code){
    try {
        GCard *tmpGC = listGC->mrerBindCheck(code);
        if (tmpGC != nullptr){
            throw "Производитель с кодом " + std::to_string(code) +
                    " привязан к видеокарте с кодом " + std::to_string(*tmpGC->getCode());
        }
        listMRER->deleteElement(code);
        cout << " <> Успешно удален производитель с кодом " << code << endl;
        cout << endl;
    }
    catch (string errLog){
        cout << " <> Ошибка! ";
        cout << errLog << endl;
        cout << " <> Удаление не произведено" << endl;
        cout << endl;
        return false;
    }
    return true;
}
bool Catalog::deleteMMR (int code){
    try {
        GCard *tmpGC = listGC->mmrBindCheck(code);
        if (tmpGC != nullptr){
            throw "Память с кодом " + std::to_string(code) +
                    " привязан к видеокарте с кодом " + std::to_string(*tmpGC->getCode());
        }
        listMMR->deleteElement(code);
        cout << " <> Успешно удалена память с кодом " << code << endl;
        cout << endl;
    }
    catch (string errLog){
        cout << " <> Ошибка! ";
        cout << errLog << endl;
        cout << " <> Удаление не произведено" << endl;
        cout << endl;
        return false;
    }
    return true;
}
//GET SIZE
int Catalog::getListGCsize (){
    return this->listGC->getSize();
}
int Catalog::getListGPUsize (){
    return this->listGPU->getSize();
}
int Catalog::getListMRERsize (){
    return this->listMRER->getSize();
}
int Catalog::getListMMRsize (){
    return this->listMMR->getSize();
}
//GET LIST
BaseList <GPU>* Catalog::getListGPU (){
    return listGPU;
}
BaseList <MRER>* Catalog::getListMRER (){
    return listMRER;
}
BaseList <MMR>* Catalog::getListMMR (){
    return listMMR;
}
GCardList* Catalog::getListGC (){
    return listGC;
}
//GET INFO
string Catalog::getGPUInfo (int code){
    return this->listGPU->findByCode(code)->getInfo();
}
string Catalog::getGCInfo (int code){
    return this->listGC->findByCode(code)->getInfo();
}
string Catalog::getMRERInfo (int code){
    return this->listMRER->findByCode(code)->getInfo();
}
string Catalog::getMMRInfo (int code){
    return this->listMMR->findByCode(code)->getInfo();
}
//PRINT
string Catalog::printListGC (){
    return this->listGC->printAll();
}
string Catalog::printListGP (){
    return this->listGPU->printAll();
}
string Catalog::printListMRER (){
    return this->listMRER->printAll();
}
string Catalog::printListMMR (){
    return this->listMMR->printAll();
}
string Catalog::printAllInfo (){
    string tmpOut;
    tmpOut += "\n";
    tmpOut += "GPU[" + to_string(this->getListGPUsize()) + "]:\n";
    tmpOut += printListGP();
    tmpOut += "Manufacturer[" + to_string(this->getListMRERsize()) + "]:\n";
    tmpOut += printListMRER();
    tmpOut += "Memory[" + to_string(this->getListMMRsize()) + "]:\n";
    tmpOut += printListMMR();
    tmpOut += "VIDEOCARDS[" + to_string(this->getListGCsize()) + "]:\n";
    tmpOut += printListGC();
    return tmpOut;
}

//Чтение из XML файла
bool Catalog::readFromXML (string fileName){
//    fileName = fileNameCheck(fileName, "xml");
    cout << "Файл: " << fileName << endl;
    TiXmlDocument document(fileName.c_str());
    document.LoadFile(TIXML_ENCODING_UTF8);
    if (!document.LoadFile()){
        cout << " <> Ошибка, файл " << fileName << " не найден" << endl;
        return false;
    }
    else {
        TiXmlElement *database = document.FirstChildElement("database");
        if (database){
            //GPU
            for (TiXmlElement *gpulist = database->FirstChildElement("gpuList");
                 gpulist;
                 gpulist = gpulist->NextSiblingElement("gpuList")){
                for (TiXmlElement *gpu = gpulist->FirstChildElement("gpu");
                     gpu;
                     gpu = gpu->NextSiblingElement("gpu")){
                    int intCode = -1, intNup = -1, intFreq = -1;
                    const char *mrer = "Неизвестно";
                    const char *name = gpu->Attribute("name");
                    if (isEmpty(name)){
                        name = "Неизвестно";
                    }
                    const char *code = gpu->Attribute("code");
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *gpuInfo = gpu->FirstChildElement("gpuInfo");
                    if (gpuInfo){
                        const char *nup = gpuInfo->Attribute("nup");
                        if (!isEmpty(nup)){
                            intNup = atoi(nup);
                        }
                        const char *freq = gpuInfo->Attribute("freq");
                        if (!isEmpty(freq)){
                            intFreq = atoi(freq);
                        }
                        if (!isEmpty(gpuInfo->Attribute("mrer"))){
                            mrer = gpuInfo->Attribute("mrer");
                        }
                    }

                    this->addNewGPU(static_cast<const char*>(name), intCode, intNup, intFreq, static_cast<const char*>(mrer));
                }
            }

            //MANUFACTURE
            for (TiXmlElement *manlist = database->FirstChildElement("mrerList");
                 manlist;
                 manlist = manlist->NextSiblingElement("mrerList")){
                for (TiXmlElement *manf = manlist->FirstChildElement("mrer");
                     manf;
                     manf = manf->NextSiblingElement("mrer")){
                    const char *site = "Неизвестно", *year;
                    unsigned short shortYear = 0;
                    int intCode = -1;
                    const char *name = manf->Attribute("name");
                    if (isEmpty(name)){
                        name = "Неизвестно";
                    }
                    const char *code = manf->Attribute("code");
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *manfInfo = manf->FirstChildElement("mrerInfo");
                    if (manfInfo){
                        year = manfInfo->Attribute("fYear");
                        if (!isEmpty(year)){
                            shortYear = static_cast<unsigned short>(atoi(year));
                        }
                        if (!isEmpty(manfInfo->Attribute("site"))){
                            site = manfInfo->Attribute("site");
                        }
                    }

                    this->addNewMRER(static_cast<const char*>(name), intCode, shortYear, static_cast<const char*>(site));
                }
            }

            //MEMORY
            for (TiXmlElement *memlist = database->FirstChildElement("mmrList");
                 memlist;
                 memlist = memlist->NextSiblingElement("mmrList")){
                for (TiXmlElement *mem = memlist->FirstChildElement("mmr");
                     mem;
                     mem = mem->NextSiblingElement("mmr")){
                    const char *type = "Неизвестно";
                    double dbBand = 0.0;
                    const char *code = mem->Attribute("code");
                    int intCode = -1, intFreq = 0;
                    unsigned short shortMem = 0;
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *memInfo = mem->FirstChildElement("mmrInfo");
                    if (memInfo){
                        const char *memory = memInfo->Attribute("memory");
                        if (!isEmpty(memory)){
                            shortMem = static_cast<unsigned short>(atoi(memory));
                        }
                        if (!isEmpty(memInfo->Attribute("type"))){
                            type = memInfo->Attribute("type");
                        }
                        const char *bandwidth = memInfo->Attribute("bandwidth");
                        if (!isEmpty(bandwidth)){
                            dbBand = atof(bandwidth);
                        }
                        const char *freq = memInfo->Attribute("freq");
                        if (!isEmpty(freq)){
                            intFreq = atoi(freq);
                        }
                    }

                    this->addNewMMR(intCode, shortMem, static_cast<const char*>(type), dbBand, intFreq);
                }
            }

            //VIDEOCARDS
            for (TiXmlElement *gclist = database->FirstChildElement("gcList");
                 gclist;
                 gclist = gclist->NextSiblingElement("gcList")){
                for (TiXmlElement *gc = gclist->FirstChildElement("gc");
                     gc;
                     gc = gc->NextSiblingElement("gc")){
                    int intCode = -1, intManCode = -1,  intGPUCode = -1, intMemCode = -1;
                    const char *name = gc->Attribute("name");
                    if (isEmpty(name)){
                        name = "Неизвестно";
                    }
                    const char *code = gc->Attribute("code");
                    if (!isEmpty(code)){
                        intCode = atoi(code);
                    }

                    TiXmlElement *gcInfo = gc->FirstChildElement("gcInfo");
                    if (gcInfo){
                        const char *manCode = gcInfo->Attribute("mrerCode");
                        if (!isEmpty(manCode)){
                            intManCode = atoi(manCode);
                        }
                        const char *gpuCode = gcInfo->Attribute("gpuCode");
                        if (!isEmpty(gpuCode)){
                            intGPUCode = atoi(gpuCode);
                        }
                        const char *memCode = gcInfo->Attribute("mmrCode");
                        if (!isEmpty(memCode)){
                            intMemCode = atoi(memCode);
                        }
                    }
                    this->addNewGC(static_cast<const char*>(name), intCode, intManCode, intGPUCode, intMemCode);
                }
            }
        }
        else {
            cout << "Неккоретный файл" << endl;
            return false;
        }
        //Устарело~
        //        listGC->refresh(*&listGPU, *&listMR, *&listMMR);
        return true;
    }
}

//Запись в XML файл
void Catalog::writeToXML (string fileName){
    //Не требуется проверять имя файла при работе в GUI
//    fileName = fileNameCheck(fileName, "xml");
    ofstream fileOUT;
    fileOUT.open(fileName.c_str());
    cout << "Файл: " << fileName << endl;

    fileOUT << "<?xml version='1.0' encoding=\"utf-8\" standalone=\"yes\"?>" << endl;
    fileOUT << endl;
    fileOUT << "<database>" << endl;

    if(this->getListGPU()->getSize() != 0){
        fileOUT << "\t<gpuList>" << endl;
        fileOUT << this->getListGPU()->printXML();
        fileOUT << "\t</gpuList>" << endl;
    }
    if(this->getListMMR()->getSize() != 0){
        fileOUT << "\t<mmrList>" << endl;
        fileOUT << this->getListMMR()->printXML();
        fileOUT << "\t</mmrList>" << endl;
    }
    if(this->getListMRER()->getSize() != 0){
        fileOUT << "\t<mrerList>" << endl;
        fileOUT << this->getListMRER()->printXML();
        fileOUT << "\t</mrerList>" << endl;
    }
    if(this->getListGC()->getSize() != 0){
        fileOUT << "\t<gcList>" << endl;
        fileOUT << this->getListGC()->printXML();
        fileOUT << "\t</gcList>" << endl;
    }

    fileOUT << "</database>" << endl;
    fileOUT.close();
}


//CALLBACK для sqlite3_exec
//Callback функции в конкретно нашем случае не могут быть вызваны как не-статичные
//методы Catalog, поэтому они описаны как статичные функции, где указатель на каталог
//это глобальная static переменная

//GPU HANDLER
//Предупреждение о неизбежной устарелой переменной
int callbackGPU(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        int code = atoi(colArg[0]);
        string name = colArg[1];
        int nup = atoi(colArg[2]);
        int freq = atoi(colArg[3]);
        string mrer = colArg[4];
        currentCatalog->addNewGPU(name, code, nup, freq, mrer);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}
//MRER HANDLER
//Предупреждение о неизбежной устарелой переменной
int callbackMRER(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        int code = atoi(colArg[0]);
        string name = colArg[1];
        unsigned short year =static_cast<unsigned short>(atoi(colArg[2]));
        string site = colArg[3];
        currentCatalog->addNewMRER(name, code, year, site);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}
//MMR HANDLER
//Предупреждение о неизбежной устарелой переменной
int callbackMMR(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        int code = atoi(colArg[0]);
        unsigned short memory = static_cast<unsigned short>(atoi(colArg[1]));
        double band = atof(colArg[2]);
        int freq = atoi(colArg[3]);
        string type = colArg[4];
        currentCatalog->addNewMMR(code, memory, type, band, freq);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}
//GC HANDLER
//Предупреждение о неизбежной устарелой переменной
int callbackGC(void *obsolete, int tableCol, char **colArg, char **colName) {
    if (tableCol > 0){
        int code = atoi(colArg[0]);
        string name = colArg[1];
        int mrerCode = atoi(colArg[2]);
        int gpuCode = atoi(colArg[3]);
        int mmrCode = atoi(colArg[4]);
        currentCatalog->addNewGC(name, code, mrerCode, gpuCode, mmrCode);
    }
    else {
        cout << "<> Ошибка! Запрос вернул пустую строку" << endl;
    }
    return 0;
}

//Reading from SQLite db
bool Catalog::readFromSQL (string fileName){
    //Не требуется проверять имя файла при работе в GUI
//    fileName = fileNameCheck(fileName, "db");

    cout << "Файл: " << fileName << endl;
    ifstream fileCheck(fileName.c_str());
    if (!fileCheck.is_open()){
        cout << " <> Ошибка, файл " << fileName << " не найден" << endl;
        fileCheck.close();
        return false;
    }
    else {
        fileCheck.close();
    }

    sqlite3 *database;
    char *errMsg = nullptr;
    const char *sql;
    int tmpExec;
    currentCatalog = this;

    tmpExec = sqlite3_open(fileName.c_str(), &database);
    if (tmpExec){
        cout << " <> Невозможно открыть базу данных: " << sqlite3_errmsg(database) << endl;
        return false;
    }
    else {
        cout << "База данных открыта успешно" << endl;
        try {
            sql = "SELECT * FROM GPU;";
            tmpExec = sqlite3_exec(database, sql, callbackGPU, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << this->getListGPU()->getObjName() << " OK" << endl;
            sql = "SELECT * FROM MRER;";
            tmpExec = sqlite3_exec(database, sql, callbackMRER, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << this->getListMRER()->getObjName() << " OK" << endl;
            sql = "SELECT * FROM MMR;";
            tmpExec = sqlite3_exec(database, sql, callbackMMR, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << this->getListMMR()->getObjName() << " OK" << endl;
            sql = "SELECT * FROM GC;";
            tmpExec = sqlite3_exec(database, sql, callbackGC, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            cout << "\t" << this->getListGC()->getObjName() << " OK" << endl;
        }
        catch (char *errLog){
            cout << " <> Ошибка! Не выполнен запрос: " << sql << endl;
            cout << " <> Ошибка SQL: " << errLog << endl;
            cout << " <> Чтение прервано" << endl;
            sqlite3_free(errMsg);
        }
    }
    sqlite3_close(database);
    return true;
}

//Шаблон sql запроса
template < typename T >
void Catalog::execLibSQLquery (T inputLib, sqlite3 *database){
    string sql;
    char *errMsg;
    int tmpExec;

    for (int i = 0; i < inputLib->getSize(); i++){
        sql = inputLib->createSQLquery(i);
        try {
            tmpExec = sqlite3_exec(database, sql.c_str(), nullptr, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
        }
        catch (char *errLog){
            cout << " <> Ошибка! Не выполнен запрос: " << sql << endl;
            cout << " <> Ошибка SQL: " << errLog << endl;
            sqlite3_free(errMsg);
        }
    }
    cout << "\t" << inputLib->getObjName() << " OK" << endl;
}

//Запись в бд SQLite
bool Catalog::writeToSQL (string fileName){
    //Не требуется проверять имя файла при работе в GUI
//    fileName = fileNameCheck(fileName, "db");
    cout << "Файл: " << fileName << endl;
    sqlite3 *database;
    char *errMsg = nullptr;
    int tmpExec;

    tmpExec = sqlite3_open(fileName.c_str(), &database);
    if (tmpExec){
        cout << " <> Невозможно открыть базу данных: " << sqlite3_errmsg(database) << endl;
        return false;
    }
    else {
        cout << "База данных открыта успешно" << endl;
        const char *sql = "\
                DROP TABLE IF EXISTS GPU;    \
        DROP TABLE IF EXISTS MRER;    \
        DROP TABLE IF EXISTS MMR;    \
        DROP TABLE IF EXISTS GC;    \
        PRAGMA foreign_keys = ON;\
        CREATE TABLE IF NOT EXISTS GPU (\
                    code INTEGER PRIMARY KEY,\
                    name TEXT UNIQUE,\
                    nup INTEGER,\
                    freq INTEGER,\
                    mrer TEXT\
                    );\
        CREATE TABLE IF NOT EXISTS MRER (\
                    code INTEGER PRIMARY KEY,\
                    name TEXT UNIQUE,\
                    fYear SMALLINT UNSIGNED,\
                    site TEXT\
                    );\
        CREATE TABLE IF NOT EXISTS MMR (\
                    code INTEGER PRIMARY KEY,\
                    memory SMALLINT UNSIGNED,\
                    bandwidth DOUBLE,\
                    freq INTEGER,\
                    type TEXT\
                    );\
        CREATE TABLE IF NOT EXISTS GC (\
                    code INTEGER PRIMARY KEY,\
                    name TEXT,\
                    mrerCode INTEGER REFERENCES MRER(code),\
                    gpuCode INTEGER REFERENCES GPU(code),\
                    mmrCode INTEGER REFERENCES MMR(code)\
                    );";

        try {
            tmpExec = sqlite3_exec(database, sql, nullptr, nullptr, &errMsg);
            if (tmpExec != SQLITE_OK){
                throw errMsg;
            }
            else {
                cout << "\tСтруктуры таблиц OK" << endl;
                execLibSQLquery(this->getListGPU(), database);
                execLibSQLquery(this->getListMMR(), database);
                execLibSQLquery(this->getListMRER(), database);
                execLibSQLquery(this->getListGC(), database);
            }
        }
        catch (char *errLog){
            cout << " <> Ошибка! Не выполнен запрос: " << sql << endl;
            cout << " <> Ошибка SQL: " << errLog << endl;
            sqlite3_free(errMsg);
            return false;
        }
    }
    sqlite3_close(database);
    return true;
}
