#include "adminform.h"
#include "ui_adminform.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QListWidgetItem>
#include <QDebug>
#include <QRegularExpression>

AdminForm::AdminForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminForm)
{
    ui->setupUi(this);

    ui->roleComboBox->addItem("Manager", "manager");
    ui->roleComboBox->addItem("Staff", "staff");

    refreshUsersList();
}

AdminForm::~AdminForm()
{
    qDebug() << "AdminForm destroyed";
    delete ui;
}

void AdminForm::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    event->accept();
}

QMap<QString, QPair<QString, QString>> AdminForm::loadUsersFromFile()
{
    QMap<QString, QPair<QString, QString>> users;
    QFile file(userFile);

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(":");
            if (parts.size() == 3) {
                users[parts[0]] = qMakePair(parts[1], parts[2]);
            }
        }
        file.close();
    }
    return users;
}

bool AdminForm::saveUsersToFile(const QMap<QString, QPair<QString, QString>>& users)
{
    QFile file(userFile);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QTextStream out(&file);
    for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
        out << it.key() << ":" << it.value().first << ":" << it.value().second << "\n";
    }
    file.close();
    return true;
}

bool AdminForm::validatePassword(const QString &password)
{
    QRegularExpression regex("^(?=.*\\d).{8,}$");
    return regex.match(password).hasMatch();
}

void AdminForm::refreshUsersList()
{
    ui->usersListWidget->clear();
    QMap<QString, QPair<QString, QString>> users = loadUsersFromFile();

    for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
        QListWidgetItem *item = new QListWidgetItem(
            QString("%1 (%2)").arg(it.key(), it.value().second));
        item->setData(Qt::UserRole, it.key());
        ui->usersListWidget->addItem(item);
    }
}

void AdminForm::on_addUserButton_clicked()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString role = ui->roleComboBox->currentData().toString();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill all fields");
        return;
    }

    if (!validatePassword(password)) {
        QMessageBox::warning(this, "Error",
                             "Password must be at least 8 characters and contain numbers");
        return;
    }

    QMap<QString, QPair<QString, QString>> users = loadUsersFromFile();
    if (users.contains(username)) {
        QMessageBox::warning(this, "Error", "Username already exists");
        return;
    }

    users[username] = qMakePair(password, role);
    if (saveUsersToFile(users)) {
        QMessageBox::information(this, "Success", "User added successfully");
        refreshUsersList();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save users");
    }
}

void AdminForm::on_removeUserButton_clicked()
{
    QListWidgetItem *item = ui->usersListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Error", "Please select a user to remove");
        return;
    }

    QString username = item->data(Qt::UserRole).toString();
    if (username == "admin") {
        QMessageBox::warning(this, "Error", "Cannot delete admin account");
        return;
    }

    QMap<QString, QPair<QString, QString>> users = loadUsersFromFile();
    if (users.remove(username) > 0) {
        if (saveUsersToFile(users)) {
            QMessageBox::information(this, "Success", "User removed successfully");
            refreshUsersList();
        } else {
            QMessageBox::critical(this, "Error", "Failed to save changes");
        }
    } else {
        QMessageBox::warning(this, "Error", "User not found");
    }
}

void AdminForm::on_backButton_clicked()
{
    this->close();
}

void AdminForm::clearInputs()
{
    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->roleComboBox->setCurrentIndex(0);
}
