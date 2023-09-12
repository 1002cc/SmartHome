#ifndef EQUIPMENTCONTROL_H
#define EQUIPMENTCONTROL_H

#include <QMainWindow>
#include "controlwidget.h"
#include <QGridLayout>
#include <QScrollArea>
namespace Ui {
class EquipmentControl;
}

class EquipmentControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit EquipmentControl(QWidget *parent = nullptr);
    ~EquipmentControl();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_addButton_clicked();
private:
    Ui::EquipmentControl *ui;
    controlwidget*c;
    QString name;
    QGridLayout* grid;
    QScrollArea* scrollArea;
    int i=0;
    int j=0;
};

#endif // EQUIPMENTCONTROL_H
