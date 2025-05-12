#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>
#include <QTableWidget>
#include <QVector>
#include <QStandardItemModel>

// Forward declaration
class AddIem;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private slots:
<<<<<<< HEAD
    void updateInventoryTable();
    void openAddItemForm();
    void removeSelectedItem();      
    void searchInventory();         

public slots:
    void onItemAdded();             
private:
    Ui::Form *ui;
    AddIem *addItemForm = nullptr;
    QStandardItemModel *model;

    void setupTable();
    void setTableHeaders();
=======
    void updateInventoryTable();    // Method to refresh inventory display
    void openAddItemForm();         // Method to open the add item form
    void removeSelectedItem();      // Method to remove selected item
    void searchInventory();         // Method to search/filter inventory

public slots:
    void onItemAdded();             // Called when an item is added via AddIem

private:
    Ui::Form *ui;
    AddIem *addItemForm = nullptr;  // Pointer to the add item form
    QStandardItemModel *model;      // Model for the table view

    void setupTable();              // Helper to set up the table view
    void setTableHeaders();         // Helper to set table column headers
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
};

#endif // INVENTORY_H
