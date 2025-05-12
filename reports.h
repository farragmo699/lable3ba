#ifndef REPORTS_H
#define REPORTS_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "addiem.h" // To access inventory data

namespace Ui {
class Reports;
}

class Reports : public QWidget
{
    Q_OBJECT

public:
    explicit Reports(QWidget *parent = nullptr);
    ~Reports();

signals:
    void backToDashboard();

private slots:
    void on_radioButton_clicked();      // Full Inventory Report
    void on_radioButton_2_clicked();    // Low-Stock Alert Report
    void on_pushButton_clicked();       // Export as TXT
    void on_pushButton_2_clicked();     // Export as CSV
    void on_pushButton_3_clicked();     // Back to Dashboard

private:
    Ui::Reports *ui;
    enum ReportType { FULL_INVENTORY, LOW_STOCK };
    ReportType currentReportType;

    QString generateFullReport();       // Generate full inventory report
    QString generateLowStockReport();   // Generate low-stock alert report
    void generateCurrentReport();       // Generate current selected report
    bool exportReportToFile(const QString &format);  // Export report to file
    double calculateTotalInventoryValue(); // Calculate total inventory value
};

#endif // REPORTS_H
