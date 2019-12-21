#ifndef PLAYERTHREAD_H
#define PLAYERTHREAD_H

#include <QObject>
#include "public.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaControl>
#include <QMediaContent>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <QTimer>

class playerThread : public QObject
{
        Q_OBJECT
    public:
        explicit playerThread(QObject *parent = nullptr);

    signals:
    void changedStep(int);

    private:
        void initd();
        QStringList getFileNames(const QString &path);

    public slots:
       void playerMusic(playerStatus);
       void setFilesPath(QString musicSource);
       void playerUrl(QString );
       void setStep(int);
       void startWork();
       void stopWork();
       void playNext();
       void playLast();

    private slots:
       void mTimeOut();

    private:
      playerStatus pStatus;
      QMediaPlaylist *mPlayerList;
      QMediaPlayer   *mPlayer;

      bool          bStop;
      QString       musicPath;
      QTimer        *mStepTimer;
      int           curPlayPos;
};

#endif // PLAYERTHREAD_H
