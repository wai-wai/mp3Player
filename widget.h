#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>
#include <QMetaType>
#include "public.h"

#include <QDir>
#include <QStringList>

#include <QListWidget>
#include <QListView>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QThread>

#include <QSlider>
#include "playerthread.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class playerThread;
class Widget : public QWidget
{
        Q_OBJECT
    public:
        Widget(QWidget *parent = nullptr);
        ~Widget();


    signals:
        void playerMusic(playerStatus);
        void setPlayPath(QString);
        void playerUrl(QString );
        void setStep(int);
        void startWork();
        void stopWork();
        void playNext();
        void playLast();

    private:
        void initd();
        void readFileList();
        QStringList  getFileNames(const QString &path);

    private slots:
        void on_browseBtn_clicked();

        void on_playerBtn_clicked();

        void on_comboBox_currentIndexChanged(int index);
        void itemDoubleClicked(QListWidgetItem *item);
        void changedStep(int);

        void on_pauseBtn_clicked();

        void on_lastBtn_clicked();

        void on_nextBtn_clicked();
        void timeOut();

        void on_stepSlider_sliderMoved(int position);

        void on_stepSlider_sliderReleased();

    private:
        Ui::Widget *ui;
        QString  currentMusicPath;
        QListWidget     *musicListW;
        QThread         mPlayThread;
        playerThread    *mWorkPlay;
        bool            mPlayStatus;
        bool            bPauseStatus;
        int             stepMovePos;
        int             playPostion;
        QTimer          *mStimer;

};

Q_DECLARE_METATYPE(playerStatus);
Q_DECLARE_METATYPE(qint64);
#endif // WIDGET_H
