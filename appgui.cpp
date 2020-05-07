#include "appgui.h"


TableBase::TableBase(QWidget *parent) : QWidget (parent){
    mVLayout = new QVBoxLayout(this);
    mHLayout = new QHBoxLayout();
    editorLayout = new QHBoxLayout();
    buttsLayout = new QVBoxLayout();

    table = new QTableWidget();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(nullptr);

    pushAdd = new QPushButton("Добавить");
    pushDel = new QPushButton("Удалить");
    pushUndo = new QPushButton("Отмениь");
    pushRefr = new QPushButton("Обновить");

    buttsLayout->addWidget(pushAdd);
    buttsLayout->addWidget(pushDel);
    buttsLayout->addWidget(pushUndo);
    buttsLayout->addWidget(pushRefr);
    mHLayout->addWidget(table);
    mHLayout->addLayout(buttsLayout);
    mVLayout->addLayout(mHLayout);
    mVLayout->addLayout(editorLayout);

    pushDel->setEnabled(false);
    connect(pushRefr, &QPushButton::clicked, this, [=](){
        emit this->needRefresh();
    });
    connect(pushUndo, &QPushButton::clicked, this, [=](){
        emit this->needUndo();
    });
    connect(table, &QTableWidget::cellPressed, this, [=](int row){
        this->pushDel->setEnabled(true);
        this->currentCellRow = row;
        cout << "Current row:" << currentCellRow << endl;
    });
}
TableBase::~TableBase(){
    delete table;
    delete pushAdd;
    delete pushUndo;
    delete pushRefr;
    delete buttsLayout;
    delete mHLayout;
    delete editorLayout;
    delete mVLayout;
}
void TableBase::blockDelButton(){
    this->pushDel->setEnabled(false);
}
void TableBase::clearTable(){
    this->table->setRowCount(0);
}
void TableBase::updateCatalog(Catalog *newOrig){
    this->originalCatalog = newOrig;
    updateAllCells();
}


