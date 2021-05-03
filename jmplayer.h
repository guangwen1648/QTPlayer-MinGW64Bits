#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QLabel>
#include<QFileDialog>
#include "FFmpegManager/ffmpegmanager.h"
#include "SDLManager/sdlmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class JMPlayer : public QMainWindow
{
    Q_OBJECT

public:
    JMPlayer(QWidget *parent = nullptr);
    ~JMPlayer();

private slots:
            void onButtonChange(bool flag);

            void onVideoPlay();

            void on_pauseButton_2_clicked();

            void on_stopButton_2_clicked();

            void on_quitButton_2_clicked();

            void on_actionOpenFile_triggered();

            void on_actionPause_triggered();

            void on_actionReplay_triggered();

            void on_actionStop_triggered();

            void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;

    FFmpegManager ffmMgr;

    SDLManager sdlMgr;

    QRect rect;

    QString currentFilePath;

    bool isPlaying;

    int point_x;
    int point_y;

    int labelHeight;
    int labelWidth;
};
#endif // MAINWINDOW_H
