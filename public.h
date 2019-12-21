#ifndef PUBLIC_H
#define PUBLIC_H

enum playerStatus{
    mPlay = 1,
    mStop,
    mPause,
    mOnePlay/* 播放一次*/,
    mOneLoop/* 单曲循环 */,
    mSequential/* 顺序 */,
    mLoop/* 循环 */,
    mRandom/* 随机 */
};

#endif // PUBLIC_H
