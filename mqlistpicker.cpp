#include "MQListPicker.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QKeyEvent>

//MQListPicker::MQListPicker(QWidget *parent) :
//    QWidget(parent),
//    _labels(QVector<QLabel*>(3)),
//    centerLabel(1),
//    circleItem(false)
//{
//    initView();
//}

MQListPicker::MQListPicker(QWidget *parent, int numOfRowsShown, int centerFontSize, QColor primaryColor, QColor inactiveColor) :
    QWidget(parent),
    primaryColor(primaryColor),
    inactiveColor(inactiveColor),
    centerFontSize(centerFontSize),
    _labels(QVector<QLabel*>(numOfRowsShown <= 0 ? numOfRowsShown = 3
        : numOfRowsShown > 7 ? numOfRowsShown = 7
        : (numOfRowsShown % 2 == 0 ? --numOfRowsShown
        : numOfRowsShown))),
    centerLabel(numOfRowsShown / 2),
    circleItem(false)
{
    initView();
}

MQListPicker::~MQListPicker()
{
//    std::for_each(_labels.begin(), _labels.end(), [] (QLabel *p) {delete p;});
    qDeleteAll(_labels);
}

void MQListPicker::initView()
{
    setFocusPolicy(Qt::StrongFocus);

    QString stylesheet = R"(
                       .QLabel { font: %1 %2pt }
                       .QLabel:disabled { color : %3 }
                       .QLabel:enabled { color : %4 }
                       .QLabel:enabled:hover { color : %5 }
                       )";
    int red = primaryColor.red(), green = primaryColor.green(), blue = primaryColor.blue();
    for (int i = 0, k = _labels.size(); i < k; i++)
    {
        _labels[i] = new QLabel(this);
        _labels[i]->setAlignment(Qt::AlignCenter);
        _labels[i]->setEnabled(false);
        _labels[i]->setStyleSheet(stylesheet.arg(centerLabel == i ? "bold" : "")
                                  .arg(centerFontSize - abs(i - centerLabel) * 4)
                                  .arg(inactiveColor.name())
                                  .arg(QColor(red, green, blue, 0xFF - abs(i - centerLabel) * 0x20 - 0x10).name(QColor::HexArgb))
                                  .arg(primaryColor.name()));
    }
    _labels[centerLabel]->setObjectName("centerLabel");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(6);
    layout->setContentsMargins(6, 6, 6, 6);

    layout->addStretch();
    for (int i = 0, k = _labels.size(); i < k; i++)
    {
        layout->addWidget(_labels[i]);
    }
    layout->addStretch();
    this->setLayout(layout);

    connect(this, &MQListPicker::onSelectedValueChanged, this, &MQListPicker::refreshList);
    connect(this, &MQListPicker::onMaxShownItemsChanged, this, &MQListPicker::refreshList);
}

