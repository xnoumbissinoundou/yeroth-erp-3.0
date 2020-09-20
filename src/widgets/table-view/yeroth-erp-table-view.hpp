/*
 * yeroth-erp-table-view.hpp
 *
 *      Author: Dipl.-Inf. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_TABLE_VIEW_HPP_
#define SRC_YEROTH_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"


#include <QtWidgets/QTableView>

#include <QtSql/QSqlTableModel>


class YerothERPWindows;

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothWindowsCommons;


class YerothTableView : public QTableView
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	YerothTableView();

	YerothTableView(QWidget * parent);

	virtual ~YerothTableView();

	inline void setWriteEnabled(bool w){ _writeEnabled = w; }

	inline bool isWriteEnabled() { return _writeEnabled; }

	void setupSelectionOptions();

	inline virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
	{
	}

	void lister_les_transactions_dun_fournisseur(QSqlQuery &sqlFournisseurTransactionsUnionQuery);

	void lister_les_transactions_dun_client(QSqlQuery &sqlClientTransactionsUnionQuery);

	void lister_lhistorique_du_stock(const QStringList &aMouvementStockList);

	void lister(YerothSqlTableModel &sqlTableModel,
				bool truncateString = true);

	void lister_ALL(YerothSqlTableModel &tableModel,
					QMap<QString, QString> &stockNameToStockID_in_out);

	void lister_FIFO(YerothSqlTableModel &tableModel,
					 QMap<QString, QString> &stockNameToStockID_in_out);

	QDate getEarliestDate(QMultiMap<QString, QDate> allElements, QString aDesignation);

	QDate getLatestDate(QMultiMap<QString, QDate> allElements, QString aDesignation);

	void lister_LIFO(YerothSqlTableModel &tableModel,
					 QMap<QString, QString> &stockNameToStockID_in_out);

	void lister_FEFO(YerothSqlTableModel &tableModel,
					    QMap<QString, QString> &stockNameToStockID_in_out);

	void lister_codebar_ALL(YerothSqlTableModel &tableModel,
					 	 	QMap<QString, QString> &stockReferenceToStockID_in_out);

	void lister_codebar_FIFO(YerothSqlTableModel &tableModel,
					 	 	 QMap<QString, QString> &stockReferenceToStockID_in_out);

	void lister_codebar_LIFO(YerothSqlTableModel &tableModel,
					 	 	 QMap<QString, QString> &stockReferenceToStockID_in_out);

	void lister_codebar_FEFO(YerothSqlTableModel &tableModel,
					 	 	    QMap<QString, QString> &stockReferenceToStockID_in_out);

	inline int rowCount()
	{
		return _stdItemModel->rowCount();
	}

	inline const QString &getSqlTableName()
	{
		return *_tableName;
	}

	inline void setSqlTableName(const QString *tableName)
	{
		_tableName = tableName;
	}

	inline int lastSelectedRow()
	{
		return _lastSelectedRow;
	}

	inline void setLastSelectedRow(int row)
	{
		_lastSelectedRow = row;
	}

	inline YerothPOSQStandardItemModel *getStandardItemModel()
	{
		return _stdItemModel;
	}

	inline void setStandardItemModel(YerothPOSQStandardItemModel *stdItemModel)
	{
		_stdItemModel = stdItemModel;
	}


	static YerothERPWindows *YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER;


    static const int REFERENCE_COLUMN;

    static const int DATE_ENTREE_COLUMN;

    static const int DATE_PREEMPTION_COLUMN;

    static const int DESIGNATION_COLUMN;

    static const int QUANTITE_TOTAL_COLUMN;

signals:

	void signal_lister(YerothSqlTableModel &sqlTableModel);

protected:

	virtual void selectionChanged(const QItemSelection & selected,
								  const QItemSelection & deselected);

	bool 						_writeEnabled;

	int 						_lastSelectedRow;

	const QString 				*_tableName;

	YerothPOSQStandardItemModel *_stdItemModel;

	QStringList 				*_tableModelHeaders;
};


#endif /* SRC_YEROTH_TABLE_VIEW_HPP_ */
