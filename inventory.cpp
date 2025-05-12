#include "inventory.h"
#include "ui_inventory.h"
#include "addiem.h"
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
    this->setWindowTitle("Inventory Management");

<<<<<<< HEAD

    QVBoxLayout *mainLayout = new QVBoxLayout(this);


=======
    // Create the UI elements for the inventory page
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Top section with search bar
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("Search:", this);
    QLineEdit *searchField = new QLineEdit(this);
    QPushButton *searchButton = new QPushButton("Search", this);

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchField);
    searchLayout->addWidget(searchButton);
    mainLayout->addLayout(searchLayout);

<<<<<<< HEAD

=======
    // Table for inventory items
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    QTableWidget *inventoryTable = new QTableWidget(this);
    inventoryTable->setColumnCount(5);
    QStringList headers;
    headers << "Item Name" << "Category" << "Quantity" << "Price" << "Supplier";
    inventoryTable->setHorizontalHeaderLabels(headers);
    inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(inventoryTable);

<<<<<<< HEAD

=======
    // Buttons for various actions
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("Add New Item", this);
    QPushButton *removeButton = new QPushButton("Remove Selected Item", this);
    QPushButton *refreshButton = new QPushButton("Refresh Table", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(refreshButton);
    mainLayout->addLayout(buttonLayout);

<<<<<<< HEAD

=======
    // Store references to UI elements
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    inventoryTable->setObjectName("inventoryTable");
    searchField->setObjectName("searchField");
    searchButton->setObjectName("searchButton");
    addButton->setObjectName("addButton");
    removeButton->setObjectName("removeButton");
    refreshButton->setObjectName("refreshButton");

<<<<<<< HEAD
=======
    // Connect signals and slots
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    connect(addButton, &QPushButton::clicked, this, &Form::openAddItemForm);
    connect(removeButton, &QPushButton::clicked, this, &Form::removeSelectedItem);
    connect(refreshButton, &QPushButton::clicked, this, &Form::updateInventoryTable);
    connect(searchButton, &QPushButton::clicked, this, &Form::searchInventory);

<<<<<<< HEAD
=======
    // Initial table population
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    updateInventoryTable();
}

Form::~Form()
{
    delete ui;
    if(addItemForm)
        addItemForm->deleteLater();
}

void Form::updateInventoryTable()
{
<<<<<<< HEAD

    QTableWidget *inventoryTable = findChild<QTableWidget*>("inventoryTable");
    if (!inventoryTable) return;


    QVector<InventoryItem>& items = AddIem::getInventoryItems();


    inventoryTable->setRowCount(0);


    for (int i = 0; i < items.size(); ++i) {
        inventoryTable->insertRow(i);

=======
    // Get the table widget
    QTableWidget *inventoryTable = findChild<QTableWidget*>("inventoryTable");
    if (!inventoryTable) return;

    // Get the inventory items
    QVector<InventoryItem>& items = AddIem::getInventoryItems();

    // Clear the table
    inventoryTable->setRowCount(0);

    // Populate the table with inventory items
    for (int i = 0; i < items.size(); ++i) {
        inventoryTable->insertRow(i);

        // Create table items
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
        QTableWidgetItem *nameItem = new QTableWidgetItem(items[i].name);
        QTableWidgetItem *categoryItem = new QTableWidgetItem(items[i].category);
        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(items[i].quantity));
        QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$%1").arg(items[i].price, 0, 'f', 2));
        QTableWidgetItem *supplierItem = new QTableWidgetItem(items[i].supplier);

<<<<<<< HEAD
=======
        // Set items as not editable
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        categoryItem->setFlags(categoryItem->flags() & ~Qt::ItemIsEditable);
        quantityItem->setFlags(quantityItem->flags() & ~Qt::ItemIsEditable);
        priceItem->setFlags(priceItem->flags() & ~Qt::ItemIsEditable);
        supplierItem->setFlags(supplierItem->flags() & ~Qt::ItemIsEditable);

<<<<<<< HEAD

=======
        // Add items to the table
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
        inventoryTable->setItem(i, 0, nameItem);
        inventoryTable->setItem(i, 1, categoryItem);
        inventoryTable->setItem(i, 2, quantityItem);
        inventoryTable->setItem(i, 3, priceItem);
        inventoryTable->setItem(i, 4, supplierItem);
    }
}

