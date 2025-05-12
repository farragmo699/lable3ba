#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMap>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_loginButton_clicked();
    void on_signupButton_clicked();
signals:
    void adminLoginSuccess();
    void userLoginSuccess();
    void windowClosed();
private:
    Ui::Login *ui;
    QMap<QString, QPair<QString, QString>> users;
    const QString userFile = "users.txt";

    void loadUsers();
    void saveUsers();
    bool validatePassword(const QString &password);
    void showError(const QString &message);
    void showSuccess(const QString &message);
};

#endif // LOGIN_H
