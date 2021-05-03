#include "jmplayer.h"
#include "ui_jmplayer.h"

#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

JMPlayer::JMPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("JMPlayer"));

    ui->label->setStyleSheet("QLabel{background-color:rgb(0,0,0);}");

    point_x=ui->label->pos().x();
    point_y=ui->label->pos().y();
    labelHeight=ui->label->height();
    labelWidth=ui->label->width();

    isPlaying = false;
    //onButtonChange(false);
}

JMPlayer::~JMPlayer()
{
    delete ui;
}

void JMPlayer::onButtonChange(bool flag)
{
    ui->actionReplay->setEnabled(flag);
    ui->actionPause->setEnabled(flag);
    ui->actionStop->setEnabled(flag);
    ui->stopButton_2->setEnabled(flag);
    ui->pauseButton_2->setEnabled(flag);
}

void JMPlayer::onVideoPlay()
{
    const char *filepath;
    std::string str = currentFilePath.toStdString();
    filepath = str.c_str();

    ffmMgr.initFFmPlayer(filepath);

    sdlMgr.initSDL(ffmMgr.pCodecCtx->width,ffmMgr.pCodecCtx->height,ui->label);

    sdlMgr.setSDLRect(point_x,point_y,labelWidth,labelHeight);


    while(1)
    {
        sdlMgr.SDLWaitEvent();
        if(sdlMgr.getEvent().type==SFM_REFRESH_EVENT)
        {
            AVFrame *pFrameYUV = ffmMgr.displayvideo();
            if(NULL != pFrameYUV)
            {
                sdlMgr.SDLRender(pFrameYUV->data[0],pFrameYUV->linesize[0]);

            }
            else
            {
                sdlMgr.SDLPause();
                ui->pauseButton_2->setText("Replay");
            }

        }
        else if(sdlMgr.getEvent().type==SDL_QUIT)
        {
            sdlMgr.SDLStop();

        }
        else if(sdlMgr.getEvent().type==SFM_BREAK_EVENT)
        {
            break;
        }
    }

    qDebug("numFrame : %f     nb_frames  :  %lld   numFrame/ffmMgr.inputctx->streams[ffmMgr.videoindex]->nb_frames*1000  :   %f",ffmMgr.numFrame,ffmMgr.inputctx->streams[ffmMgr.videoindex]->nb_frames,ffmMgr.numFrame/ffmMgr.inputctx->streams[ffmMgr.videoindex]->nb_frames*1000);
    qDebug("duration :%lld   time_base.den : %d",ffmMgr.inputctx->streams[ffmMgr.videoindex]->duration,ffmMgr.inputctx->streams[ffmMgr.videoindex]->time_base.den);
    double t = ffmMgr.inputctx->streams[ffmMgr.videoindex]->duration*1.0/ffmMgr.inputctx->streams[ffmMgr.videoindex]->time_base.den;
    qDebug("时长：%d:%d:%d, 共%lf  s",(int)t/3600,(int)t%3600/60,(int)t%60,t);
    qDebug("frame_rate: %d",ffmMgr.inputctx->streams[ffmMgr.videoindex]->r_frame_rate.num);
    qDebug("\n");


    sdlMgr.SDLQuit();
    ui->label->clear();

    isPlaying = false;
    ui->label = new QLabel(this);
    ui->label->setGeometry(point_x,point_y,labelWidth,labelHeight);
    ui->label->show();


    ffmMgr.finishFFmpeg();
}


void JMPlayer::on_pauseButton_2_clicked()
{
    if(ui->pauseButton_2->text() == "Pause")
    {
        sdlMgr.SDLPause();

        ui->pauseButton_2->setText("Play");
        ui->actionOpenFile->setEnabled(false);
    }
    else if(ui->pauseButton_2->text() == "Play")
    {
        sdlMgr.SDLPlay();
        ui->pauseButton_2->setText("Pause");
        ui->actionOpenFile->setEnabled(false);
    }
    else if(ui->pauseButton_2->text() == "Replay")
    {
        sdlMgr.SDLPlay();
        ui->pauseButton_2->setText("Pause");
        ui->actionOpenFile->setEnabled(false);
        ffmMgr.rePlay();
    }
}

void JMPlayer::on_stopButton_2_clicked()
{
    sdlMgr.SDLStop();
    ui->pauseButton_2->setText("Pause");
    ui->actionOpenFile->setEnabled(true);
    onButtonChange(false);
}

void JMPlayer::on_quitButton_2_clicked()
{
    sdlMgr.SDLStop();
    this->close();
}

void JMPlayer::on_actionOpenFile_triggered()
{
    onButtonChange(true);
    ui->actionOpenFile->setEnabled(false);

    //在播放过程中判断是否重复点击播放按钮
    if(!isPlaying)
    {
        isPlaying = true;
    }
    else
    {
        return;
    }

    currentFilePath = QFileDialog::getOpenFileName(this,"please choose a video file", "","all(*);;mp4 FIle(*.mp4);;flv file(*.flv);;mov file(*.mov)");
    onVideoPlay();
}

void JMPlayer::on_actionPause_triggered()
{
    if(ui->pauseButton_2->text() == "Pause")
    {
        sdlMgr.SDLPause();
        ui->pauseButton_2->setText("Play");
        ui->actionOpenFile->setEnabled(false);
    }
    else
    {
        sdlMgr.SDLPlay();
        ui->pauseButton_2->setText("Pause");
        ui->actionOpenFile->setEnabled(false);
    }
}

void JMPlayer::on_actionReplay_triggered()
{
    ui->pauseButton_2->setText("Pause");

    ffmMgr.rePlay();

    ui->actionOpenFile->setEnabled(false);
    sdlMgr.SDLPlay();
}

void JMPlayer::on_actionStop_triggered()
{
    sdlMgr.SDLStop();

    ui->actionOpenFile->setEnabled(true);
    onButtonChange(false);
}

void JMPlayer::on_actionQuit_triggered()
{
    sdlMgr.SDLStop();;
    this->close();
}
