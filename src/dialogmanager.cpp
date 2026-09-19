#include "dialogmanager.h"

#include <QMessageBox>
#include <QWidget>
#include "settings.h"

DialogManager::DialogManager(QWidget *parent, Settings *settings, const QString &version)
    : QObject(parent), m_parent(parent), m_settings(settings), m_version(version)
{
}

void DialogManager::showLanguageChangedHint()
{
    QMessageBox::information(m_parent,
                             tr("Language Setting Hint"),
                             tr("Please restart the program to make the language setting take effect."));
}

void DialogManager::showAboutDialog()
{
    QString text = QStringLiteral("<p style='font-weight: bold; font-size: 24px'>2048-Qt</p>"
                                  "<p>Version %1</p>"
                                  "<p>2015 Qiaoyong Zhong &lt;solary.sh@gmail.com&gt;</p>")
                       .arg(m_version);
    QMessageBox::about(m_parent, tr("About 2048-Qt"), text);
}

void DialogManager::showGameOverDialog()
{
    QMessageBox box(m_parent);
    box.setIcon(QMessageBox::Warning);
    box.setWindowTitle(tr("Game Over"));
    box.setText(tr("Game Over!"));
    box.setStandardButtons(QMessageBox::Retry | QMessageBox::Abort);
    box.setDefaultButton(QMessageBox::Retry);
    int result = box.exec();

    if (result == QMessageBox::Retry)
        emit requestNewGame();
    else
        emit quitApp();
}

void DialogManager::showWinDialog()
{
    QMessageBox box(m_parent);
    box.setIcon(QMessageBox::Question);
    box.setWindowTitle(tr("You Win"));
    box.setText(tr("You win! Continue playing?"));
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setDefaultButton(QMessageBox::Yes);
    int result = box.exec();

    if (result == QMessageBox::Yes)
        emit continuePlaying();
    else
        emit requestNewGame();
}
