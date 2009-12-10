//----------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------
#include "MantidDataHandling/LoadLog.h"
#include "MantidAPI/LogParser.h"
#include "MantidKernel/TimeSeriesProperty.h"
#include "MantidKernel/PropertyWithValue.h"
#include "MantidDataObjects/Workspace2D.h"
#include "MantidKernel/Glob.h"
#include "MantidKernel/FileProperty.h"
#include "LoadRaw/isisraw2.h"

#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/RegularExpression.h"

#include <fstream>  // used to get ifstream
#include <sstream>

namespace Mantid
{
namespace DataHandling
{

// Register the algorithm into the algorithm factory
DECLARE_ALGORITHM(LoadLog)

using namespace Kernel;
using API::WorkspaceProperty;
using API::MatrixWorkspace;
using API::MatrixWorkspace_sptr;
using DataObjects::Workspace2D;
using DataObjects::Workspace2D_sptr;

/// Empty default constructor
LoadLog::LoadLog()
{}

/// Initialisation method.
void LoadLog::init()
{
  // When used as a sub-algorithm the workspace name is not used - hence the "Anonymous" to satisfy the validator
  declareProperty(
    new WorkspaceProperty<MatrixWorkspace>("Workspace","Anonymous",Direction::InOut),
    "The name of the workspace to which the log data will be added");

  std::vector<std::string> exts(4, "");
  exts[0] = "txt";
  exts[1] = "raw";
  exts[2] = "s*";
  exts[3] = "add";
  declareProperty(new FileProperty("Filename", "", FileProperty::Load, exts),
		  "The filename (including its full or relative path) of either an ISIS log file\n"
		  "or an ISIS raw file. If a raw file is specified all log files associated with\n"
		  "that raw file are loaded into the specified workspace. The file extension must\n"
		  "either be .raw or .s when specifying a raw file");

  declareProperty("Period",1);
}

  //@cond NODOC
  namespace
  {
    struct FileMatcher
    {
      FileMatcher(const std::string & expression) : m_expression(expression) {}

      bool operator()(const std::string & test) const
      {
	Poco::RegularExpression regex(m_expression, Poco::RegularExpression::RE_CASELESS);
	return regex.match(test);
      }
      
