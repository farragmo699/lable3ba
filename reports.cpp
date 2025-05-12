#include "reports.h"
#include "ui_reports.h"
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>

Reports::Reports(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reports)
    , currentReportType(FULL_INVENTORY)
{
    ui->setupUi(this);
    this->setWindowTitle("Inventory Reports");

    // Set default report type and generate initial report
    ui->radioButton->setChecked(true);
    currentReportType = FULL_INVENTORY;
    generateCurrentReport();

    // Connect buttons to slots
    connect(ui->radioButton, &QRadioButton::clicked, this, &Reports::on_radioButton_clicked);
    connect(ui->radioButton_2, &QRadioButton::clicked, this, &Reports::on_radioButton_2_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &Reports::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Reports::on_pushButton_2_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Reports::on_pushButton_3_clicked);
}

Reports::~Reports()
{
    delete ui;
}

void Reports::on_radioButton_clicked()
{
    // Full Inventory Report selected
    currentReportType = FULL_INVENTORY;
    generateCurrentReport();
}

void Reports::on_radioButton_2_clicked()
{
    // Low-Stock Alert Report selected
    currentReportType = LOW_STOCK;
    generateCurrentReport();
}

void Reports::on_pushButton_clicked()
{
    // Export as TXT
    exportReportToFile("txt");
}

void Reports::on_pushButton_2_clicked()
{
    // Export as CSV
    exportReportToFile("csv");
}

void Reports::on_pushButton_3_clicked()
{
    // Back to Dashboard
    emit backToDashboard();
    this->close();
}

void Reports::generateCurrentReport()
{
    QString reportContent;

    switch (currentReportType) {
    case FULL_INVENTORY:
        reportContent = generateFullReport();
        break;
    case LOW_STOCK:
        reportContent = generateLowStockReport();
        break;
    }

    ui->textEdit->setText(reportContent);
}

QString Reports::generateFullReport()
{
    QString report;
    QTextStream stream(&report);
    QVector<InventoryItem>& items = AddIem::getInventoryItems();

    // Get current date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    // Report header
    stream << "FULL INVENTORY REPORT\n";
    stream << "Generated on: " << dateTimeString << "\n";
    stream << "----------------------------------------\n\n";

    // Report items
    if (items.isEmpty()) {
        stream << "No items in inventory.\n";
    } else {
        double totalValue = calculateTotalInventoryValue();

        stream << "Total Items: " << items.size() << "\n";
        stream << "Total Inventory Value: $" << QString::number(totalValue, 'f', 2) << "\n\n";

        stream << "ITEM DETAILS:\n";
        stream << "----------------------------------------\n";

        for (int i = 0; i < items.size(); ++i) {
            const InventoryItem& item = items[i];
            double itemTotalValue = item.price * item.quantity;

            stream << i+1 << ". " << item.name << "\n";
            stream << "   Category: " << item.category << "\n";
            stream << "   Quantity: " << item.quantity << "\n";
            stream << "   Price: $" << QString::number(item.price, 'f', 2) << "\n";
            stream << "   Total Value: $" << QString::number(itemTotalValue, 'f', 2) << "\n";
            stream << "   Supplier: " << item.supplier << "\n";
            stream << "----------------------------------------\n";
        }
    }

    return report;
}

