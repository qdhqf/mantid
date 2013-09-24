#include "MantidQtAPI/ManageUserDirectories.h"
#include "MantidQtCustomInterfaces/IndirectBayes.h"
#include "MantidQtCustomInterfaces/JumpFit.h"
#include "MantidQtCustomInterfaces/Quasi.h"
#include "MantidQtCustomInterfaces/ResNorm.h"
#include "MantidQtCustomInterfaces/Stretch.h"

#include <QDesktopServices>
#include <QUrl>

//Add this class to the list of specialised dialogs in this namespace
namespace MantidQt
{
  namespace CustomInterfaces
  {
    DECLARE_SUBWINDOW(IndirectBayes);
  }
}

using namespace MantidQt::CustomInterfaces;

IndirectBayes::IndirectBayes(QWidget *parent) : UserSubWindow(parent)
{
	m_uiForm.setupUi(this);

	//insert each tab into the interface on creation
	m_bayesTabs.insert(std::make_pair(RES_NORM, new ResNorm(m_uiForm.indirectBayesTabs->widget(RES_NORM))));
	m_bayesTabs.insert(std::make_pair(QUASI, new Quasi(m_uiForm.indirectBayesTabs->widget(QUASI))));
	m_bayesTabs.insert(std::make_pair(STRETCH, new Stretch(m_uiForm.indirectBayesTabs->widget(STRETCH))));
	m_bayesTabs.insert(std::make_pair(JUMP_FIT, new JumpFit(m_uiForm.indirectBayesTabs->widget(JUMP_FIT))));

	//Connect each tab to the actions available in this GUI
	std::map<unsigned int, IndirectBayesTab*>::iterator iter;
	for (iter = m_bayesTabs.begin(); iter != m_bayesTabs.end(); ++iter)
	{
		connect(iter->second, SIGNAL(executePythonScript(const QString&, bool)), this, SIGNAL(runAsPythonScript(const QString&, bool)));
		connect(iter->second, SIGNAL(showMessageBox(const QString&)), this, SLOT(showMessageBox(const QString&)));
		
	}

	loadSettings();

	//Connect statements for the buttons shared between all tabs on the Indirect Bayes interface
	connect(m_uiForm.pbRun, SIGNAL(clicked()), this, SLOT(runClicked()));
	connect(m_uiForm.pbHelp, SIGNAL(clicked()), this, SLOT(helpClicked()));
	connect(m_uiForm.pbManageDirs, SIGNAL(clicked()), this, SLOT(manageUserDirectories()));

	
}

void IndirectBayes::initLayout()
{
}

/**
 * Load the setting for each tab on the interface.
 *
 * This includes setting the default browsing directory to be the default save directory.
 */
void IndirectBayes::loadSettings()
{
  QSettings settings;
  QString settingsGroup = "CustomInterfaces/IndirectAnalysis/";
  QString saveDir = QString::fromStdString(Mantid::Kernel::ConfigService::Instance().getString("defaultsave.directory"));

  settings.beginGroup(settingsGroup + "ProcessedFiles");
  settings.setValue("last_directory", saveDir);

	std::map<unsigned int, IndirectBayesTab*>::iterator iter;
	for (iter = m_bayesTabs.begin(); iter != m_bayesTabs.end(); ++iter)
	{
  	iter->second->loadSettings(settings);
  }

  settings.endGroup();
}


/**
 * Slot to run the underlying algorithm code based on the currently selected
 * tab.
 * 
 * This method checks the tabs validate method is passing before calling
 * the run method.
 */
void IndirectBayes::runClicked()
{
	int tabIndex = m_uiForm.indirectBayesTabs->currentIndex();

	if(m_bayesTabs[tabIndex]->validate())
	{
		m_bayesTabs[tabIndex]->run();
	}
}

/**
 * Slot to open a new browser window and navigate to the help page
 * on the wiki for the currently selected tab.
 */
void IndirectBayes::helpClicked()
{
	int tabIndex = m_uiForm.indirectBayesTabs->currentIndex();
	QString url = m_bayesTabs[tabIndex]->tabHelpURL();
	QDesktopServices::openUrl(QUrl(url));
}

/**
 * Slot to show the manage user dicrectories dialog when the user clicks
 * the button on the interface.
 */
void IndirectBayes::manageUserDirectories()
{
  MantidQt::API::ManageUserDirectories *ad = new MantidQt::API::ManageUserDirectories(this);
  ad->show();
  ad->setFocus();
}

/**
 * Slot to wrap the protected showInformationBox method defined
 * in UserSubWindow and provide access to composed tabs.
 * 
 * @param message :: The message to display in the message box
 */
void IndirectBayes::showMessageBox(const QString& message)
{
  showInformationBox(message);
}

IndirectBayes::~IndirectBayes()
{
}