    private:
      FileMatcher();
      const std::string m_expression;
    };
  }
  //@endcond

/** Executes the algorithm. Reading in ISIS log file(s)
 * 
 *  @throw Mantid::Kernel::Exception::FileError  Thrown if file is not recognised to be a raw datafile or log file
 *  @throw std::runtime_error Thrown with Workspace problems
 */
void LoadLog::exec()
{
  // Retrieve the filename from the properties and perform some initial checks on the filename

  m_filename = getPropertyValue("Filename");

  // File property checks whether the given path exists, just check that is actually a file 
  Poco::File l_path( m_filename );
  if ( l_path.isDirectory() )
  {
    g_log.error("In LoadLog: " + m_filename + " must be a filename not a directory.");
    throw Exception::FileError("Filename is a directory:" , m_filename);
  }

  // Get the input workspace and retrieve sample from workspace.
  // the log file(s) will be loaded into the Sample container of the workspace 

  const MatrixWorkspace_sptr localWorkspace = getProperty("Workspace");
  boost::shared_ptr<API::Sample> sample = localWorkspace->getSample();

  // If m_filename is the filename of a raw datafile then search for potential log files
  // in the directory of this raw datafile. Otherwise check if m_filename is a potential
  // log file. Add the filename of these potential log files to: potentialLogFiles.
  std::set<std::string> potentialLogFiles;
  
  // start the process or populating potential log files into the container: potentialLogFiles
  std::string l_filenamePart = Poco::Path(l_path.path()).getFileName();// get filename part only
   std::string threecolumnLogfile;
  bool rawFile = false;// Will be true if Filename property is a name of a RAW file
  if ( isAscii(m_filename) && l_filenamePart.find("_") != std::string::npos )
  {
    // then we will assume that m_filename is an ISIS log file
    potentialLogFiles.insert(m_filename);
  }
  else
  {
    // then we will assume that m_filename is an ISIS raw file. The file validator will have warned the user if the
    // extension is not one of the suggested ones
    rawFile = true;
    // strip out the raw data file identifier
    std::string l_rawID("");
    size_t idx = l_filenamePart.rfind('.');
    if( idx != std::string::npos )
    {
      l_rawID = l_filenamePart.substr(0, l_filenamePart.rfind('.'));
    }
    else
    {
      l_rawID = l_filenamePart;
    }
	/// check for alternate data stream exists for raw file
	/// if exists open the stream and read  log files name  from ADS
	if(adsExists())potentialLogFiles=getLogfilenamesfromADS();

	else
	{
		// look for log files in the directory of the raw datafile
		std::string pattern(l_rawID + "_*.txt");
		Poco::Path dir(m_filename);
		dir.makeParent();
		try
		{
			Kernel::Glob::glob(Poco::Path(dir).resolve(pattern),potentialLogFiles);
		}
		catch(std::exception &)
		{
		}

		if( potentialLogFiles.empty() )
		{
			Poco::RegularExpression regex(l_rawID + "_.*\\.txt", Poco::RegularExpression::RE_CASELESS );
			Poco::DirectoryIterator end_iter;
			for ( Poco::DirectoryIterator dir_itr(Poco::Path(m_filename).parent()); dir_itr != end_iter; ++dir_itr )
			{
				if ( !Poco::File(dir_itr->path() ).isFile() ) continue;

				l_filenamePart = Poco::Path(dir_itr->path()).getFileName();

				if ( regex.match(l_filenamePart) )
				{
					potentialLogFiles.insert( dir_itr->path() );
				}
			}

		}
	}
	//.if a .log file exists in the raw file directory
	if (threeColumnFormatLogFileExists())
	{	
		threecolumnLogfile=getThreecolumnFormatLogFile();
		std::set<std::string> blockFileNameList=createthreecolumnFileLogProperty(threecolumnLogfile,sample);
		//remove the file name from potential logfiles list if it's there in the .log file.
		std::set<std::string>::const_iterator itr;
		for(itr=blockFileNameList.begin();itr!=blockFileNameList.end();++itr)
		{	
			std::set<std::string>::iterator litr= find(potentialLogFiles.begin(),potentialLogFiles.end(),*itr);
			if(litr!=potentialLogFiles.end())
			{ potentialLogFiles.erase(litr);}
		}
	}
  }
 
  //If there are no log files by now, we have nothing else to do
  if( potentialLogFiles.empty() ) return;

  //Do a quick search for the icpevent file
  std::string icpevent_file_name("");
  std::set<std::string>::const_iterator icpfile = find_if(potentialLogFiles.begin(), potentialLogFiles.end(), FileMatcher(std::string(".*icpevent.*")));
  if( icpfile != potentialLogFiles.end() )
  {
    icpevent_file_name = *icpfile;
  }

  API::LogParser parser(icpevent_file_name);
  // Add mantid-created logs
  int period = getProperty("Period");
  Property* log = parser.createPeriodLog(period);
  if (log)
  {
    sample->addLogData(log);
  }
  sample->addLogData(parser.createAllPeriodsLog());
  sample->addLogData(parser.createRunningLog());

  // Extract the common part of log file names (the workspace name)
  std::string ws_name = Poco::Path(m_filename).getFileName();
  ws_name.erase(ws_name.find_last_of('.'));
  ws_name += '_';
  size_t n_common_chars = ws_name.size();

  // Attempt to load the content of each potential log file into the Sample object
  std::set<std::string>::const_iterator logs_end = potentialLogFiles.end();
  for(std::set<std::string>::const_iterator logs_itr = potentialLogFiles.begin(); logs_itr != logs_end; ++logs_itr)
  {
    std::string filename = *logs_itr;
    // open log file
    std::ifstream inLogFile(filename.c_str());

    if (!inLogFile)
    {
      // Unable to open file
      g_log.error("Unable to open file " + filename);
      throw Exception::FileError("Unable to open file:" , filename);
    }
    // figure out if second column is a number or a string
    std::string aLine;
    if( std::getline(inLogFile, aLine, '\n') )
    {
      if ( !isDateTimeString(aLine) )
      {
        g_log.warning("File" + filename + " is not a standard ISIS log file. Expected to be a two column file.");
        inLogFile.close();
        continue;
      }
      std::string dateAndTime;
      std::stringstream ins(aLine);
      ins >> dateAndTime;

      // read in what follows the date-time string in the log file and figure out
      // what type it is
      std::string whatType;
      ins >> whatType;
      kind l_kind = classify(whatType);
      if ( LoadLog::string != l_kind && LoadLog::number != l_kind )
      {
        g_log.warning("ISIS log file contains unrecognised second column entries: " + filename);
        inLogFile.close();
        continue;
      }
      
      try
      {
	// Make the property name by removing the workspce name and file extension from the log filename
	std::string log_name = Poco::Path(Poco::Path(filename).getFileName()).getBaseName();
	
	if (rawFile)
	{
	  log_name.erase(0, n_common_chars);
	}
	
	Property* log = parser.createLogProperty(*logs_itr,stringToLower(log_name));
	if (log)
	{
	  sample->addLogData(log);
	}
      }
      catch(std::exception&)
      {
	continue;
      }
      
    } 
    inLogFile.close();
  } // end for


  // operation was a success and ended normally
  return;
}
/** this method looks in the rawfile directory for  rawfilename.log (three column format )file 
    @returns true if the file exists
*/
bool LoadLog::threeColumnFormatLogFileExists()
{  
	std::string rawID;
	size_t pos = m_filename.find(".");
   	if(pos!=std::string::npos)
		rawID=m_filename.substr(0,pos);
	// append .log to get the .log file name
	 std::string logfileName=rawID+".log";	
	 int count=0;
	 if (Poco::File(logfileName).exists())
	 {
		 //validate the file
		 std::ifstream inLogFile(logfileName.c_str());
		 if (!inLogFile)
		 { throw Exception::FileError("Unable to open file:" ,logfileName );}

//check if first 19 characters of a string is data-time string according to yyyy-mm-ddThh:mm:ss
		 std::string aLine;
		 kind l_kind(LoadLog::empty);
		 //if( std::getline(inLogFile, aLine, '\n') )
		 while(Mantid::API::extractToEOL(inLogFile,aLine))
		 {			 
			 if ( !isDateTimeString(aLine) )
			 { g_log.warning("File" + logfileName + " is not a standard ISIS log file. Expected to be a file starting with DateTime String format.");
				 inLogFile.close();
				 return false;
			 }

			 std::stringstream ins(aLine);
			 std::string firstcolumn;
			 ins >> firstcolumn;
			 // read in what follows the date-time string in the log file and figure out
			 // what type it is
			 std::string secondcolumn;
			 ins >> secondcolumn;
			 l_kind = classify(secondcolumn);
			 if ( LoadLog::string != l_kind )
			 {
				 g_log.warning("ISIS log file contains unrecognised second column entries: " + logfileName);
				 inLogFile.close();
				 return false;
			 }

			 std::string thirdcolumn;
			 ins>>thirdcolumn;
			 l_kind = classify(thirdcolumn);
			 if ( LoadLog::string != l_kind && LoadLog::number!=l_kind)
			 {
				 g_log.warning("ISIS log file contains unrecognised third column entries: " + logfileName);
				 inLogFile.close();
				 return false;
			 }
			 ++count;
			 if(count==2) ///reading first two lines from file for validation purpose.
				 break;
		 }
		return true;
	 }
	else return false;
}
/* this method looks for ADS with name checksum exists
*@return if ADS stream checkum  exists
*/
bool LoadLog::adsExists()
{
	std::string adsname(m_filename+":checksum");
	std::ifstream adstream(adsname.c_str());
	if(!adstream)
	{return false;
	}
	return true;
}
/* this method reads  the checksum ADS associated with the
* raw file and returns the filensmes of the log files
*@return list of logfile names.
*/
std::set<std::string> LoadLog::getLogfilenamesfromADS()
{	
	std::string adsname(m_filename+":checksum");
	std::ifstream adstream(adsname.c_str());
	if(!adstream)
		return std::set<std::string>();
	std::string str;
	std::string path;
	std::string logFile;
	std::set<std::string>logfilesList;
	Poco::Path logpath(m_filename);
	std::string p=logpath.home();
	size_t pos =m_filename.find_last_of("/");
	if(pos==std::string::npos)
	{
		pos =m_filename.find_last_of("\\");
	}
	if(pos!=std::string::npos)
		 path=m_filename.substr(0,pos);
	while(Mantid::API::extractToEOL(adstream,str))
	{
		std::string fileName;
		pos = str.find("*");
		if(pos==std::string::npos)
			continue;
 	    fileName=str.substr(pos+1,str.length()-pos);
		pos= fileName.find("txt");
		if(pos==std::string::npos)
			continue;
        logFile=path+"\\"+fileName;
		if(logFile.empty())
			continue;
		logfilesList.insert(logFile);
	}
	return logfilesList;
}

/**  this method returns the name of three column log file 
    @returns file name of the.log file
*/
std::string LoadLog::getThreecolumnFormatLogFile()
{
	size_t pos = m_filename.find(".");
    std::string rawID;
	if(pos!=std::string::npos)
		rawID=m_filename.substr(0,pos);
	// append .log to get the .log file name
	 std::string logfileName=rawID+".log";	
	 if (Poco::File(logfileName).exists())
		 return logfileName;
	 else return "";
}
/**
This method reads the.log file and creates timeseries property and sets that to the sample object
*@param logfile three column log(.log) file name.
*@param sample sample object
@returns list of logfiles which exists as blockname in the .log file
*/
std::set<std::string>  LoadLog::createthreecolumnFileLogProperty(const std::string& logfile,boost::shared_ptr<API::Sample> sample)
{    
	std::basic_string <char>::size_type pos=m_filename.find(".");
	std::string path =m_filename.substr(0,pos);
	std::set<std::string> blockFileNameList;

	std::ifstream file(logfile.c_str());
	if (!file)
	{	g_log.warning()<<"Cannot open log file "<<logfile<<"\n";
		return std::set<std::string>();
	}
    // Read in the data and determin if it is numeric
	std::string str,old_data,old_blockcolumn;
    bool isNumeric(false);
    std::string sdata;
	std::string propname;
	Mantid::Kernel::TimeSeriesProperty<double>* logd=0;
	Mantid::Kernel::TimeSeriesProperty<std::string>* logs=0;
	std::map<std::string,Kernel::TimeSeriesProperty<double>*> dMap;
	std::map<std::string,Kernel::TimeSeriesProperty<std::string>*> sMap;
    // MG 22/09/09: If the log file was written on a Windows machine and then read on a Linux machine, std::getline will
    // leave CR at the end of the string and this causes problems when reading out the log values. Mantid::extractTOEOL
    // extracts all EOL characters
	while(Mantid::API::extractToEOL(file,str))
	{
		if (!Kernel::TimeSeriesProperty<double>::isTimeString(str)) 
		{    //if the line doesn't start with a time read the next line
			continue;
		}
		std::stringstream line(str);
		std::string timecolumn;
		line>>timecolumn;
		std::string blockcolumn;
		line>>blockcolumn;
		std::string valuecolumn;
		line>>valuecolumn;
		sdata=valuecolumn;

		///column two in .log file is called block column
		///if any .txt file with rawfilename_blockcolumn.txt exists
		/// donot load that txt  files
		/// blockFileNameList conatins the file names to be removed from potentiallogfiles list.
		std::string blockcolumnFileName=path+"_"+blockcolumn+".txt";
		/*pos=blockcolumnFileName.find("/");
		while (pos!=std::string::npos)
		{	blockcolumnFileName.replace(pos,1,"\\");
		pos=blockcolumnFileName.find("/");
		}*/
		if(blockcolumnFileExists(blockcolumnFileName))
		{	  blockFileNameList.insert(blockcolumnFileName);
		}
		propname=stringToLower(blockcolumn);
		//check if the data is numeric
		std::istringstream istr(valuecolumn);
		double dvalue;
		istr >> dvalue;
		isNumeric = !istr.fail();

		if (isNumeric)
		{				
			std::map<std::string,Kernel::TimeSeriesProperty<double>*>::iterator ditr=dMap.find(propname);
			if(ditr!=dMap.end())
			{	Kernel::TimeSeriesProperty<double>* p=ditr->second;
				if(p)p->addValue(timecolumn,dvalue);
				dMap[propname]=p;
			}
			else
			{	logd = new Kernel::TimeSeriesProperty<double>(propname);
				logd->addValue(timecolumn,dvalue);
				dMap[propname]=logd;
			}
		}
		else
		{		
			std::map<std::string,Kernel::TimeSeriesProperty<std::string>*>::iterator sitr=sMap.find(propname);
			if(sitr!=sMap.end())
			{	Kernel::TimeSeriesProperty<std::string>* prop=sitr->second;
			if(prop) prop->addValue(timecolumn,valuecolumn);
		    	sMap[propname]=prop;
			}
			else
			{	logs = new Kernel::TimeSeriesProperty<std::string>(propname);
				logs->addValue(timecolumn,valuecolumn);
				sMap[propname]=logs;}
		}
	}
	try
	{
		std::map<std::string,Kernel::TimeSeriesProperty<double>*>::const_iterator itr=dMap.begin();
		for(;itr!=dMap.end();++itr)
		{
			sample->addLogData(itr->second);
		}
		std::map<std::string,Kernel::TimeSeriesProperty<std::string>*>::const_iterator sitr=sMap.begin();
		for(;sitr!=sMap.end();++sitr)
		{
			sample->addLogData(sitr->second);
		}
	}
	catch(std::invalid_argument &e)
	{
		g_log.warning()<<e.what();
	}
	catch(Exception::ExistsError&e)
	{
		g_log.warning()<<e.what();
	}
	
   return blockFileNameList;
	
}
/** this method looks for file with second column(block column) name exists in the raw file directory
*@param fileName -name of the file
*@returns true if the file exists
*/
bool LoadLog::blockcolumnFileExists(const std::string& fileName)
{ if (Poco::File(fileName).exists())
		return true;
	 else return false;
}

/** Takes as input a string and try to determine what type it is.
 *  @param s The input string
 *  @param s  string to be classified
 *  @return A enum kind which tells what type the string is
 */
LoadLog::kind LoadLog::classify(const std::string& s)
{
  if( s.empty() )
  {
    return LoadLog::empty;
  }

  using std::string;
  const string lower("abcdefghijklmnopqrstuvwxyz");
  const string upper("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  const string letters = lower + upper + '_';

  if (letters.find_first_of(s) != string::npos)
  {
    return LoadLog::string;
  }
  else
  {
    return LoadLog::number;
  }
}

/** change each element of the string to lower case
* @param strToConvert The input string
* @returns The string but with all characters in lower case
*/
std::string LoadLog::stringToLower(std::string strToConvert)
{
  std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), tolower);
  return strToConvert;
}

/** Checks whether filename is a simple text file
* @param filename The filename to inspect
* @returns true if the filename has the .txt extension
*/
bool LoadLog::isAscii(const std::string& filename)
{
  FILE* file = fopen(filename.c_str(), "rb");
  char data[256];
  int n = fread(data, 1, sizeof(data), file);
  char *pend = &data[n];
  /*
   * Call it a binary file if we find a non-ascii character in the 
   * first 256 bytes of the file.
   */
  for( char *p = data;  p < pend; ++p )
  {
    unsigned long ch = (unsigned long)*p;
    if( !(ch <= 0x7F) )
    {
      return false;
    }
    
  }
  return true;
}

/** check if first 19 characters of a string is date-time string according to yyyy-mm-ddThh:mm:ss
* @param str The string to test
* @returns true if the strings format matched the expected date format
*/
bool LoadLog::isDateTimeString(const std::string& str)
{
  Poco::DateTime dt;
  int tz_diff;
  return Poco::DateTimeParser::tryParse(Poco::DateTimeFormat::ISO8601_FORMAT, str.substr(0,19), dt, tz_diff);
}

} // namespace DataHandling
} // namespace Mantid
