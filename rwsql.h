#ifndef RWSQL_H
#define RWSQL_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QFileInfo>


class rwSql : public QObject
{
        Q_OBJECT
    public:
        explicit rwSql(QString dbFilePath, QObject *parent = nullptr);

        bool  findMd5(QString MD5, QString name);
        void  addMd5(QString md5, QString  name);
        bool  delMusicByName(QString name);
        bool delmusicByMd5(QString md5);
        bool clear();
    signals:

    private:
        QString dbPath;
        QSqlDatabase    dataBase;
};

#endif // RWSQL_H
