#include "lightablewidget.h"
#include <QDebug>

LightableWidget::LightableWidget(QWidget *parent)
    : QWidget(parent)
//    , lighted(false)
{
    qDebug() << styleSheet();
}

//void LightableWidget::setLighted(bool lighted)
//{
//    if (this->lighted == lighted) return;

//    QString styleSheet = this->styleSheet();
//    if (lighted)
//    {
//        this->setStyleSheet(styleSheet.replace("/off/", "/on/"));
//    }
//    else
//    {
//        this->setStyleSheet(styleSheet.replace("/on/", "/off/"));
//    }

//    this->lighted = lighted;
//}
