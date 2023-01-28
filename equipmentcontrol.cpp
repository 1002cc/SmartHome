#include "equipmentcontrol.h"
#include "ui_equipmentcontrol.h"

EquipmentControl::EquipmentControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EquipmentControl)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    grid=new QGridLayout(ui->widgetcontrol);
    ui->addwidget->hide();
    this->setGeometry(this->parentWidget()->x(),this->parentWidget()->y(),this->parentWidget()->width(),this->parentWidget()->height());
    c=new controlwidget(ui->widgetcontrol);
    c->setname("客厅");
    c->setMinimumSize(160,160);
    grid->addWidget(c,0,0);
    grid->setHorizontalSpacing(5);
    grid->setAlignment(Qt::AlignTop);
}

EquipmentControl::~EquipmentControl()
{
    delete ui;
}


void EquipmentControl::on_pushButton_clicked()
{
    this->parentWidget()->show();
    this->close();
}

void EquipmentControl::on_pushButton_2_clicked()
{
    if(ui->addwidget->isHidden())
        ui->addwidget->show();
    else
        ui->addwidget->hide();
}

void EquipmentControl::on_addButton_clicked()
{
    ui->lineEdit->text();
    ui->addwidget->hide();
    controlwidget* c1=new controlwidget();
    c1->setMinimumSize(160,160);
    c1->setname(ui->lineEdit->text());
    i++;
    if(i>=4)
    {
        j++;
        i=0;
    }
    grid->addWidget(c1,j,i);
}
