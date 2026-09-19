#include <QApplication>
#include <QMainWindow>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QMetaObject>
#include <QTranslator>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include "myclass.h"
#include "settings.h"
#include "dialogmanager.h"

static void buildMenus(QMainWindow &window, QQuickItem *root,
                       QApplication &app, Settings &settings,
                       DialogManager &dialogManager, MyClass &myClass)
{
    QMenuBar *menuBar = window.menuBar();

    QMenu *fileMenu = menuBar->addMenu(QObject::tr("File"));
    QAction *newGameAction = fileMenu->addAction(QObject::tr("New Game"));
    newGameAction->setShortcut(QKeySequence::New);
    QObject::connect(newGameAction, &QAction::triggered, root, [root] {
        if (root)
            QMetaObject::invokeMethod(root, "startNewGame", Qt::DirectConnection);
    });
    QAction *exitAction = fileMenu->addAction(QObject::tr("Exit"));
    exitAction->setShortcut(QKeySequence::Quit);
    QObject::connect(exitAction, &QAction::triggered, &app, [&app] { app.quit(); });

    QMenu *settingsMenu = menuBar->addMenu(QObject::tr("Settings"));
    QMenu *labelMenu = settingsMenu->addMenu(QObject::tr("Labeling"));

    auto addLabel = [&](const QString &name, const QString &value) {
        QAction *act = labelMenu->addAction(name);
        act->setCheckable(true);
        act->setData(value);
        QObject::connect(act, &QAction::triggered, root, [root, labelMenu, value] {
            if (root)
                QMetaObject::invokeMethod(root, "setLabel", Qt::DirectConnection,
                                          Q_ARG(QVariant, value));
            const auto acts = labelMenu->actions();
            for (QAction *a : acts)
                a->setChecked(a->data().toString() == value);
        });
        return act;
    };
    addLabel(QObject::tr("2048"), "2048");
    addLabel(QObject::tr("Degree"), "Degree");
    addLabel(QObject::tr("Military Rank"), "Military Rank");
    addLabel(QObject::tr("PRC"), "PRC");

    QActionGroup *labelGroup = new QActionGroup(&window);
    labelGroup->setExclusive(true);
    const auto labelActs = labelMenu->actions();
    for (QAction *a : labelActs) labelGroup->addAction(a);

    QMenu *languageMenu = settingsMenu->addMenu(QObject::tr("Language"));
    auto addLanguage = [&](const QString &name, const QString &code) {
        QAction *act = languageMenu->addAction(name);
        act->setCheckable(true);
        act->setChecked(settings.value("language").toString() == code);
        QObject::connect(act, &QAction::triggered, &dialogManager, [&settings, &dialogManager, code] {
            if (settings.value("language").toString() != code) {
                settings.setValue("language", code);
                dialogManager.showLanguageChangedHint();
            }
        });
        return act;
    };
    addLanguage("English", "en_US");
    addLanguage(QString::fromUtf8("Français"), "fr_FR");
    addLanguage(QString::fromUtf8("简体中文"), "zh_CN");
    addLanguage("Polski", "pl_PL");
    addLanguage(QString::fromUtf8("Русский"), "ru_RU");
    addLanguage("German", "de_DE");

    QMenu *helpMenu = menuBar->addMenu(QObject::tr("Help"));
    QAction *aboutAction = helpMenu->addAction(QObject::tr("About"));
    QObject::connect(aboutAction, &QAction::triggered, &dialogManager, [&dialogManager] {
        dialogManager.showAboutDialog();
    });
    QAction *aboutQtAction = helpMenu->addAction(QObject::tr("About Qt"));
    QObject::connect(aboutQtAction, &QAction::triggered, &myClass, [&myClass] {
        myClass.aboutQt();
    });

    if (root) {
        const QString cur = root->property("currentLabel").toString();
        for (QAction *a : labelActs)
            a->setChecked(a->data().toString() == cur);
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Settings settings(0, "2048-qt", "2048-Qt");
    settings.setVersion(QString(APP_VERSION));

    // Localization
    QString locale;
    if (settings.contains("language")) {
        locale = settings.value("language").toString();
    } else {
        locale = QLocale::system().name();
        settings.setValue("language", locale);
    }

    QTranslator translator;
    if (! locale.startsWith("en")) {
        QString tsFile = "2048-qt_" + locale;

        if (translator.load(tsFile, ":/ts")) {
            qDebug() << "Successfully loaded " + tsFile;
            app.installTranslator(&translator);
        } else {
            qDebug() << "Failed to load " + tsFile;
        }
    }

    QMainWindow window;
    window.setWindowTitle(QObject::tr("2048 Game"));
    window.resize(550, 740);

    MyClass myClass;
    DialogManager dialogManager(&window, &settings, settings.getVersion());

    QQuickWidget *view = new QQuickWidget(&window);
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    window.setCentralWidget(view);

    QQmlEngine *engine = view->engine();
    engine->rootContext()->setContextProperty("myClass", &myClass);
    engine->rootContext()->setContextProperty("settings", &settings);
    engine->rootContext()->setContextProperty("dialogManager", &dialogManager);

    QObject::connect(view, &QQuickWidget::statusChanged, &window,
                     [view, &window, &app, &settings, &dialogManager, &myClass]
                     (QQuickWidget::Status status) {
        if (status != QQuickWidget::Ready)
            return;
        QQuickItem *root = view->rootObject();
        buildMenus(window, root, app, settings, dialogManager, myClass);
    });

    view->setSource(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    window.show();

    return app.exec();
}
