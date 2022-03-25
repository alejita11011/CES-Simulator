#ifndef LIGHTABLEWIDGET_H
#define LIGHTABLEWIDGET_H

#include <QWidget>

class LightableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LightableWidget(QWidget *parent = nullptr);
//    void setLighted(bool lighted);

signals:

private:
//    bool lighted;
};

#endif // LIGHTABLEWIDGET_H
