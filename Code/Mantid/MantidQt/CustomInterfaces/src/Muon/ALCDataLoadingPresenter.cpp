#include "MantidQtCustomInterfaces/Muon/ALCDataLoadingPresenter.h"

#include "MantidAPI/AlgorithmManager.h"

using namespace Mantid::API;

namespace MantidQt
{
namespace CustomInterfaces
{
  ALCDataLoadingPresenter::ALCDataLoadingPresenter(IALCDataLoadingView* view)
    : m_view(view)
  {}

  ALCDataLoadingPresenter::~ALCDataLoadingPresenter()
  {}

  void ALCDataLoadingPresenter::initialize()
  {
    connectView();
  }

  void ALCDataLoadingPresenter::connectView()
  {
    connect(m_view, SIGNAL(loadData()), SLOT(loadData()));
  }

  void ALCDataLoadingPresenter::loadData()
  {
    try
    {
      IAlgorithm_sptr alg = AlgorithmManager::Instance().create("PlotAsymmetryByLogValue");
      alg->setChild(true); // Don't want workspaces in the ADS
      alg->setProperty("FirstRun", m_view->firstRun());
      alg->setProperty("LastRun", m_view->lastRun());
      alg->setProperty("LogValue", m_view->log());
      alg->setPropertyValue("OutputWorkspace", "__NotUsed__");
      alg->execute();

      MatrixWorkspace_const_sptr result = alg->getProperty("OutputWorkspace");

      m_view->displayData(result);
    }
    catch(std::exception& e)
    {
      m_view->displayError(e.what());
    }
  }
} // namespace CustomInterfaces
} // namespace MantidQt
