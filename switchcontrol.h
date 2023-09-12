#ifndef SWITCHCONTROL_H
#define SWITCHCONTROL_H
#include <QLabel>
#include <QWidget>

class QSwitchButton : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
    void clicked(bool is_selected);
public:
    QSwitchButton(QWidget *parent=0);
    ~QSwitchButton();
    typedef enum _buttonStyle{
        Rectage,    //矩形样式
        Ellipse,    //椭圆样式
    }ButtonStyle;
    void SetButtonStyle(ButtonStyle button_style);
    void SetSize(int width, int height);
    void SetSize(const QSize& size);
    void SetBackgroundColor(const QColor& selected_color, const QColor& not_selected_color);
    void SetSliderColor(const QColor& selected_color, const QColor& not_selected_color);
    void SetRound(int round);
    void EnableAntiAliasing(bool enable);
    bool IsAntiAliasing();
    void SetSelected(bool is_selected);
    bool IsSelected();
    void SetEnabel(bool is_enable);
    bool IsEnable();
protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
private:
    void DrawBackRect(QPainter* painter, const QRectF& rect);
    void DrawSliderRect(QPainter* painter, const QRectF& rect);
private:
    bool isEnable;  //是否启用
    bool isSelected;    //当前是否为选中状态
    bool isAntiAliasing;    //是否开启抗锯齿
    int buttonWidth;
    int buttonHeight;
    QColor backgroundColorSelected;
    QColor backgroundColorNotSelected;
    QColor sliderColorSelected;
    QColor sliderColorNotSelected;
    int rectRound;
    ButtonStyle buttonStyle;
};
#endif // SWITCHCONTROL_H
