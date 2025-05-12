#ifndef ADMINFORM_H
#define ADMINFORM_H

#include <QWidget>
#include <QCloseEvent>  // Add this include

namespace Ui {
class AdminForm;
}

class AdminForm : public QWidget
{
    Q_OBJECT

public:
    explicit AdminForm(QWidget *parent = nullptr);
    ~AdminForm();

signals:
    void windowClosed();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_addUserButton_clicked();
    void on_removeUserButton_clicked();
    void on_backButton_clicked();
    void refreshUsersList();

private:
    Ui::AdminForm *ui;
    const QString userFile = "users.txt";

    QMap<QString, QPair<QString, QString>> loadUsersFromFile();
    bool saveUsersToFile(const QMap<QString, QPair<QString, QString>>& users);
    bool validatePassword(const QString &password);
    void clearInputs();
};

#endif // ADMINFORM_H
