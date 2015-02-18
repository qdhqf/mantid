import os,sys,inspect
#os.environ["PATH"] = r"c:/Mantid/Code/builds/br_master/bin/Release;"+os.environ["PATH"]
from mantid.simpleapi import *
from mantid import api
import unittest
import numpy as np
from Direct.PropertyManager import PropertyManager
from Direct.RunDescriptor   import RunDescriptor


#-----------------------------------------------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------------------------
#-----------------------------------------------------------------------------------------------------------------------------------------
class RunDescriptorTest(unittest.TestCase):

    def __init__(self, methodName):
        self.prop_man = PropertyManager("MAR")
        return super(RunDescriptorTest, self).__init__(methodName)

    def setUp(self):
        if self.prop_man == None or type(self.prop_man) != type(PropertyManager):
            self.prop_man  = PropertyManager("MAR");
    def tearDown(self):
        api.AnalysisDataService.clear()

    @staticmethod
    def getInstrument(InstrumentName='MAR'):
        """ test method used to obtain default instrument for testing """
        idf_dir = config.getString('instrumentDefinition.directory')
        idf_file=api.ExperimentInfo.getInstrumentFilename(InstrumentName)
        tmp_ws_name = '__empty_' + InstrumentName
        if not mtd.doesExist(tmp_ws_name):
               LoadEmptyInstrument(Filename=idf_file,OutputWorkspace=tmp_ws_name)
        return mtd[tmp_ws_name].getInstrument()

 
    def test_descr_basic(self):
        propman  = PropertyManager('MAR')

        self.assertTrue(propman.sample_run is None)
        self.assertTrue(PropertyManager.sample_run.get_workspace() is None)
        
        propman.sample_run = 10;
        self.assertEqual(propman.sample_run,10)

        run_ws = CreateSampleWorkspace( Function='Multiple Peaks', NumBanks=1, BankPixelWidth=4, NumEvents=100)
        propman.sample_run = 'run_ws'
        self.assertEqual(PropertyManager.sample_run.run_number(),0)

        stor_ws = PropertyManager.sample_run.get_workspace()
        rez = CheckWorkspacesMatch(Workspace1=run_ws,Workspace2=stor_ws)

        self.assertEqual(rez,'Success!')


    def test_descr_dependend(self):
        propman  = self.prop_man
        propman.wb_run = 100
        self.assertTrue(PropertyManager.wb_run.has_own_value())
        self.assertEqual(propman.wb_run,100)
        self.assertEqual(propman.wb_for_monovan_run,100)
        self.assertFalse(PropertyManager.wb_for_monovan_run.has_own_value())
        self.assertTrue(PropertyManager.wb_run.has_own_value())

        propman.wb_for_monovan_run = 200
        self.assertEqual(propman.wb_for_monovan_run,200)
        self.assertEqual(propman.wb_run,100)
        self.assertTrue(PropertyManager.wb_run.has_own_value())
        self.assertTrue(PropertyManager.wb_for_monovan_run.has_own_value())

        propman.wb_for_monovan_run = None
        self.assertFalse(PropertyManager.wb_for_monovan_run.has_own_value())
        self.assertEqual(propman.wb_for_monovan_run,100)
        self.assertEqual(propman.wb_run,100)



    def test_find_file(self):
        propman  = self.prop_man
        propman.sample_run = 11001

        ok,file=PropertyManager.sample_run.find_file()
        self.assertTrue(ok)
        self.assertTrue(len(file)>0)

        ext = PropertyManager.sample_run.get_file_ext()
        self.assertEqual(ext,'.raw')

        PropertyManager.sample_run.set_file_ext('nxs')
        ext = PropertyManager.sample_run.get_file_ext()
        self.assertEqual(ext,'.nxs')

        test_dir = config.getString('defaultsave.directory')

        testFile1=os.path.normpath(test_dir+'MAR101111.nxs')
        testFile2=os.path.normpath(test_dir+'MAR101111.raw')

        # create two test files to check search for appropriate extension
        f=open(testFile1,'w')
        f.write('aaaaaa');
        f.close()

        f=open(testFile2,'w')
        f.write('bbbb')
        f.close()


        propman.sample_run = 101111
        PropertyManager.sample_run.set_file_ext('nxs')

        ok,file=PropertyManager.sample_run.find_file()
        self.assertEqual(testFile1,os.path.normpath(file))
        PropertyManager.sample_run.set_file_ext('.raw')
        ok,file=PropertyManager.sample_run.find_file()
        self.assertEqual(testFile2,os.path.normpath(file))

        os.remove(testFile1)
        os.remove(testFile2)

    def test_load_workspace(self):
        propman  = self.prop_man

        # MARI run with number 11001 and extension raw must among unit test files
        propman.sample_run = 11001
        PropertyManager.sample_run.set_file_ext('nxs')

        ws = PropertyManager.sample_run.get_workspace()
        self.assertEqual(PropertyManager.sample_run.get_file_ext(),'.raw')

        self.assertTrue(isinstance(ws, api.Workspace))

        mon_ws = PropertyManager.sample_run.get_monitors_ws()
        self.assertTrue(isinstance(mon_ws, api.Workspace))
        self.assertEqual(mon_ws.name(),ws.name()) 
    
    def test_copy_spectra2monitors(self):
        propman  = self.prop_man
        run_ws = CreateSampleWorkspace( Function='Multiple Peaks', WorkspaceType = 'Event',NumBanks=1, BankPixelWidth=5, NumEvents=100)
        run_ws_monitors = CreateSampleWorkspace( Function='Multiple Peaks', WorkspaceType = 'Histogram',NumBanks=2, BankPixelWidth=1, NumEvents=100)
        self.assertEqual(run_ws_monitors.getNumberHistograms(),2)


        propman.monovan_run = run_ws
        propman.spectra_to_monitors_list = 3

        mon_ws = PropertyManager.monovan_run.get_monitors_ws()
        self.assertTrue(isinstance(mon_ws, api.Workspace))
        self.assertEqual(mon_ws.getNumberHistograms(),3)
        self.assertEqual(mon_ws.getIndexFromSpectrumNumber(3),2)

    def test_ws_name(self):
        run_ws = CreateSampleWorkspace( Function='Multiple Peaks', NumBanks=1, BankPixelWidth=4, NumEvents=100)
        propman  = self.prop_man
        propman.sample_run = run_ws

        ws = PropertyManager.sample_run.get_workspace()
        self.assertEqual(ws.name(),'SR_run_ws')

        propman.sample_run = ws
        ws = PropertyManager.sample_run.get_workspace()
        self.assertEqual(ws.name(),'SR_run_ws')
        self.assertTrue('SR_run_ws' in mtd)

        propman.sample_run = 11001
        self.assertFalse('SR_run_ws' in mtd)

        propman.load_monitors_with_workspace = False
        ws = PropertyManager.sample_run.get_workspace()
        ws_name = ws.name()
        self.assertEqual('SR_MAR011001',ws_name)
        self.assertTrue(ws_name in mtd)
        self.assertTrue(ws_name+'_monitors' in mtd)

        propman.sample_run = ws
        self.assertTrue(ws_name in mtd)

        ws1 = PropertyManager.sample_run.get_workspace()
        self.assertEqual(ws1.name(),ws_name)
        #
        PropertyManager.sample_run.set_action_suffix('_modified')
        PropertyManager.sample_run.synchronize_ws(ws1)

        ws1 = PropertyManager.sample_run.get_workspace()
        self.assertTrue(str.find(ws1.name(),'_modified')>0)

        propman.sample_run = ws1
        self.assertEqual(ws1.name(),PropertyManager.sample_run._ws_name)

        ws = PropertyManager.sample_run.get_workspace()
        ws_name = ws.name()


        # if no workspace is available, attempt to get workspace name fails
        DeleteWorkspace(ws_name)

        propman.sample_run = None
        self.assertFalse(ws_name+'_monitors' in mtd)
        # name of empty property workspace 
        #self.assertEqual(PropertyManager.sample_run.get_ws_name(),'SR_')

        
    def test_assign_fname(self):
        propman  = self.prop_man
        propman.sample_run = 'MAR11001.RAW'

        self.assertEqual(PropertyManager.sample_run.run_number(),11001)
        self.assertEqual(PropertyManager.sample_run._run_ext,'.RAW')

    def test_chop_ws_part(self):
        propman  = self.prop_man
        ws=CreateSampleWorkspace(Function='Multiple Peaks', NumBanks=4, BankPixelWidth=1, NumEvents=100, XUnit='TOF',
                                                     XMin=2000, XMax=20000, BinWidth=1)

        ws_monitors=CreateSampleWorkspace(Function='Multiple Peaks', NumBanks=4, BankPixelWidth=1, NumEvents=100, XUnit='TOF',
                                                    XMin=2000, XMax=20000, BinWidth=1)

        propman.sample_run = ws

        ws1 = PropertyManager.sample_run.chop_ws_part(None,(2000,1,5000),False,1,2)

        rez=CheckWorkspacesMatch('SR_ws',ws1)
        self.assertEqual(rez,'Success!')

        wsc=PropertyManager.sample_run.get_workspace()
        x =wsc.readX(0)
        self.assertAlmostEqual(x[0],2000)
        self.assertAlmostEqual(x[-1],5000)

        self.assertEqual(wsc.name(),'SR_#1/2#ws')
        self.assertTrue('SR_#1/2#ws_monitors' in mtd)



        ws1 = PropertyManager.sample_run.chop_ws_part(ws1,(10000,100,20000),True,2,2)
        x =ws1.readX(0)
        self.assertAlmostEqual(x[0],10000)
        self.assertAlmostEqual(x[-1],20000)

        self.assertEqual(ws1.name(),'SR_#2/2#ws')
        self.assertTrue('SR_#2/2#ws_monitors' in mtd)

    def test_run_list(self):
        pass

    def test_get_run_list(self):
        propman = PropertyManager('MAR')
        propman.sample_run = [10204]

        self.assertEqual(propman.sample_run,10204)
        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),1)
        self.assertEqual(runs[0],10204)

        # the same run list changes nothing
        propman.sample_run = [10204]
        self.assertEqual(propman.sample_run,10204)
        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),1)
        self.assertEqual(runs[0],10204)


        propman.sample_run = [11230,10382,10009]
        self.assertEqual(propman.sample_run,11230)
        propman.sum_runs = True
        self.assertEqual(propman.sample_run,10009)
        propman.sample_run = [11231,10382,10010]
        self.assertEqual(propman.sample_run,10010)

        sum_list,sum_ws,n_sum = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sum_list),3)
        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(runs[0],sum_list[0])

        # Autoreduction workflow with summation. Runs appear
        # one by one and summed together when next run appears
        propman.sample_run = 11231
        sum_list,sum_ws,n_sum = PropertyManager.sample_run.get_runs_to_sum()

        self.assertEqual(len(sum_list),1)
        self.assertEqual(sum_list[0],11231)
        self.assertEqual(propman.sample_run,11231)

        propman.sample_run = 10382
        sum_list,sum_ws,n_sum = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sum_list),2)
        self.assertEqual(sum_list[0],11231)
        self.assertEqual(sum_list[1],10382)
        self.assertEqual(propman.sample_run,10382)
        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),3)

        propman.sample_run = 10010
        sum_list,sum_ws,n_sum = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sum_list),3)
        self.assertEqual(sum_list[0],11231)
        self.assertEqual(sum_list[1],10382)
        self.assertEqual(sum_list[2],10010)
        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),3)
        self.assertTrue(propman.sum_runs)

        # check extend when summing
        propman.sample_run = 10999
        sum_list,sum_ws,n_sum = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sum_list),4)
        self.assertEqual(sum_list[0],11231)
        self.assertEqual(sum_list[1],10382)
        self.assertEqual(sum_list[2],10010)
        self.assertEqual(sum_list[3],10999)
        self.assertTrue(propman.sum_runs)

        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),4)
        self.assertTrue(propman.sum_runs)


        propman.sum_runs=False
        run_list = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(run_list),4)
        self.assertEqual(propman.sample_run,11231)
        sum_list,sum_ws,n_sum = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sum_list),0)

        # check clear list when not summing 
        propman.sample_run = 11999
        run_list = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(run_list),1)

    def test_sum_runs(self):
        propman  = self.prop_man
        propman.sample_run = [11001,11001]
        ws = PropertyManager.sample_run.get_workspace()
        test_val1 = ws.dataY(3)[0]
        test_val2 = ws.dataY(6)[100]
        test_val3 = ws.dataY(50)[200]
        self.assertEqual(ws.name(),'SR_MAR011001')
        self.assertEqual(ws.getNEvents(),2455286)

        #propman.sample_run = [11001,11001]
        propman.sum_runs = True
        self.assertFalse('SR_MAR011001' in mtd)
        ws = PropertyManager.sample_run.get_workspace()
        self.assertEqual(ws.name(),'SR_MAR011001SumOf2')
 
        self.assertEqual(2*test_val1, ws.dataY(3)[0])
        self.assertEqual(2*test_val2, ws.dataY(6)[100])
        self.assertEqual(2*test_val3, ws.dataY(50)[200])


        propman.sample_run = "MAR11001.raw,11001.nxs,MAR11001.raw"
        self.assertFalse('SR_MAR011001SumOf2' in mtd)
        ws = PropertyManager.sample_run.get_workspace()
        self.assertEqual(ws.name(),'SR_MAR011001SumOf3')
 
        self.assertEqual(3*test_val1, ws.dataY(3)[0])
        self.assertEqual(3*test_val2, ws.dataY(6)[100])
        self.assertEqual(3*test_val3, ws.dataY(50)[200])

        #TODO: Partial sum is not implemented. Should it?
        #propman.sum_runs = 2
        #propman.sample_run = "/home/my_path/MAR11001.raw,c:/somewhere/11001.nxs,MAR11001.raw"
        #self.assertFalse('SR_MAR011001SumOf3' in mtd)
        #self.assertFalse('SR_MAR011001SumOf2' in mtd)
        #ws = PropertyManager.sample_run.get_workspace()
        #self.assertEqual(ws.name(),'SR_MAR011001SumOf2')


        propman.sample_run = 11111
        sum_list,sum_ws,n_sum = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sum_list),4)

        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),4)
        self.assertEqual(runs[3],11111)
        self.assertTrue(propman.sum_runs)
        self.assertTrue('SR_MAR011001SumOf3' in mtd)

        propman.cashe_sum_ws  = True # Not used at this stage but will be used at loading
        PropertyManager.sample_run._run_list.set_cashed_sum_ws(mtd['SR_MAR011001SumOf3'],'SumWS_cashe')
        self.assertTrue('SumWS_cashe' in mtd)
        self.assertFalse('SR_MAR011001SumOf3' in mtd)

        sum_list,sum_ws,n_summed = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sum_list),1)
        self.assertEqual(n_summed,3)
        self.assertEqual(sum_ws.name(),'SumWS_cashe')
        self.assertEqual(sum_list[0],11111)

        # file 1 does not exist, so can not be found. Otherwise it should load it
        self.assertRaises(IOError,PropertyManager.sample_run.get_workspace)

        # Clear all 
        propman.sum_runs=False
        self.assertFalse('SR_MAR011001SumOf3' in mtd)
        # disabling sum_runs clears sum cash
        self.assertFalse('SumWS_cashe' in mtd)
        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),4)
        self.assertEqual(propman.sample_run,11001)

        propman.sample_run = 10111
        runs = PropertyManager.sample_run.get_run_list()
        self.assertEqual(len(runs),1)
        self.assertEqual(runs[0],10111)
        self.assertEqual(propman.sample_run,10111)
        sums,ws,n_sums = PropertyManager.sample_run.get_runs_to_sum()
        self.assertEqual(len(sums),0)
        self.assertTrue(ws is None)
        self.assertEqual(n_sums,0)



if __name__=="__main__":
    unittest.main()
