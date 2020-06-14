#ifndef FAKECLOSEBUTTON_H
#define FAKECLOSEBUTTON_H

#include <QAbstractButton>

class FakeCloseButton : public QAbstractButton
{
   Q_OBJECT

public:
   explicit FakeCloseButton(QWidget *parent = 0);

   void paintEvent(QPaintEvent *) override { }
};

#endif // FAKECLOSEBUTTON_H
