#ifndef MANTID_ALGORITHMS_GENERATEPYTHONSCRIPTTEST_H_
#define MANTID_ALGORITHMS_GENERATEPYTHONSCRIPTTEST_H_

#include <cxxtest/TestSuite.h>
#include "MantidKernel/Timer.h"
#include "MantidKernel/System.h"
#include <iostream>
#include <fstream>
#include <iomanip>

#include "MantidAlgorithms/GeneratePythonScript.h"
#include "MantidDataHandling/Load.h"
#include "MantidAPI/AlgorithmManager.h"
#include <Poco/File.h>

using namespace Mantid;
using namespace Mantid::Algorithms;
using namespace Mantid::API;

class GeneratePythonScriptTest : public CxxTest::TestSuite
{
public:
    void test_Init()
    {
        GeneratePythonScript alg;
        TS_ASSERT_THROWS_NOTHING( alg.initialize() )
        TS_ASSERT( alg.isInitialized() )
    }

    void test_exec()
    {
        // Load test file into workspace
        Mantid::DataHandling::Load loader;
        loader.initialize();
        // Here we make the assumption that IRS26173_ipg.nxs will not change, and so is
        // a valid choice to test the output of the Algorithm against.
        loader.setPropertyValue("Filename", "IRS26173_ipg.nxs");
        loader.setPropertyValue("OutputWorkspace","LoadedWorkspace");
        loader.setRethrows(true);

        std::string result[] = {
            "######################################################################",
            "#Python Script Generated by GeneratePythonScript Algorithm",
            "######################################################################",
            "LoadRaw(Filename=r'G:/Spencer/Science/Raw/irs26173.raw',OutputWorkspace='IPG',SpectrumMin='3',SpectrumMax='53')",         // Not tested.
            "ConvertUnits(InputWorkspace='IPG',OutputWorkspace='Spec',Target='Wavelength')", 
            "LoadRaw(Filename=r'G:/Spencer/Science/Raw/irs26173.raw',OutputWorkspace='Mon_in',SpectrumMax='1')",                       // Not tested.
            "Unwrap(InputWorkspace='Mon_in',OutputWorkspace='Mon',LRef='37.86')",
            "RemoveBins(InputWorkspace='Mon',OutputWorkspace='Mon',XMin='6.14600063416',XMax='6.14800063416',Interpolation='Linear')",
            "FFTSmooth(InputWorkspace='Mon',OutputWorkspace='Mon')",
            "RebinToWorkspace(WorkspaceToRebin='Spec',WorkspaceToMatch='Mon',OutputWorkspace='Spec')",
            "LoadRaw(Filename=r'G:/Spencer/Science/Raw/irs26173.raw',OutputWorkspace='Mon_in',SpectrumMax='1')",                       // Not tested.
            "Unwrap(InputWorkspace='Mon_in',OutputWorkspace='Mon',LRef='37.86')",
            "RemoveBins(InputWorkspace='Mon',OutputWorkspace='Mon',XMin='6.14600063416',XMax='6.14800063416',Interpolation='Linear')",
            "FFTSmooth(InputWorkspace='Mon',OutputWorkspace='Mon')",
            "Divide(LHSWorkspace='Spec',RHSWorkspace='Mon',OutputWorkspace='Spec')",
            "ConvertUnits(InputWorkspace='Spec',OutputWorkspace='Spec',Target='DeltaE',EMode='Indirect',EFixed='1.84')",
            "GroupDetectors(InputWorkspace='Spec',OutputWorkspace='IPG_3',MapFile=r'G:/Spencer/Science/Mantid/IRIS/PG1op3.map')",      // Not tested.
            "Load(Filename=r'C:/Mantid/Test/AutoTestData/IRS26173_ipg.nxs',OutputWorkspace='IRS26173_ipg')",                           // Not tested.
            ""
        };

        TS_ASSERT_THROWS_NOTHING(loader.execute());
        TS_ASSERT_EQUALS(loader.isExecuted(), true);

        MatrixWorkspace_sptr ws = AnalysisDataService::Instance().retrieveWS<MatrixWorkspace>("LoadedWorkspace");

        TS_ASSERT(NULL != ws);

        // Set up and execute the algorithm.
        GeneratePythonScript alg;
        TS_ASSERT_THROWS_NOTHING( alg.initialize() );
        TS_ASSERT( alg.isInitialized() );
        TS_ASSERT_THROWS_NOTHING( alg.setPropertyValue("InputWorkspace", ws->getName()) ); 
        TS_ASSERT_THROWS_NOTHING( alg.setPropertyValue("Filename", "GeneratePythonScriptTest.py") );
        TS_ASSERT_THROWS_NOTHING( alg.execute(); );
        TS_ASSERT( alg.isExecuted() );

        // Read in the file, and parse each line into a vector of strings.
        std::string filename = alg.getProperty("Filename");
        std::ifstream file(filename.c_str(), std::ifstream::in);
        std::vector<std::string> lines;

        while (file.good())
        {
            char testArray[256];
            file.getline(testArray, 256);
            std::string line = testArray;
            lines.push_back(line);
        }

        std::vector<std::string>::iterator lineIter = lines.begin();

        int lineCount = 0;

        // Compare the contents of the file to the expected result line-by-line.
        // If the line contains Filename or MapFile then just check that the string
        // is prefixed with r to convert it to a Python raw string and not the actual content
        // as the file paths are different
        for( ; lineIter != lines.end(); ++lineIter) 
        {
          const std::string & algLine = *lineIter;
          std::string::size_type filenamePos = algLine.find("Filename=");
          std::string::size_type mapfilePos = algLine.find("MapFile=");
          if(filenamePos != std::string::npos)
          {
            TS_ASSERT_EQUALS(algLine[filenamePos+9], 'r');
          }
          else if(mapfilePos != std::string::npos)
          {
            TS_ASSERT_EQUALS(algLine[mapfilePos+8], 'r');
          }
          else
          {
            TS_ASSERT_EQUALS(algLine,result[lineCount]);
          }
          lineCount++;
        }

        // Remove workspace from the data service.
        // AnalysisDataService::Instance().remove(outWSName);
        file.close();
        if (Poco::File(filename).exists()) Poco::File(filename).remove();
    }
};


#endif /* MANTID_ALGORITHMS_GENERATEPYTHONSCRIPTTEST_H_ */

