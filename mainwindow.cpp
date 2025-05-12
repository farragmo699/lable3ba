#include "mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    userIsAdmin(false)
{
}

MainWindow::~MainWindow()
{
}

void MainWindow::setUserType(bool isAdmin)
{
    userIsAdmin = isAdmin;
    setWindowTitle(userIsAdmin ? "Admin Dashboard" : "User Dashboard");
}

void MainWindow::setUsername(const QString &username)
{
    currentUsername = username;
    if (!currentUsername.isEmpty()) {
        setWindowTitle(windowTitle() + " - " + currentUsername);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Logout", "Are you sure you want to logout?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit logoutRequested();
        event->accept();
    } else {
        event->ignore();
    }
}
