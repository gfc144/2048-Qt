#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H

#include <QObject>
#include <QString>

class QWidget;
class Settings;

class DialogManager : public QObject
{
    Q_OBJECT
public:
    explicit DialogManager(QWidget *parent, Settings *settings, const QString &version);

public slots:
    void showLanguageChangedHint();
    void showAboutDialog();
    void showGameOverDialog();
    void showWinDialog();

signals:
    void requestNewGame();
    void continuePlaying();
    void quitApp();

private:
    QWidget *m_parent;
    Settings *m_settings;
    QString m_version;
};

#endif // DIALOGMANAGER_H
