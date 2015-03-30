#include "MantidMDAlgorithms/SmoothMD.h"
#include "MantidAPI/FrameworkManager.h"
#include "MantidAPI/IMDHistoWorkspace.h"
#include "MantidAPI/IMDIterator.h"
#include "MantidAPI/Progress.h"
#include "MantidKernel/ArrayProperty.h"
#include "MantidKernel/ArrayBoundedValidator.h"
#include "MantidKernel/CompositeValidator.h"
#include "MantidKernel/ListValidator.h"
#include "MantidKernel/MandatoryValidator.h"
#include "MantidKernel/PropertyWithValue.h"
#include "MantidKernel/MultiThreaded.h"
#include "MantidMDEvents/MDHistoWorkspaceIterator.h"
#include <boost/make_shared.hpp>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <utility>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/tuple/tuple.hpp>

using namespace Mantid::Kernel;
using namespace Mantid::API;
using namespace Mantid::MDEvents;

typedef std::vector<int> WidthVector;
typedef boost::function<IMDHistoWorkspace_sptr(
    IMDHistoWorkspace_const_sptr, const WidthVector &)> SmoothFunction;
typedef std::map<std::string, SmoothFunction> SmoothFunctionMap;

namespace {

/**
 * @brief functions
 * @return Allowed smoothing functions
 */
std::vector<std::string> functions() {
  std::vector<std::string> propOptions;
  propOptions.push_back("Hat");
  // propOptions.push_back("Gaussian");
  return propOptions;
}

/**
 * Maps a function name to a smoothing function
 * @return function map
 */
SmoothFunctionMap makeFunctionMap(Mantid::MDAlgorithms::SmoothMD * instance) {
  SmoothFunctionMap map;
  map.insert(std::make_pair("Hat",  boost::bind( &Mantid::MDAlgorithms::SmoothMD::hatSmooth, instance, _1, _2 )));
  return map;
}

}

