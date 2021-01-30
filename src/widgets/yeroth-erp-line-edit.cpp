/*
 * yeroth-erp-line-edit.cpp
 *
 *      Author: DR. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-line-edit.hpp"


#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtWidgets/QInputDialog>

#include <QtWidgets/QCompleter>

#include <QtCore/QDebug>


YerothLineEdit::YerothLineEdit(QWidget *parent /* = 0 */)
:QLineEdit(parent),
 _firstTimeStyleSheetCheck(true),
 _forSearch(false),
 _inputDialog(0),
 _wasMissingRequiredText(false),
 _sqlTableModel(0),
 _sqlTableModelView(0),
 _searchQCompleter(0)
{
	setMaxLength(255);

	setValidator(&YerothUtils::STRING_FOR_YEROTH_ERP_3_0_VALIDATOR);

	_aDefaultStyleSheet =
			QString("QInputDialog {background-color: rgb(%1); color: rgb(%2);}")
				.arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67,
					 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

	_inputDialog = new QInputDialog;

	_inputDialog->setWindowTitle(YerothUtils::EMPTY_STRING);

	_inputDialog->setLabelText(QObject::tr("contenu de ce champs de texte"));

	_inputDialog->setTextEchoMode(QLineEdit::Normal);

	_inputDialog->setOption(QInputDialog::NoButtons);

	_originalPlaceHolderText = placeholderText();

	_originalPaletteBeforeMissingInformation = palette();


    setYerothERPQLineEditDisplayFormat();
}


YerothLineEdit::~YerothLineEdit()
{
	if (0 != _inputDialog)
	{
		delete _inputDialog;
	}
}


void YerothLineEdit::enableForSearch(QString placeHoderText)
{
    _forSearch = true;

    setPlaceholderText(placeHoderText);

    _originalPlaceHolderText = placeHoderText;
}

void YerothLineEdit::disableForSearch()
{
    _forSearch = false;

    setPlaceholderText(YerothUtils::EMPTY_STRING);
}

bool YerothLineEdit::checkField(const QPalette *aPalette /* = 0 */)
{
    if (text().isEmpty())
    {
    	_wasMissingRequiredText = true;

    	setText(YerothUtils::EMPTY_STRING);

    	setPlaceholderText(QObject::trUtf8("[* champ de texte obligatoire]"));

    	if (0 != aPalette)
    	{
    		setPalette(*aPalette);
    	}
    	else
    	{
    		setPalette(YerothUtils::YEROTH_RED_COLOR);
    	}

        return false;
    }

    setPlaceholderText(_originalPlaceHolderText);

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}