QString Reports::generateLowStockReport()
{
    QString report;
    QTextStream stream(&report);
    QVector<InventoryItem>& items = AddIem::getInventoryItems();

    // Get current date and time
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    // Define low stock threshold (items with quantity <= 5)
    const int LOW_STOCK_THRESHOLD = 5;

    // Report header
    stream << "LOW-STOCK ALERT REPORT\n";
    stream << "Generated on: " << dateTimeString << "\n";
    stream << "----------------------------------------\n\n";

    // Filter and report low stock items
    QVector<InventoryItem> lowStockItems;
    for (const InventoryItem& item : items) {
        if (item.quantity <= LOW_STOCK_THRESHOLD) {
            lowStockItems.append(item);
        }
    }

    if (lowStockItems.isEmpty()) {
        stream << "No items with low stock (quantity <= " << LOW_STOCK_THRESHOLD << ").\n";
    } else {
        stream << "Low Stock Items (quantity <= " << LOW_STOCK_THRESHOLD << "): " << lowStockItems.size() << "\n\n";
        stream << "ITEM DETAILS:\n";
        stream << "----------------------------------------\n";

        for (int i = 0; i < lowStockItems.size(); ++i) {
            const InventoryItem& item = lowStockItems[i];

            stream << i+1 << ". " << item.name << " - QUANTITY: " << item.quantity << "\n";
            stream << "   Category: " << item.category << "\n";
            stream << "   Price: $" << QString::number(item.price, 'f', 2) << "\n";
            stream << "   Supplier: " << item.supplier << "\n";
            stream << "   ACTION REQUIRED: Reorder from supplier\n";
            stream << "----------------------------------------\n";
        }
    }

    return report;
}

bool Reports::exportReportToFile(const QString &format)
{
    // Get the appropriate file extension based on format
    QString fileExtension = format.toLower();
    QString filter = format.toUpper() + " Files (*." + fileExtension + ")";

    // Get the appropriate default file name based on report type
    QString defaultFileName = (currentReportType == FULL_INVENTORY) ? "Full_Inventory_Report" : "Low_Stock_Report";
    defaultFileName += "_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "." + fileExtension;

    // Get the directory for Documents
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultPath = documentsPath + QDir::separator() + defaultFileName;

    // Show file dialog to get the filename and location
    QString fileName = QFileDialog::getSaveFileName(this, "Export Report", defaultPath, filter);

    if (fileName.isEmpty()) {
        return false; // User canceled
    }

    // Make sure the file has the correct extension
    if (!fileName.endsWith("." + fileExtension, Qt::CaseInsensitive)) {
        fileName += "." + fileExtension;
    }

    // Open the file for writing
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
        return false;
    }

    QTextStream out(&file);

    // For CSV, we need special formatting
    if (fileExtension == "csv") {
        QVector<InventoryItem>& items = AddIem::getInventoryItems();

        // CSV Header
        out << "Item Name,Category,Quantity,Price,Total Value,Supplier\n";

        // If we're doing a low-stock report, filter the items
        if (currentReportType == LOW_STOCK) {
            const int LOW_STOCK_THRESHOLD = 5;

            for (const InventoryItem& item : items) {
                if (item.quantity <= LOW_STOCK_THRESHOLD) {
                    double totalValue = item.price * item.quantity;
                    out << "\"" << item.name << "\",\"" << item.category << "\","
                        << item.quantity << "," << QString::number(item.price, 'f', 2) << ","
                        << QString::number(totalValue, 'f', 2) << ",\"" << item.supplier << "\"\n";
                }
            }
        } else {
            // Full inventory report
            for (const InventoryItem& item : items) {
                double totalValue = item.price * item.quantity;
                out << "\"" << item.name << "\",\"" << item.category << "\","
                    << item.quantity << "," << QString::number(item.price, 'f', 2) << ","
                    << QString::number(totalValue, 'f', 2) << ",\"" << item.supplier << "\"\n";
            }
        }
    } else {
        // For TXT files, just use the same format as shown in the text edit
        out << ui->textEdit->toPlainText();
    }

    file.close();

    QMessageBox::information(this, "Export Successful",
                             "The report has been successfully exported to:\n" + fileName);

    return true;
}

double Reports::calculateTotalInventoryValue()
{
    QVector<InventoryItem>& items = AddIem::getInventoryItems();
    double totalValue = 0.0;

    for (const InventoryItem& item : items) {
        totalValue += (item.price * item.quantity);
    }

    return totalValue;
}








