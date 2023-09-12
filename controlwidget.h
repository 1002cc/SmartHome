#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

namespace Ui
{
    class controlwidget;
}

class controlwidget : public QWidget
{
    Q_OBJECT
public:
    explicit controlwidget(QWidget *parent = nullptr);
    ~controlwidget();
    void setname(const QString &name);
    void setid(const QString &id);
private slots:
    void on_bottomc(bool isc);

private:
    Ui::controlwidget *ui;
    QString name;
    QString id;
};

#endif // CONTROLWIDGET_H