TableGPU::TableGPU(Catalog *originalCatalog) : TableBase (){
    const QStringList headers = {"Код", "Название", "NUP", "Частота", "Производитель"};
    this->table->setColumnCount(5);
    this->table->setHorizontalHeaderLabels(headers);

    this->originalCatalog = originalCatalog;
    updateAllCells();

    defEditorContent();
    defEditorConnections();

    connect(pushAdd, &QPushButton::clicked, this, [=](){
        if (originalCatalog->addNewGPU()){
            this->table->insertRow(table->rowCount());
            this->createCellByRow(table->rowCount()-1);
            currentCellRow = table->rowCount()-1;
            updateEditorContent(table->rowCount()-1);
        }
    });
    connect(pushDel, &QPushButton::clicked, this, [=](){
        int codeGPU = table->item(currentCellRow, 0)->text().toInt();
        if (originalCatalog->deleteGPU(codeGPU)){
            this->table->removeRow(currentCellRow);
            if (table->rowCount() != 0){
                if (currentCellRow-1 != -1){
                    currentCellRow--;
                }
                cout << "Current row:" << currentCellRow << endl;
            }
            else {
                blockDelButton();
            }
        }
    });
    connect(table, &QTableWidget::cellPressed, this, &TableGPU::updateEditorContent);
}
TableGPU::~TableGPU(){
    delete codeValue;
    delete nameValue;
    delete nupValue;
    delete freqValue;
    delete mrerValue;
}
void TableGPU::updateAllCells(){
    clearTable();
    for (size_t it = 0; it < originalCatalog->getListGPU()->getList()->size(); it++){
        this->table->insertRow(static_cast<int>(it));
        if (table->item(static_cast<int>(it), 0) == nullptr){
            createCellByRow(static_cast<int>(it));
        }
        else {
            updateCellByRow(static_cast<int>(it));
        }
    }
}
void TableGPU::updateCellByRow(int row){
    vector <string> data = originalCatalog->getListGPU()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->item(row, 0)->setText(QString::fromStdString(data[0]));
    table->item(row, 1)->setText(QString::fromStdString(data[1]));
    table->item(row, 2)->setText(QString::fromStdString(data[2]));
    table->item(row, 3)->setText(QString::fromStdString(data[3]));
    table->item(row, 4)->setText(QString::fromStdString(data[4]));
}
void TableGPU::createCellByRow(int row){
    vector <string> data = originalCatalog->getListGPU()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(data[0])));
    table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(data[1])));
    table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(data[2])));
    table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(data[3])));
    table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(data[4])));
}
void TableGPU::defEditorContent(){
    QHBoxLayout *codeEdit = new QHBoxLayout();
    QLabel *codeTitle = new QLabel("Код:");
    codeValue = new QLineEdit();
    codeEdit->addWidget(codeTitle);
    codeEdit->addWidget(codeValue);

    QLabel *nameTitle = new QLabel("Название:");
    nameValue = new QLineEdit();
    codeEdit->addWidget(nameTitle);
    codeEdit->addWidget(nameValue);

    QLabel *nupTitle = new QLabel("NUP:");
    nupValue = new QLineEdit();
    codeEdit->addWidget(nupTitle);
    codeEdit->addWidget(nupValue);

    QLabel *freqTitle = new QLabel("Частота:");
    freqValue = new QLineEdit();
    codeEdit->addWidget(freqTitle);
    codeEdit->addWidget(freqValue);

    QLabel *mrerTitle = new QLabel("Производитель:");
    mrerValue = new QLineEdit();
    codeEdit->addWidget(mrerTitle);
    codeEdit->addWidget(mrerValue);

    this->editorLayout->addLayout(codeEdit);
}
void TableGPU::defEditorConnections(){
    connect(codeValue, &QLineEdit::returnPressed, this, [=](){
        GPU *tmp = originalCatalog->getListGPU()->at(currentCellRow);
        if (tmp != nullptr){
            if (!originalCatalog->setGPUcode(tmp, codeValue->text().toInt())){
                updateEditorContent(currentCellRow);
            }
            else {
                emit needUpdateCells();
            }
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(nameValue, &QLineEdit::returnPressed, this, [=](){
        GPU *tmp = originalCatalog->getListGPU()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setName(nameValue->text().toStdString());
            originalCatalog->checkGCPtrsVars();
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(nupValue, &QLineEdit::returnPressed, this, [=](){
        GPU *tmp = originalCatalog->getListGPU()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setNUP(nupValue->text().toInt());
            updateCellByRow(currentCellRow);
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(freqValue, &QLineEdit::returnPressed, this, [=](){
        GPU *tmp = originalCatalog->getListGPU()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setFreq(freqValue->text().toInt());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(mrerValue, &QLineEdit::returnPressed, this, [=](){
        GPU *tmp = originalCatalog->getListGPU()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setMRER(mrerValue->text().toStdString());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });
}
void TableGPU::updateEditorContent(int row){
    vector <string> data = originalCatalog->getListGPU()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    codeValue->setText(QString::fromStdString(data[0]));
    nameValue->setText(QString::fromStdString(data[1]));
    nupValue->setText(QString::fromStdString(data[2]));
    freqValue->setText(QString::fromStdString(data[3]));
    mrerValue->setText(QString::fromStdString(data[4]));
}


TableMMR::TableMMR(Catalog *originalCatalog) : TableBase (){
    const QStringList headers = {"Код", "Объем (гб)", "Пропуск. способ", "Частота", "Тип"};
    this->table->setColumnCount(5);
    this->table->setHorizontalHeaderLabels(headers);

    this->originalCatalog = originalCatalog;
    updateAllCells();

    defEditorContent();
    defEditorConnections();

    connect(pushAdd, &QPushButton::clicked, this, [=](){
        if (originalCatalog->addNewMMR()){
            this->table->insertRow(table->rowCount());
            this->createCellByRow(table->rowCount()-1);
            currentCellRow = table->rowCount()-1;
            updateEditorContent(table->rowCount()-1);
        }
    });
    connect(pushDel, &QPushButton::clicked, this, [=](){
        int codeMMR = table->item(currentCellRow, 0)->text().toInt();
        if (originalCatalog->deleteMMR(codeMMR)){
            this->table->removeRow(currentCellRow);
            if (table->rowCount() != 0){
                if (currentCellRow-1 != -1){
                    currentCellRow--;
                }
                cout << "Current row:" << currentCellRow << endl;
            }
            else {
                blockDelButton();
            }
        }
    });
    connect(table, &QTableWidget::cellPressed, this, &TableMMR::updateEditorContent);
}
TableMMR::~TableMMR(){
    delete codeValue;
    delete memoryValue;
    delete bandWValue;
    delete freqValue;
    delete typeValue;
}
void TableMMR::updateAllCells(){
    clearTable();
    for (size_t it = 0; it < originalCatalog->getListMMR()->getList()->size(); it++){
        this->table->insertRow(static_cast<int>(it));
        if (table->item(static_cast<int>(it), 0) == nullptr){
            createCellByRow(static_cast<int>(it));
        }
        else {
            updateCellByRow(static_cast<int>(it));
        }
    }
}
void TableMMR::updateCellByRow(int row){
    vector <string> data = originalCatalog->getListMMR()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->item(row, 0)->setText(QString::fromStdString(data[0]));
    table->item(row, 1)->setText(QString::fromStdString(data[1]));
    table->item(row, 2)->setText(QString::fromStdString(data[2]));
    table->item(row, 3)->setText(QString::fromStdString(data[3]));
    table->item(row, 4)->setText(QString::fromStdString(data[4]));
}
void TableMMR::createCellByRow(int row){
    vector <string> data = originalCatalog->getListMMR()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(data[0])));
    table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(data[1])));
    table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(data[2])));
    table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(data[3])));
    table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(data[4])));
}
void TableMMR::defEditorContent(){
    QHBoxLayout *codeEdit = new QHBoxLayout();
    QLabel *codeTitle = new QLabel("Код:");
    codeValue = new QLineEdit();
    codeEdit->addWidget(codeTitle);
    codeEdit->addWidget(codeValue);

    QLabel *memoryTitle = new QLabel("Объем (гб):");
    memoryValue = new QLineEdit();
    codeEdit->addWidget(memoryTitle);
    codeEdit->addWidget(memoryValue);

    QLabel *bandWTitle = new QLabel("Пропуск. способ.:");
    bandWValue = new QLineEdit();
    codeEdit->addWidget(bandWTitle);
    codeEdit->addWidget(bandWValue);

    QLabel *freqTitle = new QLabel("Частота:");
    freqValue = new QLineEdit();
    codeEdit->addWidget(freqTitle);
    codeEdit->addWidget(freqValue);

    QLabel *typeTitle = new QLabel("Производитель:");
    typeValue = new QLineEdit();
    codeEdit->addWidget(typeTitle);
    codeEdit->addWidget(typeValue);

    this->editorLayout->addLayout(codeEdit);
}
void TableMMR::defEditorConnections(){
    connect(codeValue, &QLineEdit::returnPressed, this, [=](){
        MMR *tmp = originalCatalog->getListMMR()->at(currentCellRow);
        if (tmp != nullptr){
            if (!originalCatalog->setMMRcode(tmp, codeValue->text().toInt())){
                updateEditorContent(currentCellRow);
            }
            else {
                emit needUpdateCells();
            }
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(memoryValue, &QLineEdit::returnPressed, this, [=](){
        MMR *tmp = originalCatalog->getListMMR()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setMemory(memoryValue->text().toUShort());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(bandWValue, &QLineEdit::returnPressed, this, [=](){
        MMR *tmp = originalCatalog->getListMMR()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setBandW(bandWValue->text().toDouble());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(freqValue, &QLineEdit::returnPressed, this, [=](){
        MMR *tmp = originalCatalog->getListMMR()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setFreq(freqValue->text().toInt());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(typeValue, &QLineEdit::returnPressed, this, [=](){
        MMR *tmp = originalCatalog->getListMMR()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setType(typeValue->text().toStdString());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });
}
void TableMMR::updateEditorContent(int row){
    vector <string> data = originalCatalog->getListMMR()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    codeValue->setText(QString::fromStdString(data[0]));
    memoryValue->setText(QString::fromStdString(data[1]));
    bandWValue->setText(QString::fromStdString(data[2]));
    freqValue->setText(QString::fromStdString(data[3]));
    typeValue->setText(QString::fromStdString(data[4]));
}


TableMRER::TableMRER(Catalog *originalCatalog) : TableBase (){
    const QStringList headers = {"Код", "Название", "Год основания", "Сайт"};
    this->table->setColumnCount(4);
    this->table->setHorizontalHeaderLabels(headers);

    this->originalCatalog = originalCatalog;
    updateAllCells();

    defEditorContent();
    defEditorConnections();

    connect(pushAdd, &QPushButton::clicked, this, [=](){
        if (originalCatalog->addNewMRER()){
            this->table->insertRow(table->rowCount());
            this->createCellByRow(table->rowCount()-1);
            currentCellRow = table->rowCount()-1;
            updateEditorContent(table->rowCount()-1);
        }
    });
    connect(pushDel, &QPushButton::clicked, this, [=](){
        int codeMRER = table->item(currentCellRow, 0)->text().toInt();
        if (originalCatalog->deleteMRER(codeMRER)){
            this->table->removeRow(currentCellRow);
            if (table->rowCount() != 0){
                if (currentCellRow-1 != -1){
                    currentCellRow--;
                }
                cout << "Current row:" << currentCellRow << endl;
            }
            else {
                blockDelButton();
            }
        }
    });
    connect(table, &QTableWidget::cellPressed, this, &TableMRER::updateEditorContent);
}
TableMRER::~TableMRER(){
    delete codeValue;
    delete nameValue;
    delete fYearValue;
    delete siteValue;
}
void TableMRER::updateAllCells(){
    clearTable();
    for (size_t it = 0; it < originalCatalog->getListMRER()->getList()->size(); it++){
        this->table->insertRow(static_cast<int>(it));
        if (table->item(static_cast<int>(it), 0) == nullptr){
            createCellByRow(static_cast<int>(it));
        }
        else {
            updateCellByRow(static_cast<int>(it));
        }
    }
}
void TableMRER::updateCellByRow(int row){
    vector <string> data = originalCatalog->getListMRER()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->item(row, 0)->setText(QString::fromStdString(data[0]));
    table->item(row, 1)->setText(QString::fromStdString(data[1]));
    table->item(row, 2)->setText(QString::fromStdString(data[2]));
    table->item(row, 3)->setText(QString::fromStdString(data[3]));
}
void TableMRER::createCellByRow(int row){
    vector <string> data = originalCatalog->getListMRER()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(data[0])));
    table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(data[1])));
    table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(data[2])));
    table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(data[3])));
}
void TableMRER::defEditorContent(){
    QHBoxLayout *codeEdit = new QHBoxLayout();
    QLabel *codeTitle = new QLabel("Код:");
    codeValue = new QLineEdit();
    codeEdit->addWidget(codeTitle);
    codeEdit->addWidget(codeValue);

    QLabel *nameTitle = new QLabel("Название:");
    nameValue = new QLineEdit();
    codeEdit->addWidget(nameTitle);
    codeEdit->addWidget(nameValue);

    QLabel *fYearTitle = new QLabel("Год основан.:");
    fYearValue = new QLineEdit();
    codeEdit->addWidget(fYearTitle);
    codeEdit->addWidget(fYearValue);

    QLabel *siteTitle = new QLabel("Сайт:");
    siteValue = new QLineEdit();
    codeEdit->addWidget(siteTitle);
    codeEdit->addWidget(siteValue);

    this->editorLayout->addLayout(codeEdit);
}
void TableMRER::defEditorConnections(){
    connect(codeValue, &QLineEdit::returnPressed, this, [=](){
        MRER *tmp = originalCatalog->getListMRER()->at(currentCellRow);
        if (tmp != nullptr){
            if (!originalCatalog->setMRERcode(tmp, codeValue->text().toInt())){
                updateEditorContent(currentCellRow);
            }
            else {
                emit needUpdateCells();
            }
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(nameValue, &QLineEdit::returnPressed, this, [=](){
        MRER *tmp = originalCatalog->getListMRER()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setName(nameValue->text().toStdString());
            originalCatalog->checkGCPtrsVars();
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(fYearValue, &QLineEdit::returnPressed, this, [=](){
        MRER *tmp = originalCatalog->getListMRER()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setFYear(fYearValue->text().toUShort());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(siteValue, &QLineEdit::returnPressed, this, [=](){
        MRER *tmp = originalCatalog->getListMRER()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setSite(siteValue->text().toStdString());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });
}
void TableMRER::updateEditorContent(int row){
    vector <string> data = originalCatalog->getListMRER()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    codeValue->setText(QString::fromStdString(data[0]));
    nameValue->setText(QString::fromStdString(data[1]));
    fYearValue->setText(QString::fromStdString(data[2]));
    siteValue->setText(QString::fromStdString(data[3]));
}


TableGC::TableGC(Catalog *originalCatalog) : TableBase (){
    const QStringList headers = {"Код", "Название", "Код Производ.", "Код Граф. проц.", "Код памяти"};
    this->table->setColumnCount(5);
    this->table->setHorizontalHeaderLabels(headers);

    defEditorContent();
    defEditorConnections();

    this->originalCatalog = originalCatalog;
    updateAllCells();

    connect(pushAdd, &QPushButton::clicked, this, [=](){
        if (originalCatalog->addNewGC()){
            this->table->insertRow(table->rowCount());
            this->createCellByRow(table->rowCount()-1);
            currentCellRow = table->rowCount()-1;
            updateEditorContent(table->rowCount()-1);
        }
    });
    connect(pushDel, &QPushButton::clicked, this, [=](){
        int codeGC = table->item(currentCellRow, 0)->text().toInt();
        if (originalCatalog->deleteGC(codeGC)){
            this->table->removeRow(currentCellRow);
            if (table->rowCount() != 0){
                if (currentCellRow-1 != -1){
                    currentCellRow--;
                }
                cout << "Current row:" << currentCellRow << endl;
            }
            else {
                blockDelButton();
            }
        }
    });
    connect(table, &QTableWidget::cellPressed, this, &TableGC::updateEditorContent);
}
TableGC::~TableGC(){
    delete codeValue;
    delete nameValue;
    delete mrerValue;
    delete gpuValue;
    delete mmrValue;
}
void TableGC::updateAllCells(){
    clearTable();
    for (size_t it = 0; it < originalCatalog->getListGC()->getList()->size(); it++){
        this->table->insertRow(static_cast<int>(it));
        if (table->item(static_cast<int>(it), 0) == nullptr){
            createCellByRow(static_cast<int>(it));
        }
        else {
            updateCellByRow(static_cast<int>(it));
        }
    }
    updateComboBoxes();
}
void TableGC::updateCellByRow(int row){
    vector <string> data = originalCatalog->getListGC()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->item(row, 0)->setText(QString::fromStdString(data[0]));
    table->item(row, 1)->setText(QString::fromStdString(data[1]));
    table->item(row, 2)->setText(QString::fromStdString(data[2]));
    table->item(row, 3)->setText(QString::fromStdString(data[3]));
    table->item(row, 4)->setText(QString::fromStdString(data[4]));
}
void TableGC::createCellByRow(int row){
    vector <string> data = originalCatalog->getListGC()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    table->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(data[0])));
    table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(data[1])));
    table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(data[2])));
    table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(data[3])));
    table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(data[4])));
}
void TableGC::defEditorContent(){
    QHBoxLayout *codeEdit = new QHBoxLayout();
    QLabel *codeTitle = new QLabel("Код:");
    codeValue = new QLineEdit();
    codeEdit->addWidget(codeTitle);
    codeEdit->addWidget(codeValue);

    QLabel *nameTitle = new QLabel("Название:");
    nameValue = new QLineEdit();
    codeEdit->addWidget(nameTitle);
    codeEdit->addWidget(nameValue);

    QLabel *mrerTitle = new QLabel("Код производ.:");
    mrerValue = new QComboBox();
    codeEdit->addWidget(mrerTitle);
    codeEdit->addWidget(mrerValue);

    QLabel *gpuTitle = new QLabel("Код граф. проц.:");
    gpuValue = new QComboBox();
    codeEdit->addWidget(gpuTitle);
    codeEdit->addWidget(gpuValue);

    QLabel *mmrTitle = new QLabel("Код памяти:");
    mmrValue = new QComboBox();
    codeEdit->addWidget(mmrTitle);
    codeEdit->addWidget(mmrValue);

    this->editorLayout->addLayout(codeEdit);
}
void TableGC::defEditorConnections(){
    connect(codeValue, &QLineEdit::returnPressed, this, [=](){
        GCard *tmp = originalCatalog->getListGC()->at(currentCellRow);
        if (tmp != nullptr){
            if (!originalCatalog->setGCcode(tmp, codeValue->text().toInt())){
                updateEditorContent(currentCellRow);
            }
            else {
                emit needUpdateCells();
            }
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(nameValue, &QLineEdit::returnPressed, this, [=](){
        GCard *tmp = originalCatalog->getListGC()->at(currentCellRow);
        if (tmp != nullptr){
            tmp->setName(nameValue->text().toStdString());
            emit needUpdateCells();
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(mrerValue, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, [=](const QString &value){
        GCard *tmp = originalCatalog->getListGC()->at(currentCellRow);
        if (tmp != nullptr){
            if (!originalCatalog->setGCcodeMRER(tmp, value.toInt())){
                updateEditorContent(currentCellRow);
            }
            else {
                emit needUpdateCells();
            }
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(gpuValue, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, [=](const QString &value){
        GCard *tmp = originalCatalog->getListGC()->at(currentCellRow);
        if (tmp != nullptr){
            if (!originalCatalog->setGCcodeGPU(tmp, value.toInt())){
                updateEditorContent(currentCellRow);
            }
            else {
                emit needUpdateCells();
            }
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });

    connect(mmrValue, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::activated), this, [=](const QString &value){
        GCard *tmp = originalCatalog->getListGC()->at(currentCellRow);
        if (tmp != nullptr){
            if (!originalCatalog->setGCcodeMMR(tmp, value.toInt())){
                updateEditorContent(currentCellRow);
            }
            else {
                emit needUpdateCells();
            }
        }
        else {
            cout << " <> Ошибка: Функция вернула nullptr" << endl;
        }
    });
}
void TableGC::updateEditorContent(int row){
    vector <string> data = originalCatalog->getListGC()->getList()->at(static_cast<size_t>(row))->getVarsVector();
    codeValue->setText(QString::fromStdString(data[0]));
    nameValue->setText(QString::fromStdString(data[5]));
//    mrerValue->setText(QString::fromStdString(data[2]));
//    gpuValue->setText(QString::fromStdString(data[3]));
//    mmrValue->setText(QString::fromStdString(data[4]));
}
void TableGC::updateComboBoxes(){
    mrerValue->clear();
    gpuValue->clear();
    mmrValue->clear();
    for (MRER *it : *originalCatalog->getListMRER()->getList()){
        mrerValue->addItem(QString::number(*it->getCode()));
    }
    for (GPU *it : *originalCatalog->getListGPU()->getList()){
        gpuValue->addItem(QString::number(*it->getCode()));
    }
    for (MMR *it : *originalCatalog->getListMMR()->getList()){
        mmrValue->addItem(QString::number(*it->getCode()));
    }
}


Tabs::Tabs(QTabWidget *parent) : QTabWidget(parent){
    originalCatalog = new Catalog();
    editableCatalog = new Catalog(originalCatalog);
    tableGPU = new TableGPU(editableCatalog);
    tableMRER = new TableMRER(editableCatalog);
    tableMMR = new TableMMR(editableCatalog);
    tableGC = new TableGC(editableCatalog);

    connect(tableGPU, &TableGPU::needUndo, this, &Tabs::undoAll);
    connect(tableGPU, &TableGPU::needRefresh, this, &Tabs::refreshAll);
    connect(tableGPU, &TableGPU::needUpdateCells, this, &Tabs::updateAllCells);

    connect(tableMRER, &TableMRER::needUndo, this, &Tabs::undoAll);
    connect(tableMRER, &TableMRER::needRefresh, this, &Tabs::refreshAll);
    connect(tableMRER, &TableMRER::needUpdateCells, this, &Tabs::updateAllCells);

    connect(tableMMR, &TableMMR::needUndo, this, &Tabs::undoAll);
    connect(tableMMR, &TableMMR::needRefresh, this, &Tabs::refreshAll);
    connect(tableMMR, &TableMMR::needUpdateCells, this, &Tabs::updateAllCells);

    connect(tableGC, &TableGC::needUndo, this, &Tabs::undoAll);
    connect(tableGC, &TableGC::needRefresh, this, &Tabs::refreshAll);
    connect(tableGC, &TableGC::needUpdateCells, this, &Tabs::updateAllCells);

    this->addTab(tableGPU, QString::fromStdString(originalCatalog->getListGPU()->getObjName()));
    this->addTab(tableMRER, QString::fromStdString(originalCatalog->getListMRER()->getObjName()));
    this->addTab(tableMMR, QString::fromStdString(originalCatalog->getListMMR()->getObjName()));
    this->addTab(tableGC, QString::fromStdString(originalCatalog->getListGC()->getObjName()));
}
Tabs::~Tabs(){
    delete originalCatalog;
    delete editableCatalog;
    delete tableGPU;
    delete tableMRER;
    delete tableMMR;
    delete tableGC;
}
void Tabs::updateEditableCatalog(){
    editableCatalog->copy(originalCatalog);
}
void Tabs::updateAllCells(){
    tableGPU->updateAllCells();
    tableMRER->updateAllCells();
    tableMMR->updateAllCells();
    tableGC->updateAllCells();
}
void Tabs::refreshAll(){
    originalCatalog->clearAll();
    for (QString it : xmlFiles){
        originalCatalog->readFromXML(it.toStdString());
    }
    for (QString it : sqlFiles){
        originalCatalog->readFromXML(it.toStdString());
    }
    updateEditableCatalog();
    //Далее перезагрузка таблиц
    updateAllCells();
}
void Tabs::undoAll(){
    updateEditableCatalog();
    updateAllCells();
}
void Tabs::clearAll(){
    xmlFiles.clear();
    sqlFiles.clear();
    originalCatalog->clearAll();
    updateEditableCatalog();
    //Далее очистка таблиц
    updateAllCells();
}
void Tabs::blockAllDelButtons(){
    //Далее блокировка кнопок у таблиц
    tableGPU->blockDelButton();
    tableMRER->blockDelButton();
    tableMMR->blockDelButton();
    tableGC->blockDelButton();
}
void Tabs::readFromFileXML(QString filepath){
    xmlFiles.push_back(filepath);
    originalCatalog->readFromXML(filepath.toStdString());
    updateEditableCatalog();
    //Далее перезагрузка таблиц
    updateAllCells();
}
void Tabs::readFromFileDB(QString filepath){
    sqlFiles.push_back(filepath);
    originalCatalog->readFromSQL(filepath.toStdString());
    updateEditableCatalog();
    //Далее перезагрузка таблиц
    updateAllCells();
}
void Tabs::writeToFileXML(QString filepath){
    editableCatalog->writeToXML(filepath.toStdString());
}
void Tabs::writeToFileDB(QString filepath){
    editableCatalog->writeToSQL(filepath.toStdString());
}


AppGui::AppGui(QString appName, QMainWindow *parent) : QMainWindow(parent){
    setAppName(appName);

    this->tabsWidget = new Tabs();
    setCentralWidget(tabsWidget);

    QAction *loadInfo = new QAction("Открыть инфо.", this);
    QAction *saveInfo = new QAction("Сохранить инфо.", this);
    QAction *closeInfo = new QAction("Закрыть", this);
    QAction *quit = new QAction("Выход", this);

    connect(loadInfo, &QAction::triggered, this, [=](){
        QString path = QFileDialog::getOpenFileName(this,
                        "Открой файл с инфомацией", "", "Файл информации (*.db *.xml)");
        if (path != ""){
            if (path.contains(".xml")){
                tabsWidget->readFromFileXML(path);
            }
            else {
                tabsWidget->readFromFileDB(path);
            }
        }
    });
    connect(saveInfo, &QAction::triggered, this, [=](){
        QString path = QFileDialog::getSaveFileName(this,
                        "Открой файл с инфомацией", "", "Файл информации (*.db *.xml)");
        if (path != ""){
            if (path.contains(".xml")){
                tabsWidget->writeToFileXML(path);
            }
            else {
                tabsWidget->writeToFileDB(path);
            }
        }
    });
    connect(closeInfo, &QAction::triggered, this, [=](){
        tabsWidget->clearAll();
        tabsWidget->blockAllDelButtons();
    });
    connect(quit, &QAction::triggered, qApp, QApplication::quit);

    appMenu = menuBar()->addMenu("Файл");
    appMenu->addAction(loadInfo);
    appMenu->addAction(saveInfo);
    appMenu->addAction(closeInfo);
    appMenu->addAction(quit);
}
AppGui::~AppGui(){
}
QString AppGui::getAppName(){
    return appName;
}
void AppGui::setAppName(QString appName){
    this->appName = appName;
    this->setWindowTitle(appName);
}
