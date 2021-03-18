#ifndef MQLISTPICKER_H
#define MQLISTPICKER_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QList>

class MQListPicker : public QWidget
{
    Q_OBJECT
private: /*Members*/
    //Labels to show rows
    QVector<QLabel*> _labels;
    //Central label
    int centerLabel;
    //Items storage
    QVector<QString> _strings;
    //Maximum number of elements in list
    int maxShownIndex = 0;
    //Currently selected index
    int currentIndex = 0;
    //Indicates whether list should be shown circle like
    bool circleItem;
    //Used to store initital mouse cursor x position
    int currentCursorXPos = 0;

    QColor primaryColor;
    QColor inactiveColor;
    int centerFontSize;

public: /*Methods*/
    //MQListPicker(QWidget *parent = nullptr);
    //Parent widget should be specified in constructor. Nullptr is not possible. Because list presentation is based on parent widget height.
    //Passed number should be odd, otherwise passed value will be decremented to nearest odd one
    //If zero is passed the number of shown rows will be calculated on base of widget height
    MQListPicker(QWidget *parent, int numOfRowsShown = 3, int centerFontSize = 28, QColor primaryColor = "#4FD35B", QColor inactiveColor = "#404040");

    //List creation. One of the createList function should be invoked before adding or showing widget.
    //Creating list of numbers starting from startValue
    void createList(const int numOfListElements, int startValue);
    //Creating list using initializer list
    void createList(std::initializer_list<QString> list);
    //Creating list from existing vector
    void createList(QVector<QString> &vector);
    void createList(QVector<QString> &&vector);
    //Creating list from existing list
    void createList(QList<QString> &list);
    void createList(QList<QString> &&list);
    //Setting selected item
    void setSelectedItem(int itemIndex);
    //Getting value of the selected item
    QString selectedValue();
    //Getting index of the selected item
    int selectedIndex();
    //Setting circling (true by default)
    void setCircling(bool newval);
    //Checking if circled
    bool circling();

    //Scroll
    int upCount = 0, downCount = 0;
    void scrollUp();
    void scrollDown();

public slots:
    //Setting maximum of shown items (Useful if number of shown items should be limited depending on some circumstances)
    //For example: days should be decreased/increased depending on month selected
    void setMaxShownItems(const int newval);

private: /*Methods*/
    void initView();

    //Functions used to set previous and next items
    int prevIndex(int index);
    int nextIndex(int index);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void refreshList(int centerIndex = 0);

signals:
    //Is emited if setMaxShownItems was invoked
    void onMaxShownItemsChanged(int maxShownIndex);
    //Is emited if selectedItem had changed either by setSelectedItem function or Scroll event
    void onSelectedValueChanged(int itemIndex, QString labelText);
    //Is emited if selectedItem is clicked
    void onSelectedItemClicked(int itemIndex, QString labelText);

public: /*Destructor*/
    ~MQListPicker() override;
};

#endif // MQLISTPICKER_H