namespace Mantid {
namespace MDAlgorithms {

// Register the algorithm into the AlgorithmFactory
DECLARE_ALGORITHM(SmoothMD)

//----------------------------------------------------------------------------------------------
/** Constructor
 */
SmoothMD::SmoothMD() {}

//----------------------------------------------------------------------------------------------
/** Destructor
 */
SmoothMD::~SmoothMD() {}

//----------------------------------------------------------------------------------------------

/// Algorithms name for identification. @see Algorithm::name
const std::string SmoothMD::name() const { return "SmoothMD"; }

/// Algorithm's version for identification. @see Algorithm::version
int SmoothMD::version() const { return 1; };

/// Algorithm's category for identification. @see Algorithm::category
const std::string SmoothMD::category() const { return "MDAlgorithms"; }

/// Algorithm's summary for use in the GUI and help. @see Algorithm::summary
const std::string SmoothMD::summary() const {
  return "Smooth an MDHistoWorkspace according to a weight function";
};

/**
 * Hat function smoothing. All weights even. Hat function boundaries beyond
 * width.
 * @param toSmooth : Workspace to smooth
 * @param widthVector : Width vector
 * @return Smoothed MDHistoWorkspace
 */
IMDHistoWorkspace_sptr SmoothMD::hatSmooth(IMDHistoWorkspace_const_sptr toSmooth,
                                 const WidthVector &widthVector) {

  Progress progress(this, 0, 1, size_t( toSmooth->getNPoints() ) );
  // Create the output workspace.
  IMDHistoWorkspace_sptr outWS = toSmooth->clone();

  const int nThreads = Mantid::API::FrameworkManager::Instance()
                           .getNumOMPThreads(); // NThreads to Request

  auto iterators = toSmooth->createIterators(nThreads, NULL);

  PARALLEL_FOR_NO_WSP_CHECK()
  for (int it = 0; it < int(iterators.size()); ++it) {

    PARALLEL_START_INTERUPT_REGION
    boost::scoped_ptr<MDHistoWorkspaceIterator> iterator(
        dynamic_cast<MDHistoWorkspaceIterator *>(iterators[it]));

    size_t counter = 0;
    do {
      // Gets all vertex-touching neighbours

      std::vector<size_t> neighbourIndexes =
          iterator->findNeighbourIndexesByWidth(
              widthVector.front()); // TODO we should use the whole width vector
                                    // not just the first element.
      const size_t nNeighbours = neighbourIndexes.size();
      double sumSignal = iterator->getSignal();
      double sumSqError = iterator->getError();
      for (size_t i = 0; i < neighbourIndexes.size(); ++i) {
        sumSignal += toSmooth->getSignalAt(neighbourIndexes[i]);
        double error = toSmooth->getErrorAt(neighbourIndexes[i]);
        sumSqError += (error * error);
      }

      // Calculate the mean
      outWS->setSignalAt(iterator->getLinearIndex(),
                         sumSignal / (nNeighbours + 1));
      // Calculate the sample variance
      outWS->setErrorSquaredAt(iterator->getLinearIndex(),
                               sumSqError / (nNeighbours + 1));

      if(counter % 1000 == 0) {
          progress.report();
      }
      ++counter;


    } while (iterator->next());
    PARALLEL_END_INTERUPT_REGION
  }
  PARALLEL_CHECK_INTERUPT_REGION

  return outWS;
}


//----------------------------------------------------------------------------------------------
/** Initialize the algorithm's properties.
 */
void SmoothMD::init() {
  declareProperty(new WorkspaceProperty<API::IMDHistoWorkspace>(
                      "InputWorkspace", "", Direction::Input),
                  "An input MDHistoWorkspace to smooth.");

  auto widthVectorValidator = boost::make_shared<CompositeValidator>();
  auto boundedValidator =
      boost::make_shared<ArrayBoundedValidator<int>>(1, 100);
  widthVectorValidator->add(boundedValidator);
  widthVectorValidator->add(
      boost::make_shared<MandatoryValidator<std::vector<int>>>());

  declareProperty(new ArrayProperty<int>("WidthVector", widthVectorValidator,
                                         Direction::Input),
                  "Width vector. Either specify the width in n-pixels for each "
                  "dimension, or provide a single entry (n-pixels) for all "
                  "dimensions.");

  const auto allFunctionTypes = functions();
  const std::string first = allFunctionTypes.front();

  std::stringstream docBuffer;
  docBuffer << "Smoothing function. Defaults to " << first;
  declareProperty(
      new PropertyWithValue<std::string>(
          "Function", first,
          boost::make_shared<ListValidator<std::string>>(allFunctionTypes),
          Direction::Input),
      docBuffer.str());

  declareProperty(new WorkspaceProperty<API::IMDHistoWorkspace>(
                      "OutputWorkspace", "", Direction::Output),
                  "An output smoothed MDHistoWorkspace.");
}

//----------------------------------------------------------------------------------------------
/** Execute the algorithm.
 */
void SmoothMD::exec() {

  // Get the input workspace to smooth
  IMDHistoWorkspace_sptr toSmooth = this->getProperty("InputWorkspace");
  // Get the width vector
  const std::vector<int> widthVector = this->getProperty("WidthVector");

  // Find the choosen smooth operation
  const std::string smoothFunctionName = this->getProperty("Function");
  SmoothFunctionMap functionMap = makeFunctionMap(this);
  SmoothFunction smoothFunction = functionMap[smoothFunctionName];
  // invoke the smoothing operation
  auto smoothed = smoothFunction(toSmooth, widthVector);

  setProperty("OutputWorkspace", smoothed);
}

/**
 * validateInputs
 * @return map of property names to errors.
 */
std::map<std::string, std::string> SmoothMD::validateInputs() {

  std::map<std::string, std::string> product;

  const std::string widthVectorPropertyName = "WidthVector";
  std::vector<int> widthVector = this->getProperty(widthVectorPropertyName);
  for (auto it = widthVector.begin(); it != widthVector.end(); ++it) {
    const int widthEntry = *it;
    if (widthEntry % 2 == 0) {
      std::stringstream message;
      message << widthVectorPropertyName
              << " entries must be odd numbers. Bad entry is " << widthEntry;
      product.insert(std::make_pair(widthVectorPropertyName, message.str()));
    }
  }
  return product;
}

} // namespace MDAlgorithms
} // namespace Mantid
