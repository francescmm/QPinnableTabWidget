#ifndef QTABWIDGETPIN_H
#define QTABWIDGETPIN_H

#include <qpinabletabwidget_global.h>

#include <QTabWidget>
#include <QMap>

class QPINABLETABWIDGETLIB_EXPORT QPinableTabWidget : public QTabWidget
{
   Q_OBJECT

public:
   explicit QPinableTabWidget(QWidget *parent = nullptr);
   ~QPinableTabWidget() override = default;

   int addPinnedTab(QWidget *page, const QString &label);
   int addPinnedTab(QWidget *page, const QIcon &icon, const QString &label);

   int addTab(QWidget *widget, const QString &s);
   int addTab(QWidget *widget, const QIcon &icon, const QString &label);

   int insertTab(int index, QWidget *widget, const QString &s);
   int insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label);

   void removeTab(int index);

   bool tabsClosable() const { return QTabWidget::tabsClosable(); }

   void clear();

   bool isPinned(int index);

protected:
   void mouseReleaseEvent(QMouseEvent *event) override;

private:
   int mLastPinnedTab = -1;
   QMap<int, bool> mTabState;
   bool mPrepareMenu = false;
   int mClickedTab = -1;
   int mLastPinTab = 0;

   void clickRequested(int index);
   void showContextMenu();
   void pintTab();
   void unpinTab();
   QTabBar *tabBar() const { return QTabWidget::tabBar(); }
   void setTabsClosable(bool closeable) { QTabWidget::setTabsClosable(closeable); }

   int indexAtPos(const QPoint &p);
};
#endif // QTABWIDGETPIN_H
