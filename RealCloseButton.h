#ifndef REALCLOSEBUTTON_H
#define REALCLOSEBUTTON_H

#include <QAbstractButton>

class RealCloseButton : public QAbstractButton
{
   Q_OBJECT

public:
   explicit RealCloseButton(QWidget *parent = 0);

   QSize sizeHint() const override;
   QSize minimumSizeHint() const override { return sizeHint(); }
   void enterEvent(QEvent *event) override;
   void leaveEvent(QEvent *event) override;
   void paintEvent(QPaintEvent *event) override;
};

#endif // REALCLOSEBUTTON_H
