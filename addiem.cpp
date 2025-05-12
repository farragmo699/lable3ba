#include "addiem.h"
#include "ui_addiem.h" // This will be auto-generated from the .ui file
#include "inventory.h"

// Initialize static member
QVector<InventoryItem> AddIem::inventoryItems;

AddIem::AddIem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddIem),
    parentInventory(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("Add New Item");

    // Set up the category combo box with some default categories
    ui->comboBox->addItem("Electronics");
    ui->comboBox->addItem("Furniture");
    ui->comboBox->addItem("Office Supplies");
    ui->comboBox->addItem("Food");
    ui->comboBox->addItem("Clothing");
    ui->comboBox->addItem("Other");

    // Set defaults for numeric fields
    ui->spinBox->setMinimum(1);         // Minimum quantity is 1
    ui->spinBox->setValue(1);           // Default quantity is 1
    ui->doubleSpinBox->setMinimum(0.0); // Minimum price is 0
    ui->doubleSpinBox->setMaximum(9999.99); // Maximum price
    ui->doubleSpinBox->setValue(0.0);   // Default price is 0
    ui->doubleSpinBox->setPrefix("$");  // Add dollar sign prefix
    ui->doubleSpinBox->setDecimals(2);  // Show 2 decimal places

    // Connect dialog buttons
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddIem::on_buttonBox_accepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddIem::on_buttonBox_rejected);
}

AddIem::~AddIem()
{
    delete ui;
}

void AddIem::setInventory(Form *inventory)
{
    parentInventory = inventory;
}

QVector<InventoryItem>& AddIem::getInventoryItems()
{
    return inventoryItems;
}

void AddIem::on_pushButton_clicked()
{
    // Submit button clicked
    if (validateForm()) {
        addItemToInventory();
        QMessageBox::information(this, "Success", "Item added successfully!");
        emit itemAdded(); // Emit signal that an item was added
        clearForm();      // Clear the form for the next entry
    }
}

void AddIem::on_pushButton_2_clicked()
{
    // Clear form button clicked
    clearForm();
}

void AddIem::on_pushButton_3_clicked()
{
    // Back to Inventory button clicked
    emit backToInventory(); // Emit signal to go back to inventory view
    this->close();         // Close the Add Item window
}

void AddIem::on_buttonBox_accepted()
{
    // OK button clicked - same functionality as Submit
    on_pushButton_clicked();
}

void AddIem::on_buttonBox_rejected()
{
    // Cancel button clicked - same as going back to inventory
    emit backToInventory();
    this->close();
}

void AddIem::clearForm()
{
    // Clear all form fields and reset to defaults
    ui->lineEdit->clear();              // Clear item name
    ui->comboBox->setCurrentIndex(0);   // Reset category to first item
    ui->spinBox->setValue(1);           // Reset quantity to 1
    ui->doubleSpinBox->setValue(0.0);   // Reset price to 0
    ui->lineEdit_2->clear();            // Clear supplier
}

bool AddIem::validateForm()
{
    // Validate that required fields are filled out
    if (ui->lineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter an item name.");
        ui->lineEdit->setFocus();
        return false;
    }

    // Validate price (optional)
    if (ui->doubleSpinBox->value() <= 0.0) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid price (greater than $0.00).");
        ui->doubleSpinBox->setFocus();
        return false;
    }

    // Validate supplier (optional)
    if (ui->lineEdit_2->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a supplier name.");
        ui->lineEdit_2->setFocus();
        return false;
    }

    return true;
}

void AddIem::addItemToInventory()
{
    // Create a new item structure and fill it with form data
    InventoryItem newItem;
    newItem.name = ui->lineEdit->text().trimmed();
    newItem.category = ui->comboBox->currentText();
    newItem.quantity = ui->spinBox->value();
    newItem.price = ui->doubleSpinBox->value();
    newItem.supplier = ui->lineEdit_2->text().trimmed();

    // Add the item to our inventory data structure
    inventoryItems.append(newItem);
}
