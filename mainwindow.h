#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUserType(bool isAdmin);
    void setUsername(const QString &username);

signals:
    void logoutRequested();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    bool userIsAdmin;
    QString currentUsername;
};

#endif // MAINWINDOW_H
