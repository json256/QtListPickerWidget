#include "widget.h"
#include "mqlistpicker.h"
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QVector>
#include <QList>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    InitView();
}

Widget::~Widget()
{
}

void Widget::InitView()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    QString stylesheet = R"(
                         QLabel#centerLabel { font: bold 28pt }
                         QLabel#centerLabel:enabled { color : #E04FD35B }
                         QLabel#centerLabel:enabled:hover { color : #4FD35B }
                         .QLabel:disabled { color : #40FFFFFF }
                         .QLabel:enabled { color : #804FD35B }
                         .QLabel:enabled:hover { color : #4FD35B }
                         )";

    m_pListPickerDate = new MQListPicker(this, 5);
    connect(m_pListPickerDate, &MQListPicker::onSelectedValueChanged, this, &Widget::PickerDateChanged);
//    m_pListPickerDate->createList({"2020-09-01", "2020-09-02", "2020-09-03", "2020-09-04", "2020-09-05", "2020-09-06"});
    QVector<QString> v = QVector<QString>(6, "2020-09-01");
    m_pListPickerDate->createList(v);
//    m_pListPickerDate->createList(QVector<QString>(6, "2020-09-01"));
//    m_pListPickerDate->setStyleSheet(stylesheet);
    layout->addWidget(m_pListPickerDate);

    m_pListPickerTime = new MQListPicker(this, 7);
    connect(m_pListPickerTime, &MQListPicker::onSelectedItemClicked, this, &Widget::PickerTimeClicked);
    m_pListPickerTime->createList(20, 1);
    m_pListPickerTime->setMaxShownItems(16);
//    m_pListPickerTime->setStyleSheet(stylesheet);
    layout->addWidget(m_pListPickerTime);

    this->setLayout(layout);
//    QWidget::setTabOrder(m_pListPickerDate, m_pListPickerTime);

}

void Widget::PickerDateChanged(int itemIndex, QString labelText)
{
    qDebug() << itemIndex << labelText;
}

void Widget::PickerTimeClicked(int itemIndex, QString labelText)
{
    qDebug() << itemIndex << labelText;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Menu:
        QCoreApplication::postEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier));
        break;
    default:
        break;
    }
}
