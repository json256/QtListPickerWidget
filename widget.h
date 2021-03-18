#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class MQListPicker;

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

    void InitView();

protected:
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void PickerDateChanged(int itemIndex, QString labelText);
    void PickerTimeClicked(int itemIndex, QString labelText);

private:
    MQListPicker *m_pListPickerDate;
    MQListPicker *m_pListPickerTime;
};

#endif // WIDGET_H
