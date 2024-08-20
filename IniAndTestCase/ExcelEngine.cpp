#include "ExcelEngine.h"

ExcelEngine::ExcelEngine(void):
    m_IsOpened(false),
    m_Excel(nullptr),
    m_WorkBooks(nullptr),
    m_WorkBook(nullptr),
    m_WorkSheets(nullptr),
    m_WorkSheetCount(0),
    m_FileName()
{
    // 打开当前进程的COM并释放相关资源
    OleInitialize(0);
}

ExcelEngine::ExcelEngine(const QString file_Name):
    m_IsOpened(false),
    m_Excel(nullptr),
    m_WorkBooks(nullptr),
    m_WorkBook(nullptr),
    m_WorkSheets(nullptr),
    m_WorkSheetCount(0),
    m_FileName(file_Name)
{
    // 打开当前进程的COM并释放相关资源
    OleInitialize(0);
}

ExcelEngine::~ExcelEngine(void)
{
    release();
    // 关闭当前进程的COM并释放相关资源
    OleUninitialize();
}
void ExcelEngine::QstringtoU16ptr(QString Qstr,void* number,uint8_t index)
{
    QByteArray ARY = Qstr.toLatin1(); // must
    char *str=ARY.data();
    *(uint16_t *)number=strtol(str,nullptr,index);
}
void ExcelEngine::longstrtol(char* String, uint8_t Res[], long DataSize)
{
    /*  if (datasize%2==1) {
          return;
      }*/
    char String_strol[1000];

    memset(String_strol, 0, 1000);
    memcpy(String_strol, String, DataSize * 2);
    char byte[3] = { 0 };
    //char* p = String_strol;
    char* endptr = nullptr;

    for (int i = 0, k = 0; i < DataSize * 2; i += 2, k += 1) {

        memcpy(byte, &String_strol[i], 2);
        Res[k] = (uint8_t)strtol(byte, &endptr, 16);
    }
}
// 打开Excel，如果不存在则创建
bool ExcelEngine::open(bool visible ,bool display_Alerts )
{
    // 检测文件名是否为空
    if (m_FileName.isEmpty())
    {
        // 返回异常（文件名不存在）
        return false;
    }

    QFile file(m_FileName);

    if (file.exists())
    {
        // 文件存在，但没有以只读状态打开成功
        if (!file.open(QIODevice::ReadOnly))
        {
            // 返回异常（文件存在但是未能以只读状态打开）
            return false;
        }

        file.close();
    }
    else
    {
        // 返回异常（文件不存在）
        return false;
    }

    // Excel的程序对象指针
    m_Excel = new QAxObject;

    // 链接Excel控件（用Office的Excel打开Excel文档）
    bool isOpened = m_Excel->setControl("Excel.Application");

    if (!isOpened)
    {
        // 用WPS打开Excel文件
        isOpened = m_Excel->setControl("ket.Application");

        // 如果Office和WPS都无法打开则返回false
        if (!isOpened)
        {
            // 返回异常（Excel和WPS打开excel文件均未成功）
            return false;
        }
    }

    // 不显示窗体
    m_Excel->dynamicCall("SetVisible(bool)", visible);

    // 不显示警告
    m_Excel->setProperty("DisplayAlerts", display_Alerts);

    // 获取Excel工作簿集合对象
    m_WorkBooks = m_Excel->querySubObject("WorkBooks");

    // 打开Excel文件
    m_WorkBooks->dynamicCall("Open(const QString &)",m_FileName);

    // 当前活动工作簿
    m_WorkBook = m_Excel->querySubObject("ActiveWorkBook");

    if (m_WorkBook == nullptr)
    {
        // 返回异常（当前活动工作簿打开失败）
        return false;
    }

    // 工作表集
    m_WorkSheets = m_WorkBook->querySubObject("WorkSheets");

    // 工作表的个数
    m_WorkSheetCount = m_WorkSheets->dynamicCall("Count").toInt();


    if(m_WorkSheets == nullptr)
    {
        // 返回异常（当前表格集WorkSheets不存在，打开失败!）
        return false;
    }

    m_IsOpened = true;
    return m_IsOpened;
}

// 设置窗体是否可见
bool ExcelEngine::setVisible(bool visible )
{
    if (m_Excel == nullptr)
    {
        return false;
    }
    else
    {
        m_Excel->dynamicCall("SetVisible(bool)", visible);
        return true;
    }
}

// 获取索引出的sheet（第一个就是1，不是0）
QAxObject *ExcelEngine::getWorkSheet(int sheet_Index)
{
    QAxObject *workSheet = m_WorkBook->querySubObject("Worksheets(int)",sheet_Index);
    return workSheet;
}

// 通过SheetName获取sheet
QAxObject *ExcelEngine::getWorkSheet(QString sheet_Name)
{
    QAxObject *workSheet = m_WorkBook->querySubObject("Worksheets(QString)",sheet_Name);
    return workSheet;
}