void Form::openAddItemForm()
{
    if (!addItemForm) {
        addItemForm = new AddIem();
        addItemForm->setInventory(this);
        connect(addItemForm, &AddIem::itemAdded, this, &Form::onItemAdded);
        connect(addItemForm, &AddIem::backToInventory, this, [this]() {
            this->activateWindow();
            updateInventoryTable();
        });
    }

    addItemForm->show();
    addItemForm->activateWindow();
}

void Form::removeSelectedItem()
{
    QTableWidget *inventoryTable = findChild<QTableWidget*>("inventoryTable");
    if (!inventoryTable) return;

    QModelIndexList selectedRows = inventoryTable->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select an item to remove.");
        return;
    }

<<<<<<< HEAD

=======
    // Sort indices in descending order to avoid shifting issues when removing
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    QVector<int> rowsToRemove;
    for (const QModelIndex &index : selectedRows) {
        rowsToRemove.append(index.row());
    }

    std::sort(rowsToRemove.begin(), rowsToRemove.end(), std::greater<int>());

<<<<<<< HEAD
=======
    // Remove the selected items from the data structure
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    QVector<InventoryItem>& items = AddIem::getInventoryItems();
    for (int row : rowsToRemove) {
        if (row < items.size()) {
            items.remove(row);
        }
    }

<<<<<<< HEAD
=======
    // Update the table display
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    updateInventoryTable();

    QMessageBox::information(this, "Success", "Selected item(s) removed successfully.");
}

void Form::searchInventory()
{
    QLineEdit *searchField = findChild<QLineEdit*>("searchField");
    QTableWidget *inventoryTable = findChild<QTableWidget*>("inventoryTable");

    if (!searchField || !inventoryTable) return;

    QString searchText = searchField->text().trimmed().toLower();

<<<<<<< HEAD
=======
    // If search field is empty, show all items
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    if (searchText.isEmpty()) {
        updateInventoryTable();
        return;
    }

<<<<<<< HEAD

    QVector<InventoryItem>& items = AddIem::getInventoryItems();


    inventoryTable->setRowCount(0);


    int row = 0;
    for (const InventoryItem &item : items) {

=======
    // Get the inventory items
    QVector<InventoryItem>& items = AddIem::getInventoryItems();

    // Clear the table
    inventoryTable->setRowCount(0);

    // Add only matching items to the table
    int row = 0;
    for (const InventoryItem &item : items) {
        // Search in all fields
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
        if (item.name.toLower().contains(searchText) ||
            item.category.toLower().contains(searchText) ||
            QString::number(item.quantity).contains(searchText) ||
            QString::number(item.price).contains(searchText) ||
            item.supplier.toLower().contains(searchText)) {

            inventoryTable->insertRow(row);

<<<<<<< HEAD
=======
            // Create table items
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
            QTableWidgetItem *nameItem = new QTableWidgetItem(item.name);
            QTableWidgetItem *categoryItem = new QTableWidgetItem(item.category);
            QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(item.quantity));
            QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$%1").arg(item.price, 0, 'f', 2));
            QTableWidgetItem *supplierItem = new QTableWidgetItem(item.supplier);

<<<<<<< HEAD

=======
            // Set items as not editable
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            categoryItem->setFlags(categoryItem->flags() & ~Qt::ItemIsEditable);
            quantityItem->setFlags(quantityItem->flags() & ~Qt::ItemIsEditable);
            priceItem->setFlags(priceItem->flags() & ~Qt::ItemIsEditable);
            supplierItem->setFlags(supplierItem->flags() & ~Qt::ItemIsEditable);

<<<<<<< HEAD

=======
            // Add items to the table
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
            inventoryTable->setItem(row, 0, nameItem);
            inventoryTable->setItem(row, 1, categoryItem);
            inventoryTable->setItem(row, 2, quantityItem);
            inventoryTable->setItem(row, 3, priceItem);
            inventoryTable->setItem(row, 4, supplierItem);

            row++;
        }
    }
}

void Form::onItemAdded()
{
<<<<<<< HEAD

=======
    // Update the table when an item is added
>>>>>>> 48dc7089a08777127ebae50b51e8be5d3795bacc
    updateInventoryTable();
}
