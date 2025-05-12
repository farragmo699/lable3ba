#include "dashboard.h"
#include "ui_dashboard.h"
#include <QCloseEvent>
#include "addiem.h"  // For accessing inventory items

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    this->setWindowTitle("Staff Dashboard");

    // Create labels to show actual values
    QLabel *totalItemsValueLabel = new QLabel(ui->frame);
    totalItemsValueLabel->setObjectName("totalItemsValueLabel");
    totalItemsValueLabel->setGeometry(200, 20, 300, 41);
    totalItemsValueLabel->setFont(ui->label_2->font());

    QLabel *cardValueLabel = new QLabel(ui->frame);
    cardValueLabel->setObjectName("cardValueLabel");
    cardValueLabel->setGeometry(200, 130, 300, 41);
    cardValueLabel->setFont(ui->label_3->font());

    // Update welcome message and summary cards when dashboard opens
    updateDashboardSummary();
}

Dashboard::~Dashboard()
{
    delete ui;
    if(reportsWindow) reportsWindow->deleteLater();
    if(addItemWindow) addItemWindow->deleteLater();
    if(inventoryWindow) inventoryWindow->deleteLater();
}

void Dashboard::setUsername(const QString &username)
{
    currentUsername = username;
    updateDashboardSummary();
}

void Dashboard::updateDashboardSummary()
{
    // Personalized welcome message
    QString welcomeMessage = "Welcome, " +
                             (currentUsername.isEmpty() ? "User" : currentUsername) + "!";
    ui->label->setText(welcomeMessage);

    // Find the labels we created dynamically
    QLabel *totalItemsValueLabel = ui->frame->findChild<QLabel*>("totalItemsValueLabel");
    QLabel *cardValueLabel = ui->frame->findChild<QLabel*>("cardValueLabel");

    if (!totalItemsValueLabel || !cardValueLabel) return;

    // Get inventory items
    QVector<InventoryItem>& items = AddIem::getInventoryItems();

    // Calculate summary statistics
    int totalItems = 0;
    double totalValue = 0.0;

    for (const auto& item : items) {
        totalItems += item.quantity;
        totalValue += item.price * item.quantity;
    }

    // Update summary cards
    totalItemsValueLabel->setText(QString::number(totalItems));
    cardValueLabel->setText(QString("$%1").arg(
        QString::number(totalValue, 'f', 2)
        ));
}

void Dashboard::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    event->accept();
}

void Dashboard::setupWindow(QWidget *window)
{
    window->setAttribute(Qt::WA_DeleteOnClose, false); // Don't delete the window on close
    window->setWindowModality(Qt::ApplicationModal);
    window->show();
}

void Dashboard::on_viewInventoryButton_clicked()
{
    if(!inventoryWindow) {
        inventoryWindow = new Form();
        connect(inventoryWindow, &Form::destroyed, this, [this]() {
            inventoryWindow = nullptr;
        });
    }
    setupWindow(inventoryWindow);
    inventoryWindow->activateWindow();
}

void Dashboard::on_addNewItemButton_clicked()
{
    if(!addItemWindow) {
        addItemWindow = new AddIem();
        connect(addItemWindow, &AddIem::destroyed, this, [this]() {
            addItemWindow = nullptr;
        });

        // Connect the backToInventory signal to show the inventory window
        connect(addItemWindow, &AddIem::backToInventory, this, [this]() {
            if(inventoryWindow) {
                inventoryWindow->activateWindow();
            } else {
                on_viewInventoryButton_clicked();
            }
            // Update summary after adding new item
            updateDashboardSummary();
        });

        // Update summary when an item is added
        connect(addItemWindow, &AddIem::itemAdded, this, &Dashboard::updateDashboardSummary);
    }
    setupWindow(addItemWindow);
    addItemWindow->activateWindow();
}

void Dashboard::on_generateReportsButton_clicked()
{
    if(!reportsWindow) {
        reportsWindow = new Reports();
        connect(reportsWindow, &Reports::destroyed, this, [this]() {
            reportsWindow = nullptr;
        });
        connect(reportsWindow, &Reports::backToDashboard, this, [this]() {
            this->activateWindow();
        });
    }
    setupWindow(reportsWindow);
    reportsWindow->activateWindow();
}
