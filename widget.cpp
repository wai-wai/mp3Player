#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qRegisterMetaType<playerStatus>("playerStatus");
    qRegisterMetaType<qint64>("qint64");
    initd();
    mWorkPlay = new playerThread();
    /* 设置播放进度 */
    connect(this, &Widget::playerMusic,mWorkPlay, &playerThread::playerMusic );
    /* 更新播放目录 */
    connect(this, &Widget::setPlayPath, mWorkPlay, &playerThread::setFilesPath);
    /* 播放歌曲 */
    connect(this, &Widget::playerUrl,mWorkPlay, &playerThread::playerUrl );
    /* 设置播放进度 */
    connect(this, &Widget::setStep,mWorkPlay, &playerThread::setStep );
    /* 启动播放任务 */
    connect(this, &Widget::startWork,mWorkPlay, &playerThread::startWork );
    /* 停止播放任务 */
    connect(this, &Widget::stopWork,mWorkPlay, &playerThread::stopWork );

    /* 上一曲 */
    connect(this, &Widget::playLast,mWorkPlay, &playerThread::playLast );
    /* 下一曲 */
    connect(this, &Widget::playNext,mWorkPlay, &playerThread::playNext );
    /* 播放进度 */
    connect(mWorkPlay, &playerThread::changedStep ,this, &Widget::changedStep);

    /* 绑定线程 */
    connect(&mPlayThread, &QThread::finished, mWorkPlay, &playerThread::deleteLater);
    //connect(&mPlayThread, &QThread::start, mWorkPlay, &playerThread::startWork);
    mWorkPlay->moveToThread(&mPlayThread);
    mPlayThread.start();
    mPlayStatus = false;
    bPauseStatus = false;
    ui->stepSlider->setRange(0,1000);
    //ui->stepSlider->setEnabled(false);
    mStimer  = new QTimer(this);
    connect(mStimer, &QTimer::timeout, this, &Widget::timeOut);
    mStimer->setInterval(1000);
    ui->comboBox->setCurrentIndex(2);

}

Widget::~Widget()
{
    mPlayThread.quit();
    mPlayThread.wait();
    delete ui;
}

void Widget::initd(){
    musicListW = new QListWidget();
    musicListW->setViewMode(QListView::ListMode);
    /* 绑定双击事件处理 */
    connect(musicListW, &QListWidget::itemDoubleClicked, this,&Widget::itemDoubleClicked);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addWidget(musicListW);
    ui->musicList->setLayout(hLayout);
    musicListW->hide();
}

void Widget::on_browseBtn_clicked()
{
    QString currentMusic =   QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    QString filePath = QFileDialog::getExistingDirectory(this, "选择音乐目录", currentMusic, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!filePath.isEmpty()){
        currentMusicPath =  filePath;
    }else{
        currentMusicPath = currentMusic;
    }
    ui->musicPath->setText(currentMusicPath);
    musicListW->clear();
    readFileList();
    emit setPlayPath(currentMusicPath);
}

QStringList  Widget::getFileNames(const QString &path){
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

void Widget::readFileList(){
    /* 读取文件列表信息 */
    QStringList fileList = getFileNames(currentMusicPath);

    /* 加载到 listWidget  listMode */
    musicListW->addItems(fileList);
    musicListW->show();
}

void Widget::on_playerBtn_clicked()
{
    ui->playerBtn->setEnabled(false);
    ui->stepSlider->setValue(0);
    ui->pauseBtn->setText("暂停");
    emit startWork();
    ui->playerBtn->setEnabled(true);
     mStimer->start();
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    switch(index){
        case 0:
            /* 单曲 */
            emit playerMusic(mOnePlay);
            break;
        case 1:
            /* 单曲循环 */
            emit playerMusic(mOneLoop);
            break;
        case 2:
            /* 顺序 */
            emit playerMusic(mSequential);
            break;
        case 3:
            /* 循环 */
            emit playerMusic(mLoop);
            break;
        case 4:
            /* 随机 */
            emit playerMusic(mRandom);
            break;
        default:
            break;
    }
}

/* 鼠标双击 音乐列表 */
void Widget::itemDoubleClicked(QListWidgetItem *item){
    ui->stepSlider->setValue(0);
    QString musicName = item->text();
    /* player 当前音乐 */
    emit playerUrl(musicName);
     mStimer->start();
}
/* */

void Widget::timeOut(){
    ui->stepSlider->setValue(playPostion);
}

/* 播放进度 */
void Widget::changedStep(int num){
    playPostion = num;
}

void Widget::on_pauseBtn_clicked()
{
    ui->pauseBtn->setEnabled(false);

    if(bPauseStatus == false){
        emit playerMusic(mPause);
        ui->pauseBtn->setText("继续");
        mStimer->stop();
        bPauseStatus = true;
    }else{
        ui->pauseBtn->setText("暂停");
        emit playerMusic(mPlay);
        mStimer->start();
        bPauseStatus = false;
    }
    ui->pauseBtn->setEnabled(true);
}

/* 上一曲 */
void Widget::on_lastBtn_clicked()
{
    bPauseStatus = false;
    ui->stepSlider->setValue(0);
    ui->pauseBtn->setText("暂停");
    emit playLast();
}

/* 下一曲 */
void Widget::on_nextBtn_clicked()
{
    bPauseStatus = false;
    ui->stepSlider->setValue(0);
    ui->pauseBtn->setText("暂停");
    emit playNext();
}

void Widget::on_stepSlider_sliderMoved(int position)
{
    mStimer->stop();
    playPostion = position;
    emit setStep(position);
}

void Widget::on_stepSlider_sliderReleased()
{
    mStimer->start();
}


