#include "controlwidget.h"
#include "ui_controlwidget.h"
#include "switchcontrol.h"
#include <QDebug>
#include <QColor>
controlwidget::controlwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controlwidget)
{
    ui->setupUi(this);
    ui->label_name->setAlignment(Qt::AlignCenter);
    QSwitchButton*q=new QSwitchButton(ui->frame_control);
    q->SetBackgroundColor(QColor(71,146,230),QColor(213,213,213));
    q->setGeometry(20,100,100,50);
    connect(q,SIGNAL(clicked(bool)),this,SLOT(on_bottomc(bool)));
}

controlwidget::~controlwidget()
{
    delete ui;
}

void controlwidget::setname(const QString &name)
{
    ui->label_name->setText(name);
}

void controlwidget::setid(const QString &id)
{
    this->id=id;
}

void controlwidget::on_bottomc(bool isc)
{
    qDebug()<<isc;

}
