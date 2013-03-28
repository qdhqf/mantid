/*WIKI* 

Accepts an input workspace and the name of a file.  Retrieves the algorithm history of the workspace, and then dumps it to file in the format of a Python script.  

Example format:

<div style="border:1pt dashed black; background:#f9f9f9;padding: 1em 0;">
<source lang="python">
 ######################################################################
 #Python Script Generated by GeneratePythonScript Algorithm
 ######################################################################
 LoadRaw(Filename='G:/Spencer/Science/Raw/irs26173.raw',OutputWorkspace='IPG',SpectrumMin='3',SpectrumMax='53')
 ConvertUnits(InputWorkspace='IPG',OutputWorkspace='Spec',Target='Wavelength')
 LoadRaw(Filename='G:/Spencer/Science/Raw/irs26173.raw',OutputWorkspace='Mon_in',SpectrumMax='1')
</source></div>

[[Category:Algorithms]]
{{AlgorithmLinks|GeneratePythonScript}}



*WIKI*/
#include "MantidAlgorithms/GeneratePythonScript.h"
#include "MantidAPI/FileProperty.h"
#include "MantidAPI/MultipleFileProperty.h"
#include "MantidAPI/AlgorithmManager.h"
#include "MantidAPI/AlgorithmHistory.h"

#include <fstream>

using namespace Mantid::Kernel;
using namespace Mantid::API;

