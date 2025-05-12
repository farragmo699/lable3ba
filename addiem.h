#ifndef ADDIEM_H
#define ADDIEM_H

#include <QWidget>
#include <QMessageBox>
#include <QVector>

// Structure to hold item data
struct InventoryItem {
    QString name;
    QString category;
    int quantity;
    double price;
    QString supplier;
};

// Forward declarations
namespace Ui {
class AddIem; // Matches the class name in the UI file
}

class Form; // Forward declaration of Inventory class (Form)

class AddIem : public QWidget
{
    Q_OBJECT

public:
    explicit AddIem(QWidget *parent = nullptr);
    ~AddIem();

    // Method to set the parent inventory
    void setInventory(Form *inventory);

    // Static method to access the inventory items
    static QVector<InventoryItem>& getInventoryItems();

private slots:
    void on_pushButton_clicked();       // Submit button
    void on_pushButton_2_clicked();     // Clear form button
    void on_pushButton_3_clicked();     // Back to Inventory button
    void on_buttonBox_accepted();       // OK button
    void on_buttonBox_rejected();       // Cancel button

signals:
    void backToInventory();             // Signal to switch back to inventory view
    void itemAdded();                   // Signal to notify that an item was added

private:
    Ui::AddIem *ui;
    Form *parentInventory;              // Pointer to the parent inventory
    static QVector<InventoryItem> inventoryItems; // Shared inventory data

    void clearForm();                   // Helper method to clear all form fields
    bool validateForm();                // Helper method to validate form inputs
    void addItemToInventory();          // Helper method to add item to data structure
};

#endif // ADDIEM_H
