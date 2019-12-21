#include "playerthread.h"

playerThread::playerThread(QObject *parent) : QObject(parent)
{
    bStop = false;
    mPlayerList = new QMediaPlaylist(this);
    mPlayer     = new QMediaPlayer(this);
    mStepTimer  = new QTimer(this);
    connect(mStepTimer, &QTimer::timeout, this, &playerThread::mTimeOut);
}

void playerThread::mTimeOut(){
    /* 值0 ~100 */
    if(mPlayer->isAvailable() && QMediaPlayer::PlayingState == mPlayer->state()){
        /* 计算出 播放进去 */
        int step = (int)((mPlayer->position() * 1000 / mPlayer->duration()));
        emit changedStep(step);
    }
}

void playerThread::setStep(int pos){
    if(mPlayer->isAvailable() && QMediaPlayer::PlayingState == mPlayer->state()){
        /* 计算要滑动的进度 */
        qint64 setp =  (qint64) ((mPlayer->duration()/1000) *pos);

        mPlayer->setPosition(setp);
    }
}

void playerThread::initd(){
    /* 读取播放列表 */
    /* 将读取后的列表放入 mediaplayerList */

    /* 初始化播放列表 */
    mPlayerList->clear();
    mPlayer->setPlaylist(mPlayerList);

   QStringList pathList = getFileNames(musicPath);
   QString mUrl;
   for(int i = 0; i< pathList.length();i++ ){
       mUrl.clear();
       mUrl += (musicPath + "/" + pathList.at(i));
       mPlayerList->addMedia(QUrl(mUrl));
   }
   mPlayerList->setCurrentIndex(1);
   mPlayerList->setPlaybackMode(QMediaPlaylist::Sequential);
   mPlayer->setPlaylist(mPlayerList);
}

void playerThread::setFilesPath(QString musicSource){
    /* 停止播放的音乐,更新列表 */
    if(mPlayer->isAvailable()){
        mPlayer->stop();
    }

    musicPath = musicSource;
    initd();
}

void playerThread::playerMusic(playerStatus status){
    pStatus = status;
    /*
     * CurrentItemOnce 单次,
     * CurrentItemInLoop 单曲循环,
     * Sequential  顺序播放,
     * Loop 列表,
     * Random 随机
    */
    QMediaPlaylist::PlaybackMode model;
    switch(pStatus){
        case mOnePlay:
            model = QMediaPlaylist::CurrentItemOnce;
            mPlayerList->setPlaybackMode(model);
            break;
        case mOneLoop:
            model = QMediaPlaylist::CurrentItemInLoop;
             mPlayerList->setPlaybackMode(model);
            break;
        case mSequential:
            model = QMediaPlaylist::Sequential;
             mPlayerList->setPlaybackMode(model);
            break;
        case mLoop:
            model = QMediaPlaylist::Loop;
             mPlayerList->setPlaybackMode(model);
            break;
        case mRandom:
            model = QMediaPlaylist::Random;
             mPlayerList->setPlaybackMode(model);
            break;
        case mPlay:
            if(mPlayer->isAvailable() && QMediaPlayer::PlayingState != mPlayer->state()){
                startWork();
            }

            break;
        case mPause:
            if(mPlayer->isAvailable() && QMediaPlayer::PlayingState == mPlayer->state() ){
                mPlayer->pause();
                mStepTimer->stop();
            }
            break;
        case mStop:
            if(mPlayer->isAvailable() && QMediaPlayer::StoppedState != mPlayer->state()){
                mPlayer->stop();
                mStepTimer->stop();
            }
            break;
    }

}

void playerThread::startWork(){
    mStepTimer->start(500);
    mPlayer->play();
}

void playerThread::stopWork(){
    bStop = true;
    mStepTimer->stop();
    mPlayer->stop();
}

void playerThread::playLast(){
    mPlayer->stop();
    mStepTimer->stop();
    int curIndex = mPlayerList->currentIndex();
    if(--curIndex < 0) curIndex = 0;

    mPlayerList->setCurrentIndex(curIndex);

    startWork();
}

void playerThread::playNext(){
    mPlayer->stop();
    mStepTimer->stop();
    int curIndex = mPlayerList->currentIndex();
    if(++curIndex == mPlayerList->mediaCount()) curIndex = 0;
    mPlayerList->setCurrentIndex(curIndex);
    startWork();
}

void playerThread::playerUrl(QString name){
    mPlayer->stop();
    mStepTimer->stop();
    curPlayPos = mPlayerList->currentIndex();
    /* 暂停列表播放 */
    QString url = musicPath + "/" + name;

    mPlayerList->insertMedia(curPlayPos+1,QUrl(url));
    mPlayerList->setCurrentIndex(curPlayPos+1);
    startWork();
}

QStringList  playerThread::getFileNames(const QString &path){
    QStringList files;
    do{
        files.clear();
        if(path.isEmpty()) break;

        QDir dir(path);
        QStringList nameFilters;

        nameFilters<<"*.mp3"<<"*.wav" ;
        files = dir.entryList(nameFilters,QDir::Files|QDir::Readable, QDir::Name);
    }while(0);

    return files;
}

