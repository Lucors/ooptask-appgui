#ifndef GUI_H
#define GUI_H

//QT
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QTabWidget>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMenuBar>
#include <QTableWidget>
#include <QComboBox>
#include <QDebug>
//Библиотека каталога
#include "bones.h"

using namespace std;

class AppGui;

//TableBase основа каждой таблицы данных
//Добавьте editorLayout в mVLayout в своей таблицы
//Добавте действия для pushAdd и pushDel в своей таблице
class TableBase : public QWidget {
    Q_OBJECT
protected:
    //Указатель на главный каталог
    Catalog *originalCatalog;

    QTableWidget *table;
    QPushButton *pushAdd, *pushDel, *pushUndo, *pushRefr;
    QVBoxLayout *mVLayout, *buttsLayout;
    QHBoxLayout *mHLayout, *editorLayout;

    int currentCellRow = -1;
public:
    TableBase(QWidget* = nullptr);
    virtual ~TableBase();
public slots:
    virtual void updateAllCells() = 0;
    virtual void updateCellByRow(int row) = 0;
    void blockDelButton();
    void clearTable();
    void updateCatalog(Catalog*);
    virtual void defEditorContent() = 0;
    virtual void defEditorConnections() = 0;
    virtual void updateEditorContent(int) = 0;
signals:
    void needRefresh();
    void needUndo();
    void needUpdateCells();
};

class TableGPU : public TableBase {
    Q_OBJECT
protected:
    QLineEdit *codeValue;
    QLineEdit *nameValue;
    QLineEdit *nupValue;
    QLineEdit *freqValue;
    QLineEdit *mrerValue;
public:
    TableGPU(Catalog*);
    ~TableGPU();
public slots:
    void updateAllCells();
    void updateCellByRow(int);
    void createCellByRow(int);
    void defEditorContent();
    void defEditorConnections();
    void updateEditorContent(int);
};

class TableMMR : public TableBase {
    Q_OBJECT
protected:
    QLineEdit *codeValue;
    QLineEdit *memoryValue;
    QLineEdit *bandWValue;
    QLineEdit *freqValue;
    QLineEdit *typeValue;
public:
    TableMMR(Catalog*);
    ~TableMMR();
public slots:
    void updateAllCells();
    void updateCellByRow(int);
    void createCellByRow(int);
    void defEditorContent();
    void defEditorConnections();
    void updateEditorContent(int);
};

class TableMRER : public TableBase {
    Q_OBJECT
protected:
    QLineEdit *codeValue;
    QLineEdit *nameValue;
    QLineEdit *fYearValue;
    QLineEdit *siteValue;
public:
    TableMRER(Catalog*);
    ~TableMRER();
public slots:
    void updateAllCells();
    void updateCellByRow(int);
    void createCellByRow(int);
    void defEditorContent();
    void defEditorConnections();
    void updateEditorContent(int);
};

class TableGC : public TableBase {
    Q_OBJECT
protected:
    QLineEdit *codeValue;
    QLineEdit *nameValue;
    QComboBox *mrerValue;
    QComboBox *gpuValue;
    QComboBox *mmrValue;
public:
    TableGC(Catalog*);
    ~TableGC();
public slots:
    void updateAllCells();
    void updateCellByRow(int);
    void createCellByRow(int);
    void defEditorContent();
    void defEditorConnections();
    void updateEditorContent(int);
    void updateComboBoxes();
};


//Виджет с вкладками
class Tabs : public QTabWidget {
    Q_OBJECT
private:
    TableGPU *tableGPU;
    TableMRER *tableMRER;
    TableMMR *tableMMR;
    TableGC *tableGC;
    QStringList xmlFiles, sqlFiles;
    Catalog *originalCatalog;
    Catalog *editableCatalog = nullptr;
public:
    friend AppGui;
    Tabs(QTabWidget* = nullptr);
    virtual ~Tabs();
public slots:
    void updateEditableCatalog();
    void blockAllDelButtons();
    void updateAllCells();
    void refreshAll();
    void undoAll();
    void clearAll();
    void readFromFileXML(QString);
    void readFromFileDB(QString);
    void writeToFileXML(QString);
    void writeToFileDB(QString);
};

class AppGui : public QMainWindow {
private:
    QMenu *appMenu;
    QString appName;
    Tabs *tabsWidget;
public:
    AppGui(QString = tr("appname"),  QMainWindow* = nullptr);
    virtual ~AppGui();
    //appname
    QString getAppName();
    void setAppName(QString);
};

#endif // GUI_H