bool YerothLineEdit::checkFieldAndMinimalValue(double min, QPalette *aPalette /* = 0 */)
{
    if (text().isEmpty() 		||
        text().toDouble() < min)
    {
    	_wasMissingRequiredText = true;

    	if (0 != aPalette)
    	{
    		setPalette(*aPalette);
    	}
    	else
    	{
    		setPalette(YerothUtils::YEROTH_RED_COLOR);
    	}

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}


void YerothLineEdit::setYerothERPQLineEditDisplayFormat()
{
	if (isReadOnly())
	{
		bool isLeftAligned = ( (Qt::AlignLeft | alignment()) == 1 );

		/*
		 * Read-only text must be horizontally centered.
		 * We don't modify already centered or
		 * right-aligned text.
		 */
		if ( isLeftAligned )
		{
			setAlignment(Qt::AlignHCenter);
		}
	}
    else
    {
        setAlignment(Qt::AlignLeft);
    }

    if (!_forSearch)
    {
        if (isReadOnly())
        {
            setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
        }
        else
        {
            setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        }
    }

    YerothUtils::yerothSetWidgetColor(this);
}


void YerothLineEdit::clearQCompleter()
{
	clearQCompleterText();

	if (0 != _searchQCompleter)
	{
		delete _searchQCompleter;

		_searchQCompleter = 0;
	}
}


void YerothLineEdit::clearField()
{
    QLineEdit::clear();

    setPlaceholderText(_originalPlaceHolderText);

    setPalette(_originalPaletteBeforeMissingInformation);

    if (_wasMissingRequiredText)
    {
    	_wasMissingRequiredText = false;
    }

	setYerothERPQLineEditDisplayFormat();
}


void YerothLineEdit::setYerothEnabled(bool enabled)
{
    QLineEdit::setReadOnly(!enabled);

    setYerothERPQLineEditDisplayFormat();
}

void YerothLineEdit::clearQCompleterText()
{
    _lastQCompleterText = text();
    myClear();
}

void YerothLineEdit::isInputValid()
{
    if (text().isEmpty())
    {
        return ;
    }
}


QString YerothLineEdit::text(int truncate_pos /* = -1 */) const
{
	QString a_truncated_string(QLineEdit::text().trimmed());

	if (truncate_pos < 0 						 ||
		a_truncated_string.length() <= truncate_pos)
	{
		return a_truncated_string;
	}

	a_truncated_string.truncate(truncate_pos-1);

	return a_truncated_string.append(YerothUtils::DOT_CHAR);
}


void YerothLineEdit::myClear()
{
    QLineEdit::clear();
    QLineEdit::setText(YerothUtils::EMPTY_STRING);
}


void YerothLineEdit::setupMyQCompleter(QString sqlTableName,
                                      int listColumnIndex,
                                      enum Qt::SortOrder sortOrder,
									  bool truncateString /* = true*/)
{
    _sqlTableModel = new YerothSqlTableModel(sqlTableName);
    _sqlTableModel->yerothSetSort(listColumnIndex, sortOrder);

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);
    _sqlTableModelView->lister(*_sqlTableModel, truncateString);

    _searchQCompleter = new QCompleter(this);

    _searchQCompleter->setModel(_sqlTableModelView->getStandardItemModel());
    _searchQCompleter->setCompletionColumn(listColumnIndex);
    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect( this, SIGNAL(returnPressed()), this, SLOT(clearQCompleterText()) );
}


void YerothLineEdit::setupMyStaticQCompleter(QString sqlTableName,
        									 const QString fieldName,
											 bool returnPressedSignalActivated /* = true */,
											 QString aQSqlConditionStr /* = YerothUtils::EMPTY_STRING */)
{
    YerothUtils::getColumnListString(_currentStaticStringList,
                                     sqlTableName,
                                     fieldName,
									 aQSqlConditionStr);

    if (0 != _searchQCompleter)
    {
        delete _searchQCompleter;
    }

    _searchQCompleter = new QCompleter(_currentStaticStringList, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(editingFinished()), this, SLOT(isInputValid()));

    if (returnPressedSignalActivated)
    {
        connect(this,
        		SIGNAL(returnPressed()),
				this,
				SLOT(clearQCompleterText()) );
    }
    else
    {
		disconnect(this,
				   SIGNAL(returnPressed()),
				   this,
				   SLOT(clearQCompleterText()) );
    }
}


