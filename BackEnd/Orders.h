#ifndef ORDERS_H
#define ORDERS_H

#include "DataBase/DataBaseApi.h"
#include <QCalendarWidget>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QTextEdit>

#include "ui_mainwindow.h"

namespace BackEnd {

class Orders : QObject
{
    Q_OBJECT
    QTableWidget&            mOrders;
    QPushButton&             mWyswietlButton;
    QCalendarWidget&         mCalendar;
    QTextEdit&               mAdditionalInformation;
    DataBaseApi::DataBaseApi mDataBaseApi;
    QPushButton&             mAddOrderButton;

    QDate mSelectedDate;

    void printOrders();
    void printCalculatedOrder();

public:
    Orders(Ui::MainWindow* ui, DataBaseApi::DataBaseApi& dataBaseApi);

    void printEmptyTable(void);

private slots:
    void wyswietlPressed();
    void chosenDateChanged();
    void addOrderPressed();
};

} // namespace BackEnd

#endif // ORDERS_H