namespace Mantid
{
namespace Algorithms
{

// Register the algorithm into the AlgorithmFactory
DECLARE_ALGORITHM(GeneratePythonScript)



//----------------------------------------------------------------------------------------------
/// Sets documentation strings for this algorithm
void GeneratePythonScript::initDocs()
{
  this->setWikiSummary("Generate a Python script file to reproduce the history of a workspace.");
  this->setOptionalMessage("An Algorithm to generate a Python script file to reproduce the history of a workspace.");
}

//----------------------------------------------------------------------------------------------
/** Initialize the algorithm's properties.
*/
void GeneratePythonScript::init()
{
  declareProperty(new WorkspaceProperty<Workspace>("InputWorkspace","",Direction::Input), "An input workspace.");

  std::vector<std::string> exts;
  exts.push_back(".py");

  declareProperty(new API::FileProperty("Filename","", API::FileProperty::Save, exts),
  "The file into which the Python script will be generated.");
}

//----------------------------------------------------------------------------------------------
/** Execute the algorithm.
*/
void GeneratePythonScript::exec()
{
  const Workspace_const_sptr ws = getProperty("InputWorkspace");
  const std::string filename = getPropertyValue("Filename");
  std::ofstream file(filename.c_str(), std::ofstream::trunc);


  if (NULL == file)
  {
    g_log.error("Unable to create file: " + filename);
    throw Exception::FileError("Unable to create file: " , filename);
  }

  // Get the algorithm histories of the workspace.
  const WorkspaceHistory wsHistory = ws->getHistory();

  // Cycle through the AlgorithHistory objects of the workspace, create a string for each one,
  // and then add them to the list.
  const WorkspaceHistory::AlgorithmHistories  & algHistories = wsHistory.getAlgorithmHistories();
  WorkspaceHistory::AlgorithmHistories::const_iterator algHistIter = algHistories.begin();

  std::vector<std::string> orderedHists;
  for( ; algHistIter != algHistories.end(); ++algHistIter)
  {
    orderedHists.push_back(genAlgString(*algHistIter));
  }

  // Generate the python script from each of the Algorith strings, then write to file.
  std::string generatedScript = "";
  generatedScript += "######################################################################\n";
  generatedScript += "#Python Script Generated by GeneratePythonScript Algorithm\n";
  generatedScript += "######################################################################\n";

  std::vector<std::string>::iterator m3_pIter;
  for (m3_pIter=orderedHists.begin( );m3_pIter!=orderedHists.end( );++m3_pIter)
  {
    generatedScript += *m3_pIter + "\n";
  }

  file << generatedScript;
  file.flush();
  file.close();
}
//----------------------------------------------------------------------------------------------
/** Generate the line of script corresponding to the given AlgorithmHistory
*
* @param algHist :: AlgorithmHistory to generate a line in the script for.
* @returns - the generated string for the given algorithm.
*/
std::string GeneratePythonScript::genAlgString(const API::AlgorithmHistory &algHist)
{
  std::string algString = "";

  // Get the details of this algorithm history.
  const std::string name = algHist.name();
  const int version = algHist.version();

  // Create an unmanaged version of the algorithm, with witch we can compare the parameters later.
  const IAlgorithm_sptr ialg_Sptr = AlgorithmManager::Instance().createUnmanaged(name,version);
  if(ialg_Sptr)
  {
    ialg_Sptr->initialize();
  }

  // Get the properties of this algorithm history, loop through them, and generate
  // a string with the appropriate parameters.
  std::vector<Kernel::PropertyHistory> props = algHist.getProperties();
  std::vector<Kernel::PropertyHistory>::iterator propsIter = props.begin();

  for( ; propsIter != props.end(); ++propsIter)
  {
    std::string paramString = genParamString(*propsIter, ialg_Sptr, name);

    // Miss out parameters that are empty.
    if(paramString.length() != 0)
    {
      if(algString.length() != 0)
      {
        algString += ",";
      }
      algString += paramString;
    }
  }

  return name + "(" + algString + ")";
}
//----------------------------------------------------------------------------------------------
/** Generate the parameter string (of format "[name]='[value]'") for the given PropertyHistory.
*
* @param propHist :: PropertyHistory of the parameter.
* @param ialg_Sptr :: An unmanaged version of the algorithm this parameter belongs to.
* @param algHistName :: The name of the AlgorithHistory.
* @returns - the generated string for the given parameter.
*/
std::string GeneratePythonScript::genParamString(
  const Kernel::PropertyHistory &propHist,
  const API::IAlgorithm_sptr ialg_Sptr,
  const std::string algHistName)
{
  std::string params = "";

  const std::string name = sanitizePropertyName(propHist.name());
  const std::string value = propHist.value();
  const unsigned int direction = propHist.direction();

  // See if the the property is an Output workspace.
  bool outputWkspace = false;

  // Fit can create more properties after setting Function and InputWorkspace[_#]
  if ( algHistName == "Fit" &&
    (name == "Function" || name.substr(0,14) == "InputWorkspace"))
  {
    ialg_Sptr->setPropertyValue(name, value);
  }

  if( ialg_Sptr->existsProperty(name) && direction == Mantid::Kernel::Direction::Output )
  {
    Property *p = ialg_Sptr->getProperty(name);
    if( dynamic_cast<IWorkspaceProperty*>(p) ) outputWkspace = true;
  }

  // Only non-default properties should be included in the paramter list.
  if(!propHist.isDefault())
  {
    // If the property name occurs in the unmanaged version of the Algorithm, then
    // we should include it in the parameter list.
    if( (algHistName == "Load"  || ialg_Sptr->existsProperty(name) ) &&
      (direction == Direction::Input || direction == Direction::InOut || outputWkspace) )
    {
      if(params.length() != 0)
      {
        params += ",";
      }

      // Try and cast the property to a MultiFileProperty or FileProperty type.  If successful, we have to guard against
      // the the occurence of directory separators (forward or back slashes) in the file path that the
      // property contains. We do this by appending "r" onto the parameter in the output, to make it
      // a Python "raw" string.
      Property *p = ialg_Sptr->getPointerToProperty(name);
      auto* fp = dynamic_cast<FileProperty*>(p);
      auto* mp = dynamic_cast<MultipleFileProperty*>(p);
      if(fp || mp) params += name + "=r'" + value + "'";
      else params += name + "='" + value + "'";
    }
  }

  return params;
}
//----------------------------------------------------------------------------------------------
/** Sanitises the property name.
*/
std::string GeneratePythonScript::sanitizePropertyName(const std::string & name)
{
  std::string arg;
  std::string::const_iterator sIter = name.begin();
  std::string::const_iterator sEnd = name.end();

  for( ; sIter != sEnd; ++sIter )
  {
    int letter = (int)(*sIter);

    if( (letter >= 48 && letter <= 57) || (letter >= 97 && letter <= 122) ||
    (letter >= 65 && letter <= 90) )
    {
      arg.push_back(*sIter);
    }
  }
  return arg;
}

} // namespace Algorithms
} // namespace Mantid