// 获取单元格内容
QString ExcelEngine::getCellString(int workSheet_Index,int row,int column)
{
    QAxObject *workSheet = getWorkSheet(workSheet_Index);

    if(workSheet == nullptr)
    {
        return "";
    }

    QAxObject *range = workSheet->querySubObject("Cell(int,int)",row,column);

    if(range == nullptr)
    {
        return "";
    }

    QString cellTdext = range->dynamicCall("Value2()").toString();

    return cellTdext;
}

// 获取单元格内容
QString ExcelEngine::getCellString(QString workSheet_Name,int row,int column)
{
    QAxObject *workSheet = getWorkSheet(workSheet_Name);

    if(workSheet == nullptr)
    {
        return "";
    }

    QAxObject *range = workSheet->querySubObject("Cells(int,int)",row,column);

    if(range == nullptr)
    {
        return "";
    }

    QString cellTdext = range->dynamicCall("Value2()").toString();

    return cellTdext;
}

// 获取workSheet的行数
int ExcelEngine::getWorkSheetRows(QString workSheet_Name)
{
    QAxObject *workSheet = getWorkSheet(workSheet_Name);

    if(workSheet == nullptr)
    {
        return 0;
    }

    // 获取权限
    QAxObject *usedrange = workSheet->querySubObject("Usedrange");
    int rows = usedrange->querySubObject("Rows")->property("Count").toInt();
    return rows;
}

// 获取workSheet的行数
int ExcelEngine::getWorkSheetRows(int  workSheet_Index)
{
    QAxObject *workSheet = getWorkSheet(workSheet_Index);

    if(workSheet == nullptr)
    {
        return 0;
    }

    // 获取workSheet表中的有效区域
    QAxObject *usedrange = workSheet->querySubObject("Usedrange");
    int rows = usedrange->querySubObject("Rows")->property("Count").toInt();
    return rows;
}

// 获取workSheet的列数
int ExcelEngine::getWorkSheetColumns(QString workSheet_Name)
{
    QAxObject *workSheet = getWorkSheet(workSheet_Name);

    if(workSheet == nullptr)
    {
        return 0;
    }

    // 获取workSheet表中的有效区域
    QAxObject *usedrange = workSheet->querySubObject("Usedrange");
    int columns = usedrange->querySubObject("Columns")->property("Count").toInt();
    return columns;
}

// 获取workSheet的列数
int ExcelEngine::getWorkSheetColumns(int workSheet_Index)
{
    QAxObject *workSheet = getWorkSheet(workSheet_Index);

    if(workSheet == nullptr)
    {
        return 0;
    }

    // 获取workSheet表中的有效区域
    QAxObject *usedrange = workSheet->querySubObject("Usedrange");
    int columns = usedrange->querySubObject("Columns")->property("Count").toInt();
    return columns;
}


// 通过索引值获取工作表（workSheet）的表名
QString ExcelEngine::getWorkSheetNameByIndex(int workSheet_Inedx)
{
    QAxObject *workSheet = getWorkSheet(workSheet_Inedx);

    if(workSheet == nullptr)
    {
        return "";
    }

    QString sheetName = workSheet->dynamicCall("Name").toString();
    return sheetName;
}

// 获取WorkSheets工作表的表名列表
QStringList ExcelEngine::getWorkSheetNameList(void)
{
    QStringList workSheetsList;

    for(int i = 1;i<=m_WorkSheetCount;i++)
    {
        QAxObject *workSheet = getWorkSheet(i);
        QString sheetName = workSheet->dynamicCall("Name").toString();

        workSheetsList.append(sheetName);
    }

    return workSheetsList;
}

// 保存（用于写操作）
bool ExcelEngine::save(void)
{
    if(!m_IsOpened)
    {
        return false;
    }

    QFile file(m_FileName);

    if(file.exists())
    {
        m_WorkBook->dynamicCall("Save()");
    }else
    {
        return false;
    }

    return true;
}

// 关闭资源（释放资源）
void ExcelEngine::close(void)
{
    release();
}

// 释放资源
void ExcelEngine::release(void)
{
    m_IsOpened = false;

    if(m_WorkBook != nullptr)
    {
        m_WorkBook->dynamicCall("Close(QVariant)",0);
        delete m_WorkBook;
        m_WorkBook = nullptr;
    }

    if(m_WorkBooks != nullptr)
    {
        m_WorkBooks->dynamicCall("Close(QVariant)",0); // 报错（QAxBase: Error calling IDispatch member Close: Bad parameter count）
        delete m_WorkBooks;
        m_WorkBooks = nullptr;
    }

    if(m_Excel != nullptr)
    {
        m_Excel->setProperty("DisplayAlerts", false);
        m_Excel->dynamicCall("Quit()");
        delete m_Excel;
        m_Excel = nullptr;
    }
}

