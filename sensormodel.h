#ifndef SENSORMODEL_H
#define SENSORMODEL_H

#include <QAbstractTableModel>
#include <QDir>
#include <QFont>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QPainter>
#include <QStandardPaths>
#include <QStyledItemDelegate>
#include <QUuid>
#include <QVariant>

#include "sensor.h"

class SensorModel : public QAbstractTableModel {
  Q_OBJECT
public:
  SensorModel(QObject *parent = nullptr);
  //SensorModel(const QList<Sensor*> &sensors, QObject *parent = nullptr);
  QString getDbName();
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  bool visible(int col) const;
  inline QModelIndex last() const {
    return index(count() - 1, 0, QModelIndex());
  }
  inline int count() const {
    return sensors->size();
  }
  void add(Sensor *sensor);
  Sensor *get(int row);
  void replace(Sensor &sensor);
  int read();
  void write();

private:
  QList<Sensor*> *sensors;
  int indexOf(QList<Sensor*> list, std::function<bool (const Sensor *)> &predicate);
};

class CustomDelegate : public QStyledItemDelegate {
public:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
    if (index.column() == 1 /*Name*/) {
      QModelIndex active = index.sibling(index.row(), 2 /*Active*/);
      QVariant bold = active.data(Qt::DisplayRole);
      if (bold.isValid() && bold.toBool()) {
        QStyleOptionViewItem opt = option;
        opt.font.setBold(true);
        initStyleOption(&opt, index);
        QStyledItemDelegate::paint(painter, opt, index);
        return;
      }
    }
    QStyledItemDelegate::paint(painter, option, index);
  }
};

#endif // SENSORMODEL_H
