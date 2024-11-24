#include "python_enty.h"
// #include <minizip/unzip.h>
#include <QStringList>
#include <QProcess>
#include <qDebug>
#include <qcoreapplication.h>
#include <QFile>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
bool  python_enty::install()
{

    switch (this->type)
    {
    case 36:{
        break;
    }
    case 37:{
        break;
    }
    case 38:{
        break;
    }
    case 39:{
        break;
    }
    case 310:{
        break;
    }
    case 311:{
        break;
    }
    case 312:{
        break;
    }
    default:{
        return false;
    }

    }

    QString appDir = QCoreApplication::applicationDirPath();

    QString zipname = QString("%1/source/py%2.zip").arg(appDir).arg(QString::number(this->type));
    qDebug() << "ZIP File: " << zipname;

    // 构造程序路径（minizip.exe 相对于当前目录）
    QString program = QString("%1/7z.exe").arg(appDir);
    qDebug() << "Program Path: " << program;

    QString patharg = QString("%1%2").arg("-o").arg(this->pypath);
    QStringList arguments;
    arguments << "x" << "-aoa" <<patharg << zipname;
    qDebug() << "Arguments: " << arguments;

    // 创建 QProcess 对象
    QProcess *process = new QProcess( );  // 使用指针来确保在适当时机销毁

    // 设置不显示外部程序的窗口
    process->setProcessChannelMode(QProcess::MergedChannels);

    qDebug() << "Starting QProcess...";

    // 启动进程
    process->start(program, arguments);

    // 等待进程启动并执行完毕
    if (!process->waitForStarted()) {
        qDebug() << "Error: Could not start process.";
        return false;
    }

    // 等待进程完成
    if (!process->waitForFinished()) {
        qDebug() << "Error: Process did not finish correctly.";
        qDebug() << process->errorString();
        return false;
    }

    // 获取标准输出和错误输出
    qDebug() << "Standard Output: " << process->readAllStandardOutput();
    qDebug() << "Standard Error: " << process->readAllStandardError();

    // 输出解压结果
    qDebug() << "Process Finished!";

    // 删除进程对象
    delete process;

    QDir dir(this->pypath);
    if (!dir.exists()) {
        qDebug() << "路径无效：" <<this->pypath;
        return false;
    }

    // 构造 python.exe 文件路径
    QString pythonExePath = dir.filePath("python.exe");

    // 检查是否存在 python.exe 文件
    if (QFile::exists(pythonExePath)) {
        qDebug() << "找到 python.exe：" << pythonExePath;
        return true;
    } else {
        qDebug() << "未找到 python.exe";
        return false;
    }

    return true;
}
bool deleteDirectory(const QString &dirPath) {
    QDir dir(dirPath);
    // 确保目录存在
    if (!dir.exists()) {
        qDebug() << "Directory does not exist.";
        return false;
    }

    // 遍历目录中的所有文件和子目录
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();

    foreach (const QFileInfo &fileInfo, fileList) {
        // 如果是文件，删除文件
        if (fileInfo.isFile()) {
            if (!QFile::remove(fileInfo.absoluteFilePath())) {
                qDebug() << "Failed to remove file:" << fileInfo.absoluteFilePath();
            }
        }
        // 如果是目录，递归删除
        else if (fileInfo.isDir()) {
            if (!deleteDirectory(fileInfo.absoluteFilePath())) {
                qDebug() << "Failed to remove directory:" << fileInfo.absoluteFilePath();
            }
        }
    }
}
void python_enty::uninstall()
{
    deleteDirectory(this->pypath);
}

bool python_enty::regeistToDb()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("INSERT INTO python_entries (type, name, pypath) VALUES (:type, :name, :pypath)");
    query.bindValue(":type", type);
    query.bindValue(":name", name);
    query.bindValue(":pypath", pypath);

    if (!query.exec()) {
        qDebug() << "插入数据失败: " << query.lastError().text();
        return false;
    }
    return true;
}

void python_enty::deletefromdb()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);

    // 修改为删除操作，删除名字为 :name 的记录
    query.prepare("DELETE FROM python_entries WHERE name = :name");
    query.bindValue(":name", name);  // 将 name 绑定到参数 :name

    if (!query.exec()) {
        qDebug() << "删除失败 " << query.lastError().text();
        return;
    }
    qDebug() << "删除成功";
}

python_enty::python_enty() {}
