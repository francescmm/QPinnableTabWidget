#include "QPinableTabWidget.h"

#include "FakeCloseButton.h"
#include "RealCloseButton.h"

#include <QMenu>
#include <QMouseEvent>
#include <QTabBar>
#include <QStyle>

namespace
{
class TabBarPin : public QTabBar
{
public:
   explicit TabBarPin(QWidget *parent = nullptr)
      : QTabBar(parent)
   {
   }

protected:
   void mousePressEvent(QMouseEvent *event) override
   {
      const QPoint pos = event->pos();
      mIndexToMove = indexAtPos(pos);

      QTabBar::mousePressEvent(event);
   }

   void mouseMoveEvent(QMouseEvent *event) override
   {
      if (!dynamic_cast<QPinableTabWidget *>(parentWidget())->isPinned(mIndexToMove))
         QTabBar::mouseMoveEvent(event);
   }

   void mouseReleaseEvent(QMouseEvent *event) override
   {
      mIndexToMove = -1;
      QTabBar::mouseReleaseEvent(event);
   }

private:
   int mIndexToMove = -1;

   int indexAtPos(const QPoint &p)
   {
      if (tabRect(currentIndex()).contains(p))
         return currentIndex();

      for (int i = 0; i < count(); ++i)
         if (isTabEnabled(i) && tabRect(i).contains(p))
            return i;

      return -1;
   }
};
}

QPinableTabWidget::QPinableTabWidget(QWidget *parent)
   : QTabWidget(parent)
{
   setTabBar(new TabBarPin());

   setTabsClosable(true);
   setMovable(true);
   connect(this, &QTabWidget::tabCloseRequested, this, &QPinableTabWidget::removeTab, Qt::QueuedConnection);
   connect(this, &QTabWidget::tabBarClicked, this, &QPinableTabWidget::clickRequested);
}

int QPinableTabWidget::addPinnedTab(QWidget *page, const QString &label)
{
   const auto tabIndex = addTab(page, label);
   tabBar()->setTabButton(
       tabIndex,
       static_cast<QTabBar::ButtonPosition>(style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, 0, this)),
       new FakeCloseButton());

   tabBar()->moveTab(tabIndex, mLastPinTab);

   mTabState.insert(mLastPinTab++, true);

   return tabIndex;
}

int QPinableTabWidget::addPinnedTab(QWidget *page, const QIcon &icon, const QString &label)
{
   const auto tabIndex = addTab(page, icon, label);
   tabBar()->setTabButton(
       tabIndex,
       static_cast<QTabBar::ButtonPosition>(style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, 0, this)),
       new FakeCloseButton());

   tabBar()->moveTab(tabIndex, mLastPinTab);

   mTabState.insert(mLastPinTab++, true);

   return tabIndex;
}

int QPinableTabWidget::addTab(QWidget *widget, const QString &s)
{
   return QTabWidget::addTab(widget, s);
}

int QPinableTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
   return QTabWidget::addTab(widget, icon, label);
}

int QPinableTabWidget::insertTab(int index, QWidget *widget, const QString &s)
{
   if (index <= mLastPinTab)
      index = mLastPinTab + 1;

   return QTabWidget::insertTab(index, widget, s);
}

int QPinableTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label)
{
   if (index <= mLastPinTab)
      index = mLastPinTab + 1;

   return QTabWidget::insertTab(index, widget, icon, label);
}

void QPinableTabWidget::removeTab(int index)
{
   QTabWidget::removeTab(index);

   if (mTabState.value(index))
      --mLastPinTab;

   mTabState.remove(index);
}

void QPinableTabWidget::clear()
{
   QTabWidget::clear();
   mLastPinnedTab = -1;
   mTabState.clear();
   mPrepareMenu = false;
   mClickedTab = -1;
   mLastPinTab = 0;
}

bool QPinableTabWidget::isPinned(int index)
{
   return mTabState.contains(index);
}

void QPinableTabWidget::mouseReleaseEvent(QMouseEvent *event)
{
   if (event->button() == Qt::RightButton)
      showContextMenu();
   else
      mClickedTab = -1;
}

void QPinableTabWidget::clickRequested(int index)
{
   mPrepareMenu = true;
   mClickedTab = index;
}

void QPinableTabWidget::showContextMenu()
{
   if (!mPrepareMenu)
      return;

   const auto actions = new QMenu(this);

   if (mTabState.value(mClickedTab))
      connect(actions->addAction("Unpin"), &QAction::triggered, this, &QPinableTabWidget::unpinTab);
   else
      connect(actions->addAction("Pin"), &QAction::triggered, this, &QPinableTabWidget::pintTab);

   connect(actions->addAction("Close"), &QAction::triggered, this, [this]() { removeTab(mClickedTab); });

   actions->exec(QCursor::pos());
}

void QPinableTabWidget::pintTab()
{
   tabBar()->setTabButton(
       mClickedTab,
       static_cast<QTabBar::ButtonPosition>(style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, 0, this)),
       new FakeCloseButton());

   if (mClickedTab != mLastPinTab)
      tabBar()->moveTab(mClickedTab, mLastPinTab);

   mTabState.insert(mLastPinTab, true);

   mLastPinTab = mTabState.count();

   mClickedTab = -1;
}

void QPinableTabWidget::unpinTab()
{
   tabBar()->setTabButton(
       mClickedTab,
       static_cast<QTabBar::ButtonPosition>(style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition, 0, this)),
       new RealCloseButton());

   mTabState.remove(mClickedTab);

   mLastPinTab = mTabState.count();

   auto deletions = false;

   for (auto pair : mTabState.toStdMap())
   {
      if (pair.first > mClickedTab)
      {
         mTabState[pair.first - 1] = pair.second;
         deletions = true;
      }
   }

   if (deletions)
      mTabState.remove(mTabState.lastKey());

   tabBar()->moveTab(mClickedTab, mLastPinTab);

   mClickedTab = -1;
}
