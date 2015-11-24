#include "videoclient.h"
#include "ui_videoclient.h"

VideoClient::VideoClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoClient)
{
    ui->setupUi(this);
}

VideoClient::~VideoClient()
{
    delete ui;
}
