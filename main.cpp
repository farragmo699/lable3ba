#include "login.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName("Inventory Management System");
    QApplication::setOrganizationName("YourCompany");

    if (QFile::exists("reset_users.flag")) {
        if (QFile::remove("users.dat")) {
            qDebug() << "Reset users database";
        } else {
            qWarning() << "Failed to reset users database";
        }
        QFile::remove("reset_users.flag");
    }

    Login login;
    login.show();

    a.setQuitOnLastWindowClosed(false);

    QFile styleFile(":/styles/styles.qss");
    if (styleFile.open(QIODevice::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
        styleFile.close();
    }

    return a.exec();
}
