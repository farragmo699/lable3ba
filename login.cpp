#include "login.h"
#include "adminform.h"
#include "dashboard.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->passwordField->setEchoMode(QLineEdit::Password);

    loadUsers();

    if (users.isEmpty()) {
        users["admin"] = qMakePair("Admin@123", "admin");
        saveUsers();
        qDebug() << "Created default admin account";
    }
}

Login::~Login()
{
    delete ui;
}

void Login::loadUsers()
{
    QFile file(userFile);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open users file for reading";
        return;
    }

    users.clear();
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(":");
        if (parts.size() == 3) {
            users[parts[0]] = qMakePair(parts[1], parts[2]);
        }
    }

    file.close();
    qDebug() << "Loaded" << users.size() << "users";
}

void Login::saveUsers()
{
    QFile file(userFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open users file for writing";
        return;
    }

    QTextStream out(&file);

    for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
        out << it.key() << ":" << it.value().first << ":" << it.value().second << "\n";
    }

    file.close();
    qDebug() << "Saved" << users.size() << "users";
}

bool Login::validatePassword(const QString &password)
{
    QRegularExpression regex("^(?=.*\\d).{8,}$");
    return regex.match(password).hasMatch();
}

void Login::on_loginButton_clicked()
{
    QString username = ui->usernameField->text().trimmed();
    QString password = ui->passwordField->text();

    // Input validation
    if (username.isEmpty() || password.isEmpty()) {
        showError("Please enter both username and password");
        return;
    }

    if (!users.contains(username)) {
        showError("Username not found");
        return;
    }

    if (users[username].first != password) {
        showError("Incorrect password");
        return;
    }

    qDebug() << "Login successful for:" << username << "role:" << users[username].second;

    ui->usernameField->clear();
    ui->passwordField->clear();

    QWidget* dashboard = nullptr;
    if (users[username].second == "admin") {
        dashboard = new AdminForm();
    } else {
        dashboard = new Dashboard();
    }

    // Connect dashboard's closed signal using windowClosed() signal
    if (users[username].second == "admin") {
        connect(static_cast<AdminForm*>(dashboard), &AdminForm::windowClosed, this, [this]() {
            this->show();
            this->activateWindow();
        });
    } else {
        connect(static_cast<Dashboard*>(dashboard), &Dashboard::windowClosed, this, [this]() {
            this->show();
            this->activateWindow();
        });
    }

    // Set attribute to delete when closed
    dashboard->setAttribute(Qt::WA_DeleteOnClose);

    // Show dashboard and hide login
    dashboard->show();
    this->hide();
}
void Login::on_signupButton_clicked()
{
    QString username = ui->usernameField->text().trimmed();
    QString password = ui->passwordField->text();

    if (username.isEmpty() || password.isEmpty()) {
        showError("Please enter both username and password");
        return;
    }

    if (!validatePassword(password)) {
        showError("Password must be at least 8 characters and contain numbers");
        return;
    }

    if (users.contains(username)) {
        showError("Username already exists");
        return;
    }

    users[username] = qMakePair(password, "staff");
    saveUsers();

    showSuccess("Account created successfully!");
    ui->usernameField->clear();
    ui->passwordField->clear();
}

void Login::showError(const QString &message)
{
    QMessageBox::critical(this, "Error", message);
}

void Login::showSuccess(const QString &message)
{
    QMessageBox::information(this, "Success", message);
}
