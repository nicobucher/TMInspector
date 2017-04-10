#ifndef CUSTOMMODELS_H
#define CUSTOMMODELS_H

#include <QStringListModel>


/*
 * This String List implements QStringList and holds only unique string items.
 * It is used as a container for e.g. the objects on the watch list.
 */
class StringList : public QStringListModel
{
public:
  void append (const QString& string) {
    if (match(this->index(0, 0), Qt::DisplayRole, string).count() > 0) {
        // string is already in the model list.. skip
    } else {
        insertRows(this->rowCount(), 1);
        setData(index(this->rowCount() - 1), string);
    }
  }

  void deleteRow(const QString& string) {
      QModelIndex index = match(this->index(0, 0), Qt::DisplayRole, string)[0];
      removeRows(index.row(), 1);
  }

  /*
   * Convenience operator for putting a new sting into the model list.
   */
  StringList& operator<<(const QString& string) {
    append(string);
    return *this;
  }
};

#endif // CUSTOMMODELS_H
