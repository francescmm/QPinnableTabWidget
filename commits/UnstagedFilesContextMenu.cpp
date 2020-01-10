#include "UnstagedFilesContextMenu.h"

#include <GitBase.h>
#include <GitSyncProcess.h>
#include <GitLocal.h>

#include <QFile>
#include <QDir>
#include <QMessageBox>

UnstagedFilesContextMenu::UnstagedFilesContextMenu(const QSharedPointer<GitBase> &git, const QString &fileName,
                                                   bool hasConflicts, QWidget *parent)
   : QMenu(parent)
   , mGit(git)
   , mFileName(fileName)
{
   connect(addAction("See changes"), &QAction::triggered, this, &UnstagedFilesContextMenu::signalShowDiff);

   addSeparator();

   if (hasConflicts)
   {
      connect(addAction("Mark as resolved"), &QAction::triggered, this, [this] {
         QScopedPointer<GitLocal> git(new GitLocal(mGit));
         const auto ret = git->markFileAsResolved(mFileName);

         if (ret.success)
            emit signalConflictsResolved();
      });

      addSeparator();
   }

   connect(addAction("Revert file changes"), &QAction::triggered, this, [this]() {
      const auto msgBoxRet
          = QMessageBox::question(this, tr("Ignoring file"), tr("Are you sure you want to revert the changes?"));

      if (msgBoxRet == QMessageBox::Yes)
      {
         QScopedPointer<GitLocal> git(new GitLocal(mGit));
         const auto ret = git->checkoutFile(mFileName);

         emit signalCheckedOut(ret);
      }
   });

   connect(addAction("Blame"), &QAction::triggered, this, &UnstagedFilesContextMenu::signalShowFileHistory);

   connect(addAction("Ignore file"), &QAction::triggered, this, [this]() {
      const auto ret = QMessageBox::question(this, tr("Ignoring file"),
                                             tr("Are you sure you want to add the file to the black list?"));

      if (ret == QMessageBox::Yes)
      {
         const auto gitRet = addEntryToGitIgnore(mFileName);

         if (gitRet)
            emit signalCheckedOut(gitRet);
      }
   });

   connect(addAction("Ignore extension"), &QAction::triggered, this, [this]() {
      const auto msgBoxRet = QMessageBox::question(this, tr("Ignoring file"),
                                                   tr("Are you sure you want to add the file to the black list?"));

      if (msgBoxRet == QMessageBox::Yes)
      {
         auto fileParts = mFileName.split(".");
         fileParts.takeFirst();
         const auto extension = QString("*.%1").arg(fileParts.join("."));
         const auto ret = addEntryToGitIgnore(extension);

         if (ret)
            emit signalCheckedOut(ret);
      }
   });

   /*
   QAction *removeAction = nullptr;
   connect(removeAction = addAction("Remove file"), &QAction::triggered, this, []() {});
   removeAction->setDisabled(true);
   */

   addSeparator();

   connect(addAction("Add all files to commit"), &QAction::triggered, this, &UnstagedFilesContextMenu::signalCommitAll);
   connect(addAction("Revert all changes"), &QAction::triggered, this, [this]() {
      const auto msgBoxRet = QMessageBox::question(this, tr("Ignoring file"),
                                                   tr("Are you sure you want to add the file to the black list?"));
      if (msgBoxRet == QMessageBox::Yes)
         emit signalRevertAll();
   });
}

bool UnstagedFilesContextMenu::addEntryToGitIgnore(const QString &entry)
{
   auto entryAdded = false;
   QDir d(mGit->getWorkingDir());
   QFile f(d.absolutePath() + "/.gitignore");

   if (!f.exists())
   {
      if (f.open(QIODevice::ReadWrite))
         f.close();
   }

   if (f.open(QIODevice::Append))
   {
      const auto bytesWritten = f.write(entry.toUtf8() + "\n");

      if (bytesWritten != -1)
      {
         QMessageBox::information(this, tr("File added to .gitignore"),
                                  tr("The file has been added to the ignore list in the file .gitignore."));

         entryAdded = true;
      }
      else
         QMessageBox::warning(this, tr("Unable to add the entry"),
                              tr("It was impossible to add the entry in the .gitignore file."));

      f.close();
   }

   return entryAdded;
}
