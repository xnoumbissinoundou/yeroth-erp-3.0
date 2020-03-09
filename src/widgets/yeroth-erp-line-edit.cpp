/*
 * yeroth-erp-line-edit.cpp
 *
 *      Author: Xavier NOUMBISSI NOUNDOU, Dipl.-Inf., Ph.D. (ABD)
 */

#include "yeroth-erp-line-edit.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include <QtWidgets/QCompleter>

#include <QtCore/QDebug>

const QString YerothLineEdit::EMPTY_STRING("");

YerothLineEdit::YerothLineEdit(QWidget *parent /* = 0 */)
:QLineEdit(parent),
 _forSearch(false),
 _wasMissingRequiredText(false),
 _sqlTableModel(0),
 _sqlTableModelView(0),
 _searchQCompleter(0)
{
	_originalPlaceHolderText = placeholderText();

	_originalPaletteBeforeMissingInformation = palette();

    setYerothERPQLineEditDisplayFormat();
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

    setPlaceholderText("");
}

bool YerothLineEdit::checkField(const QPalette *aPalette /* = 0 */)
{
    if (text().isEmpty())
    {
    	_wasMissingRequiredText = true;

    	setText("");

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


void YerothLineEdit::setEnabled(bool enabled)
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

void YerothLineEdit::myClear()
{
    QLineEdit::clear();
    QLineEdit::setText(EMPTY_STRING);
}

void YerothLineEdit::setupMyQCompleter(QString sqlTableName,
                                      int listColumnIndex,
                                      enum Qt::SortOrder sortOrder,
									  bool truncateString /* = true*/)
{
    _sqlTableModel = new YerothSqlTableModel(sqlTableName);
    _sqlTableModel->yerothSetSort(listColumnIndex, sortOrder);

    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);
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
											 bool enableNewCreation /* = false */,
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

    if (enableNewCreation)
    {
        if (YerothUtils::isEqualCaseInsensitive("CATEGORIES", sqlTableName))
        {
# ifdef YEROTH_FRANCAIS_LANGUAGE
            _currentStaticStringList.append(QObject::trUtf8(STRING_NEW_CATEGORY_FR));
# else //YEROTH_ENGLISH_LANGUAGE
            _currentStaticStringList.append(STRING_NEW_CATEGORY_EN);
# endif
        }
        else if (YerothUtils::isEqualCaseInsensitive("FOURNISSEURS", sqlTableName))
        {
# ifdef YEROTH_FRANCAIS_LANGUAGE
            _currentStaticStringList.append(QObject::trUtf8(STRING_NEW_SUPPLIER_FR));
# else //YEROTH_ENGLISH_LANGUAGE
            _currentStaticStringList.append(STRING_NEW_SUPPLIER_EN);
# endif
        }
        else if (YerothUtils::isEqualCaseInsensitive("CLIENTS", sqlTableName))
        {
# ifdef YEROTH_FRANCAIS_LANGUAGE
            _currentStaticStringList.append(QObject::trUtf8(STRING_NEW_CLIENT_FR));
# else //YEROTH_ENGLISH_LANGUAGE
            _currentStaticStringList.append(STRING_NEW_CLIENT_EN);
# endif
        }
    }

    _searchQCompleter = new QCompleter(_currentStaticStringList, this);

    _searchQCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    _searchQCompleter->setCompletionMode(QCompleter::PopupCompletion);

    setCompleter(_searchQCompleter);

    connect(this, SIGNAL(editingFinished()), this, SLOT(isInputValid()));

    if (returnPressedSignalActivated)
    {
        connect( this, SIGNAL(returnPressed()), this, SLOT(clearQCompleterText()) );
    }
}


void YerothLineEdit::setupMyQCompleterALL(QString sqlTableName)
{
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _designationToTableRows.clear();

    _sqlTableModelView->lister_ALL(*_sqlTableModel,
                                   _designationToTableRows);

    QStringList articleDesignations;

    QMapIterator<QString, int> it(_designationToTableRows);

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

void YerothLineEdit::setupMyQCompleterDEF_DEO(QString sqlTableName)
{
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_DEF_DEO(*_sqlTableModel,
                                       _designationToTableRows);

    QStringList articleDesignations;

    QMapIterator<QString, int> it(_designationToTableRows);

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
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_FIFO(*_sqlTableModel,
                                    _designationToTableRows);

    QStringList articleDesignations;

    QMapIterator<QString, int> it(_designationToTableRows);

    //qDebug() << "++ YerothLineEdit::setupMyQCompleterFIFO, size articleDesignations: "
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

void YerothLineEdit::setupMyQCompleterLIFO(QString sqlTableName)
{
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_LIFO(*_sqlTableModel,
                                    _designationToTableRows);

    QStringList articleDesignations;

    QMapIterator<QString, int> it(_designationToTableRows);

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
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_ALL(*_sqlTableModel,
                                           _codebarToTableRows);

    QStringList articleCodebars;

    QMapIterator<QString, int> it(_codebarToTableRows);
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

void YerothLineEdit::setupMyQCompleterCodebarDEF_DEO(QString sqlTableName)
{
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_DEF_DEO(*_sqlTableModel,
            _codebarToTableRows);

    QStringList articleCodebars;

    QMapIterator<QString, int> it(_codebarToTableRows);
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
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_FIFO(*_sqlTableModel,
                                            _codebarToTableRows);

    QStringList articleCodebars;

    QMapIterator<QString, int> it(_codebarToTableRows);
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
    _sqlTableModelView = new YerothTableView;
    _sqlTableModelView->setTableName(&sqlTableName);

    _sqlTableModel = new YerothSqlTableModel(sqlTableName, true);

    _sqlTableModelView->lister_codebar_LIFO(*_sqlTableModel,
                                            _codebarToTableRows);

    QStringList articleCodebars;

    QMapIterator<QString, int> it(_codebarToTableRows);
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
    _designationToTableRows.clear();

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL,
                                           strategy))
    {
        _sqlTableModelView->lister_codebar_ALL(*_sqlTableModel, _designationToTableRows);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
             strategy))
    {
        _sqlTableModelView->lister_codebar_FIFO(*_sqlTableModel, _designationToTableRows);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
             strategy))
    {
        _sqlTableModelView->lister_codebar_LIFO(*_sqlTableModel, _designationToTableRows);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO,
             strategy))
    {
        _sqlTableModelView->lister_codebar_DEF_DEO(*_sqlTableModel, _designationToTableRows);
    }
}

void YerothLineEdit::refreshCompleterList(QString strategy)
{
    //qDebug() << "++ test refresh: " << _sqlTableModelView;
    _designationToTableRows.clear();

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL,
                                           strategy))
    {
        _sqlTableModelView->lister_ALL(*_sqlTableModel, _designationToTableRows);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
             strategy))
    {
        _sqlTableModelView->lister_FIFO(*_sqlTableModel, _designationToTableRows);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
             strategy))
    {
        _sqlTableModelView->lister_LIFO(*_sqlTableModel, _designationToTableRows);
    }
    else if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::STRATEGIE_VENTE_SORTIE_DEF_DEO,
             strategy))
    {
        _sqlTableModelView->lister_DEF_DEO(*_sqlTableModel, _designationToTableRows);
    }
}
