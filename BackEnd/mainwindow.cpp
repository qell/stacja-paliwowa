#include "mainwindow.h"
#include "ClientsTab.h"
#include "FuelPriceChart.h"
#include "FuelTanks.h"
#include "Orders.h"
#include "SuggestedProducts.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QScopedPointer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeObjects()
{
    qDebug() << __PRETTY_FUNCTION__;

    // making objects
    mOrders.reset(new BackEnd::Orders(ui, mDataBaseApi));
    mFuelPriceChart.reset(new BackEnd::FuelPriceChart(ui, mDataBaseApi));
    mFuelTanks.reset(new BackEnd::FuelTanks(ui, mDataBaseApi));
    mClientsTab.reset(new BackEnd::ClientsTab(ui, mDataBaseApi));
    mSuggestedProducts.reset(new BackEnd::SuggestedProducts(ui, mDataBaseApi));

    mFuelTanks.get()->displayTanksFillLevel();
}
