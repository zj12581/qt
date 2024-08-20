#ifndef EXCELENGINE_H
#define EXCELENGINE_H
#include <ActiveQt/QAxObject>
#include <Windows.h>
#include <QFile>
#include <QStringList>
#include <QString>

 class ExcelEngine
{
public:
    ExcelEngine(void);
    ExcelEngine(const QString file_Name);
    ~ExcelEngine(void);

    // 打开Excel
    bool open(bool visible = false,bool display_Alerts = false);

    void QstringtoU16ptr(QString Qstr,void* number,uint8_t index);

    void longstrtol(char* String, uint8_t Res[], long DataSize);
    // 通过Index获取sheet工作表
    QAxObject *getWorkSheet(int Sheet_Index);

    // 通过SheetName获取sheet工作表
    QAxObject *getWorkSheet(QString sheet_Name);

    // 通过索引值获取工作表（workSheet）的表明
    QString getWorkSheetNameByIndex(int inedx);

    // 获取WorkSheets工作表名列表
    QStringList getWorkSheetNameList(void);

    // 获取单元格内容
    QString getCellString(int workSheet_Index,int row,int column);

    // 获取单元格内容
    QString getCellString(QString workSheet_Name,int row,int column);

    // 获取workSheet的行数
    int getWorkSheetRows(QString workSheet_Name);

    // 获取workSheet的行数
    int getWorkSheetRows(int  workSheet_Index);

    // 获取workSheet的列数
    int getWorkSheetColumns(QString workSheet_Name);

    // 获取workSheet的列数
    int getWorkSheetColumns(int workSheet_Index);

    // 保存（写操作时需要要保存）
    bool save(void);

    // 关闭资源（释放资源）
    void close(void);

private:
    // 设置窗体是否可见
    bool setVisible(bool visible = false);

    // 释放资源
    void release(void);

private:
    bool m_IsOpened;

    // EXCEL程序指针
    QAxObject *m_Excel;

    // 工作簿集
    QAxObject *m_WorkBooks;

    // 当前活动的工作簿
    QAxObject *m_WorkBook;

    // 当前活动工作簿的工作表集（即所有的sheet表）
    QAxObject *m_WorkSheets;

    // 工作簿（m_WorkBook）中工作表（workSheet）的个数
    int m_WorkSheetCount;

    // 文件名
    QString m_FileName;

};

#endif // EXCELENGINE_H