void MQListPicker::createList(const int numOfListElements, int startValue)
{
    if (_strings.size() != numOfListElements)
    {
        _strings.resize(numOfListElements);
    }
    for (int i = 0; i < numOfListElements; i++)
    {
        _strings[i] = QString::number(startValue++);
    }
    maxShownIndex = numOfListElements - 1;
    currentIndex = 0;
    if (_strings.empty())
        return;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::createList(std::initializer_list<QString> list)
{
    _strings.clear();
    _strings = QVector<QString>(list);
    maxShownIndex = _strings.size() - 1;
    currentIndex = 0;
    if (_strings.empty())
        return;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::createList(QVector<QString> &vector)
{
    _strings.clear();
    _strings = vector;
    maxShownIndex = _strings.size() - 1;
    currentIndex = 0;
    if (_strings.empty())
        return;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::createList(QVector<QString> &&vector)
{
    _strings.clear();
    _strings = std::move(vector);
    maxShownIndex = _strings.size() - 1;
    currentIndex = 0;
    if (_strings.empty())
        return;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::createList(QList<QString> &list)
{
    _strings.clear();
    _strings = QVector<QString>::fromList(list);
    maxShownIndex = _strings.size() - 1;
    currentIndex = 0;
    if (_strings.empty())
        return;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::createList(QList<QString> &&list)
{
    _strings.clear();
    _strings = QVector<QString>::fromList(std::move(list));
    maxShownIndex = _strings.size() - 1;
    currentIndex = 0;
    if (_strings.empty())
        return;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::setSelectedItem(int itemIndex)
{
    if (_strings.empty())
        return;

    currentIndex = itemIndex;
    int max = _strings.size() - 1;
    currentIndex > max ? currentIndex = max : currentIndex;
    currentIndex > maxShownIndex ? currentIndex = maxShownIndex : currentIndex;
    currentIndex < 0 ? currentIndex = 0 : currentIndex;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

QString MQListPicker::selectedValue()
{
    return _labels[centerLabel]->text();
}

int MQListPicker::selectedIndex()
{
    return currentIndex;
}

void MQListPicker::setCircling(bool newval)
{
    circleItem = newval;
}

bool MQListPicker::circling()
{
    return circleItem;
}

void MQListPicker::setMaxShownItems(const int newval)
{
    maxShownIndex = newval - 1;
    emit onMaxShownItemsChanged(currentIndex);
}

int MQListPicker::prevIndex(int index)
{
    if (index > 0)
        --index;
    else
        index = maxShownIndex;
    return index;
}

int MQListPicker::nextIndex(int index)
{
    if (index < maxShownIndex)
        ++index;
    else
        index = 0;
    return index;
}

void MQListPicker::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0)
    {
        upCount += event->delta();
        if (upCount >= 80)
        {
            scrollUp();
            upCount = 0;
        }
    }
    else if (event->delta() < 0)
    {
        downCount -= event->delta();
        if (downCount >= 80)
        {
            scrollDown();
            downCount = 0;
        }
    }
    event->accept();
}

void MQListPicker::mousePressEvent(QMouseEvent *event)
{
    currentCursorXPos = event->pos().y();
}

void MQListPicker::mouseMoveEvent(QMouseEvent *event)
{
    if ((geometry().contains(QCursor::pos())) && (event->buttons() & Qt::LeftButton))
    {
        if (event->pos().y() < currentCursorXPos)
        {
            upCount += 2;
            if  (upCount >= 30)
            {
                currentCursorXPos = event->pos().y();
                scrollDown();
                upCount = 0;
            }
        }
        else if (event->pos().y() > currentCursorXPos)
        {
            downCount += 2;
            if (downCount >= 30)
            {
                currentCursorXPos = event->pos().y();
                scrollUp();
                downCount = 0;
            }
        }
    }
}

void MQListPicker::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->pos().y() == currentCursorXPos)
    {
        for (int i = 0, k = _labels.size();i < k;++i)
        {
            if (_labels[i]->geometry().contains(event->pos()))
            {
//                if (_labels[i]->text().isEmpty())
//                    break;
                int index = currentIndex + (i - centerLabel);
                if (circleItem)
                    index = (index + maxShownIndex + 1) % (maxShownIndex + 1);
                setSelectedItem(index);
                emit onSelectedItemClicked(index, index >= 0 && index <= maxShownIndex ? _strings[index] : "");
                break;
            }
        }
    }
}

void MQListPicker::resizeEvent(QResizeEvent *event)
{
    event->accept();
}

void MQListPicker::changeEvent(QEvent *event)
{
//    if (event->type() == QEvent::EnabledChange)
//    {
//        bool enabled =this->isEnabled();
//        foreach (QLabel *label, _labels)
//        {
//            label->setEnabled(enabled);
//        }
//    }
}

void MQListPicker::focusInEvent(QFocusEvent *event)
{
    foreach (QLabel *label, _labels)
    {
        label->setEnabled(true);
    }
}

void MQListPicker::focusOutEvent(QFocusEvent *event)
{
    foreach (QLabel *label, _labels)
    {
        label->setEnabled(false);
    }
}

void MQListPicker::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up)
    {
        scrollUp();
        event->accept();// this is optional
    }
    else if (event->key() == Qt::Key_Down)
    {
        scrollDown();
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit onSelectedItemClicked(currentIndex, _strings[currentIndex]);
    }
    else
        event->ignore();// this is necessary otherwise key would not transmitted to parent
}

void MQListPicker::scrollUp()
{
    if (_strings.empty())
        return;

    if (currentIndex >= 0)
        currentIndex--;
    if (currentIndex < 0 && circleItem)
        currentIndex = maxShownIndex;
    else if (currentIndex < 0 && !circleItem)
        currentIndex = 0;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::scrollDown()
{
    if (_strings.empty())
        return;

    if (currentIndex <= maxShownIndex)
        currentIndex++;
    if (currentIndex > maxShownIndex && circleItem)
        currentIndex = 0;
    else if (currentIndex > maxShownIndex && !circleItem)
        currentIndex = maxShownIndex;
    emit onSelectedValueChanged(currentIndex, _strings[currentIndex]);
}

void MQListPicker::refreshList(int centerIndex)
{
    if (_strings.empty())
        return;

    if (circleItem)
    {
        _labels[centerLabel]->setText(_strings[centerIndex]);
        int pr_index = prevIndex(centerIndex);
        for (int i = centerLabel - 1; i >= 0; i--)
        {
            if (pr_index != centerIndex)
                _labels[i]->setText(_strings[pr_index]);
            else
                _labels[i]->setText("");
            pr_index = prevIndex(pr_index);
        }
        int nt_index = nextIndex(centerIndex);
        for (int i = centerLabel + 1, k = _labels.size(); i < k; i++)
        {
            if (nt_index != centerIndex)
                _labels[i]->setText(_strings[nt_index]);
            else
                _labels[i]->setText("");
            nt_index = nextIndex(nt_index);
        }
    }
    else
    {
        _labels[centerLabel]->setText(_strings[centerIndex]);
        int pr_index = 0, nt_index = 0;
        if (centerIndex >= 0)
            pr_index = centerIndex - 1;
        for (int i = centerLabel - 1; i >= 0; i--)
        {
            if (pr_index >= 0)
            {
                _labels[i]->setText(_strings[pr_index]);
                pr_index--;
            }
            else
                _labels[i]->setText("");
        }
        if (centerIndex <= maxShownIndex)
            nt_index = centerIndex + 1;
        for (int i = centerLabel + 1, k = _labels.size(); i < k; i++)
        {
            if (nt_index <= maxShownIndex)
            {
                _labels[i]->setText(_strings[nt_index]);
                nt_index++;
            }
            else
                _labels[i]->setText("");
        }
    }
}
