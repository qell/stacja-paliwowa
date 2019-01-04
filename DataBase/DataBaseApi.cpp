#include "DataBaseApi.h"

namespace DataBaseApi {
DataBaseApi::DataBaseApi()
{
    driverDatabase = QSqlDatabase::addDatabase(driverName);

    QString absolutePathDatabase = QDir::currentPath().split("build")[0] + pathDatabase;
    qDebug() << absolutePathDatabase;
    driverDatabase.setDatabaseName(absolutePathDatabase);

    // Checking DB connection
    if (driverDatabase.open() == false)
    {
        qDebug() << "Error: connection with database fail";
    }
}

DataBaseApi::~DataBaseApi()
{
    driverDatabase.close();
}

QMap<Common::FuelTankType, uint32_t> DataBaseApi::getTanksFillLevel(void)
{
    QMap<Common::FuelTankType, uint32_t> data;
    QSqlQuery                            q;

    q.prepare("SELECT * FROM Magazyny ");

    if (q.exec())
    {
        while (q.next())
        {
            data.insert(Common::getFuelTankEnum(q.value("Zbiornik").toString()), q.value("Zawartosc").toUInt());
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    return data;
}

QVector<Common::OrdersStruct> DataBaseApi::getOrdersByDate(QDate date)
{
    QVector<Common::OrdersStruct> data;
    QSqlQuery                     q;

    q.prepare(QString("SELECT * FROM Zamowienia left join Klienci_hurtowi on "
                      "Zamowienia.Klienci_hurtowi_ID=Klienci_hurtowi.ID WHERE "
                      "`Data` LIKE '%1'")
                  .arg(date.toString("'%'yyyy-MM-dd'%'")));

    if (q.exec())
    {
        while (q.next())
        {
            data.push_back(Common::OrdersStruct(
                q.value("Ilosc").toUInt(), q.value("Data").toDate(), q.value("Cena").toDouble(),
                Common::CustomerStruct(q.value("Odbiorca").toString(), q.value("Miasto").toString(),
                                       q.value("Ulica").toString(), q.value("Numer").toString()),
                q.value("Typ_Paliwa_Nazwa").toString(), q.value("Przychod").toDouble()));
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    return data;
}

int DataBaseApi::addCustomer(const Common::CustomerStruct& customer)
{
    QSqlQuery q;

    q.prepare("INSERT OR REPLACE INTO "
              "`Klienci_hurtowi`(`Odbiorca`,`Miasto`,`Ulica`,`Numer`) VALUES "
              "(?,?,?,?)");
    q.bindValue(0, customer.name);
    q.bindValue(1, customer.city);
    q.bindValue(2, customer.street);
    q.bindValue(3, customer.propertyNumber);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
    return q.lastInsertId().toInt();
}

void DataBaseApi::addOrder(const Common::OrdersStruct& order)
{
    QSqlQuery q;

    q.prepare("INSERT INTO "
              "`Zamowienia`(`Ilosc`,`Data`,`Cena`,`Klienci_hurtowi_ID`,`Typ_paliwa_"
              "Nazwa`,`Marza`) VALUES (?,?,?,?,?,?)");
    q.bindValue(0, order.amount);
    q.bindValue(1, order.date);
    q.bindValue(2, order.totalPrice);
    q.bindValue(3, addCustomer(order.customer));
    q.bindValue(4, order.fuelType);
    q.bindValue(5, order.establishedProfit);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void DataBaseApi::updateTankFillLevel(Common::FuelTankType tank, double number)
{
    QSqlQuery q;

    q.prepare("UPDATE Magazyny SET `Zawartosc` = ? WHERE `Zbiornik` = ?;");
    q.bindValue(0, number);
    q.bindValue(1, Common::getFuelTankTypeName(tank));

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void DataBaseApi::addPriceOfPetrol(double price, QDate date, Common::FuelType type)
{
    QSqlQuery q;

    q.prepare("INSERT INTO "
              "`Cena_paliwa`(`Data`,`Cena`,`Typ_paliwa) VALUES (?,?,?)");
    q.bindValue(0, date);
    q.bindValue(1, price);
    q.bindValue(2, Common::getFuelTypeName(type));

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

} // namespace DataBaseApi
