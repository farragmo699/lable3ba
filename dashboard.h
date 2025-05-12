#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include "reports.h"
#include "addiem.h"
#include "inventory.h"

namespace Ui {
class Dashboard;
}

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    // New method to set username
    void setUsername(const QString &username);

signals:
    void windowClosed();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_viewInventoryButton_clicked();
    void on_addNewItemButton_clicked();
    void on_generateReportsButton_clicked();

    // New method to update dashboard summary
    void updateDashboardSummary();

private:
    Ui::Dashboard *ui;
    Reports *reportsWindow = nullptr;
    AddIem *addItemWindow = nullptr;
    Form *inventoryWindow = nullptr;

    // New member to store current username
    QString currentUsername;

    void setupWindow(QWidget *window);
};

#endif // DASHBOARD_H