void YerothLineEdit::setupMyQCompleterALL(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _stockNameToStockID.clear();

    _sqlTableModelView->lister_ALL(*_sqlTableModel,
                                   _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator<QString, QString> it(_stockNameToStockID);

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect( this, SIGNAL(returnPressed()), this, SLOT(clearQCompleterText()) );
}


void YerothLineEdit::setupMyQCompleterFEFO(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_FEFO(*_sqlTableModel,
                                    _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator<QString, QString> it(_stockNameToStockID);

    //qDebug() << "++ YerothLineEdit::setupMyQCompleterDPF_DPO, size articleDesignations: "
    //<< articleDesignations.size();

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    //qDebug() << "++ qlineedit. articleDesignations: " << articleDesignations;

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect( this, SIGNAL(returnPressed()), this, SLOT(clearQCompleterText()) );
}


void YerothLineEdit::setupMyQCompleterFIFO(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_FIFO(*_sqlTableModel,
                                    _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator<QString, QString> it(_stockNameToStockID);

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect( this, SIGNAL(returnPressed()), this, SLOT(clearQCompleterText()) );
}


void YerothLineEdit::setupMyQCompleterLIFO(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_LIFO(*_sqlTableModel,
                                    _stockNameToStockID);

    QStringList articleDesignations;

    QMapIterator<QString, QString> it(_stockNameToStockID);

    //qDebug() << "++ YerothLineEdit::setupMyQCompleterLIFO, size articleDesignations: "
    //	 << articleDesignations.size();

    while (it.hasNext())
    {
        it.next();
        articleDesignations.append(it.key());
    }

    _searchQCompleter = new QCompleter(articleDesignations, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect( this, SIGNAL(returnPressed()), this, SLOT(clearQCompleterText()) );
}


void YerothLineEdit::setupMyQCompleterCodebarALL(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_ALL(*_sqlTableModel,
                                           _stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator<QString, QString> it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerothLineEdit::setupMyQCompleterCodebarFEFO(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_FEFO(*_sqlTableModel,
            								_stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator<QString, QString> it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerothLineEdit::setupMyQCompleterCodebarFIFO(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_FIFO(*_sqlTableModel,
                                            _stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator<QString, QString> it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerothLineEdit::setupMyQCompleterCodebarLIFO(QString sqlTableName)
{
	if (0 != _sqlTableModel)
	{
		delete _sqlTableModel;
	}

	if (0 != _sqlTableModelView)
	{
		delete _sqlTableModelView;
	}

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setSqlTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_LIFO(*_sqlTableModel,
                                            _stockReferenceToStockID);

    QStringList articleCodebars;

    QMapIterator<QString, QString> it(_stockReferenceToStockID);
    QString aCodebarString;

    while (it.hasNext())
    {
        it.next();

        aCodebarString = it.key();

        if (aCodebarString.isEmpty())
        {
            continue;
        }

        articleCodebars.append(aCodebarString);
    }

    _searchQCompleter = new QCompleter(articleCodebars, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);
}


void YerothLineEdit::refreshCodebarCompleterList(QString strategy)
{
    //qDebug() << "++ test refresh: " << _sqlTableModelView;
    _stockNameToStockID.clear();

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL,
                                           strategy))
    {
        _sqlTableModelView->lister_codebar_ALL(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
             strategy))
    {
        _sqlTableModelView->lister_codebar_FIFO(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
             strategy))
    {
        _sqlTableModelView->lister_codebar_LIFO(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             strategy))
    {
        _sqlTableModelView->lister_codebar_FEFO(*_sqlTableModel, _stockNameToStockID);
    }
}


void YerothLineEdit::refreshCompleterList(QString strategy)
{
    //qDebug() << "++ test refresh: " << _sqlTableModelView;
    _stockNameToStockID.clear();

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL,
                                           strategy))
    {
        _sqlTableModelView->lister_ALL(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
             strategy))
    {
        _sqlTableModelView->lister_FIFO(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
             strategy))
    {
        _sqlTableModelView->lister_LIFO(*_sqlTableModel, _stockNameToStockID);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             strategy))
    {
        _sqlTableModelView->lister_FEFO(*_sqlTableModel, _stockNameToStockID);
    }
}


void YerothLineEdit::hideEvent(QHideEvent * hideEvent)
{
	if (0 != _inputDialog)
	{
		_inputDialog->close();
	}
}


void YerothLineEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (text().isEmpty() ||
		!isReadOnly())
	{
		return ;
	}

	if (_firstTimeStyleSheetCheck)
	{
		QString aStyleSheet(styleSheet());

		if (!aStyleSheet.isEmpty())
		{
			_inputDialog->setStyleSheet(aStyleSheet);
		}
		else
		{
			_inputDialog->setStyleSheet(_aDefaultStyleSheet);
		}

		_firstTimeStyleSheetCheck = false;
	}

	_inputDialog->setTextValue(text());

	_inputDialog->show();
}

