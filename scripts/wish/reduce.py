# flake8: noqa
import sys
from mantid.simpleapi import *
import numpy as n
import mantid.simpleapi as mantid

sys.path.insert(0, "/opt/Mantid/bin")
sys.path.append("/isis/NDXWISH/user/scripts/autoreduction")


class Wish:
    
    def __init__(self, input_mode, cal_directory, user_directory, output_folder, delete_workspace):
        self.name = input_mode
        self.cal_dir = cal_directory
        self.use_folder = user_directory
        self.out_folder = output_folder
        self.deleteWorkspace = delete_workspace
        self.username = None
        self.wish_datadir = None
        self.wish_userdir = None
        self.wish_datafile = None
        self.userdatadir = None
        self.userdataprocessed = None

    def validate(self, input_file, output_dir):
        """
        Autoreduction validate Function
        -------------------------------

        Function to ensure that the files we want to use in reduction exist.
        Please add any files/directories to the required_files/dirs lists.
        """
        print("Running validation")
        required_files = [input_file]
        required_dirs = [output_dir]
        for file_path in required_files:
            if not os.path.isfile(file_path):
                raise RuntimeError("Unable to find file: {}".format(file_path))
        for folder in required_dirs:
            if not os.path.isdir(folder):
                raise RuntimeError("Unable to find directory: {}".format(folder))
        print("Validation successful")

    # Get the run number from the input data path
    def get_run_number(self, data_path):
        data_path = data_path.split('/')[-1]  # Get the file name
        data_path = data_path.split('.')[0]  # Remove the extension
        data_path = data_path[4:]  # Remove the WISH prefix
        return int(data_path)

    def setuser(self, usern):
        self.username = usern

    # Get the valid wavelength range, i.e. excluding regions where choppers cut
    def getlambdarange(self):
        return 0.7, 10.35

    def setdatadir(self, directory="/archive/ndxwish/Instrument/data/cycle_09_5/"):
        self.wish_datadir = directory

    def setuserdir(self, directory):
        self.wish_userdir = directory

    def setdatafile(self, filename):
        self.wish_datafile = filename

    def startup(self, usern, cycle='14_3'):
        sys.path.append('/home/' + usern + '/Scripts/')
        userdatadir = self.use_folder
        self.setdatadir(userdatadir)
        print "Raw Data in :   ", userdatadir
        userdataprocessed = self.out_folder
        self.setuserdir(directory=userdataprocessed)
        print "Processed Data in :   ", userdataprocessed

    # Returns the calibration filename
    def get_cal(self):
        return self.cal_dir + "WISH_cycle_10_3_noends_10to10.cal"
        # return "/home/mp43/Desktop/Si_Mar15/test_detOffsets_SiMar15_noends.cal"
        # return "/home/ryb18365/Desktop/WISH_cycle_10_3_noends_10to10_dodgytubesremoved.cal"

    # Returns the grouping filename
    def get_group_file(self):
        return self.cal_dir + "WISH_cycle_10_3_noends_10to10.cal"
        # return "/home/mp43/Desktop/Si_Mar15/test_detOffsets_SiMar15_noends.cal"
        # return "/home/ryb18365/Desktop/WISH_cycle_10_3_noends_10to10_dodgytubesremoved.cal"

    def get_vanadium(self, panel, SE="candlestick", cycle="09_4"):
        vanadium_string = {
                "09_2": "vana318-" + str(panel) + "foc-rmbins-smooth50.nx5",
                "09_3": "vana935-" + str(panel) + "foc-SS.nx5",
                "09_4": "vana3123-" + str(panel) + "foc-SS.nx5",
                "09_5": "vana3123-" + str(panel) + "foc-SS.nx5",
                "11_1": "vana17718-" + str(panel) + "foc-SS.nxs",
                "11_2": "vana16812-" + str(panel) + "foc-SS.nx5",
                "11_3": "vana18590-" + str(panel) + "foc-SS-new.nxs",
                "11_4": "vana38428-" + str(panel) + "foc-SF-SS.nxs",
                "18_2": "WISHvana41865-" + str(panel) + "foc.nxs"
        }
        return self.cal_dir + vanadium_string.get(cycle)

    def split_string(self, t):
        indxp = 0
        for i in range(0, len(t)):
            if t[i] == "+":
                indxp = i
        if indxp != 0:
            return int(t[0:indxp]), int(t[indxp + 1:len(t)])

    def get_empty(self, panel, SE="WISHcryo", cycle="09_4"):
        if SE == "WISHcryo":
            empty_string = {
                "09_2": "emptycryo322-" + str(panel) + "-smooth50.nx5",
                "09_3": "emptycryo1725-" + str(panel) + "foc.nx5",
                "09_4": "emptycryo3307-" + str(panel) + "foc.nx5",
                "09_5": "emptycryo16759-" + str(panel) + "foc.nx5",
                "11_1": "emptycryo17712-" + str(panel) + "foc-SS.nxs",
                "11_2": "emptycryo16759-" + str(panel) + "foc-SS.nx5",
                "11_3": "emptycryo17712-" + str(panel) + "foc-SS-new.nxs",
                "11_4": "empty_mag20620-" + str(panel) + "foc-HR-SF.nxs"

            }
            return self.cal_dir + empty_string.get(cycle)

        if SE == "candlestick":
            empty_string = {
                "09_3": "emptyinst1726-" + str(panel) + "foc-monitor.nxs",
                "09_4": "emptyinst3120-" + str(panel) + "foc.nxs",
                "11_4": "emptyinst19618-" + str(panel) + "foc-SF-S.nxs",
                "17_1": "emptyinst38581-" + str(panel) + "foc.nxs"
            }
            return self.cal_dir + empty_string.get(cycle)

    def get_file_name(self, run_number, ext):
        if ext[0] != 's':
            data_dir = self.wish_datadir
        else:
            # data_dir="/datad/ndxwish/"
            data_dir = self.wish_datadir
        digit = len(str(run_number))
        filename = os.path.join(data_dir, "WISH")
        for i in range(0, 8 - digit):
            filename = filename + "0"
        filename += str(run_number) + "." + ext
        return filename

    def return_panel(self, panel):
        return_panel={
            1: (6, 19461),
            2: (19462, 38917),
            3: (38918, 58373),
            4: (58374, 77829),
            5: (77830, 97285),
            6: (97286, 116741),
            7: (116742, 136197),
            8: (136198, 155653),
            9: (155654, 175109),
            10: (175110, 194565),
            0: (6, 194565)
        }
        print panel
        return return_panel.get(panel)

    # Reads a wish data file return a workspace with a short name
    def read_wish_file(self, number, panel, ext):
        if type(number) is int:
            filename = self.wish_datafile  # Changed as full path is set in main now
            ext = filename.split('.')[-1]  # Get the extension from the inputted filename
            print "Extension is: " + ext
            # if (ext[0:10]=="nxs_event"):
            #    filename=WISH_getfilename(number,"nxs")
            print "will be reading filename..." + filename
            panel_min, panel_max = self.return_panel(panel)
            if panel != 0:
                output = "w" + str(number) + "-" + str(panel)
            else:
                output = "w" + str(number)
            if ext == "raw":
                mantid.LoadRaw(Filename=filename, OutputWorkspace=output, SpectrumMin=str(panel_min),
                               SpectrumMax=str(panel_max), LoadLogFiles="0")
                mantid.MaskBins(InputWorkspace=output, OutputWorkspace=output, XMin=99900, XMax=106000)
                print "standard raw file loaded"
            if ext[0] == "s":
                mantid.LoadRaw(Filename=filename, OutputWorkspace=output, SpectrumMin=str(panel_min),
                               SpectrumMax=str(panel_max), LoadLogFiles="0")
                mantid.MaskBins(InputWorkspace=output, OutputWorkspace=output, XMin=99900, XMax=106000)
                print "sav file loaded"
            if ext == "nxs_event":
                mantid.LoadEventNexus(Filename=filename, OutputWorkspace=output, LoadMonitors='1')
                mantid.RenameWorkspace(output + "_monitors", "w" + str(number) + "_monitors")
                mantid.Rebin(InputWorkspace=output, OutputWorkspace=output, Params='6000,-0.00063,110000')
                mantid.ConvertToMatrixWorkspace(output, output)
                panel_min, panel_max = self.return_panel(panel)
                mantid.CropWorkspace(InputWorkspace=output, OutputWorkspace=output, StartWorkspaceIndex=panel_min - 6,
                                     EndWorkspaceIndex=panel_max - 6)
                mantid.MaskBins(InputWorkspace=output, OutputWorkspace=output, XMin=99900, XMax=106000)
                print "full nexus event file loaded"
            if ext[0:10] == "nxs_event_":
                label, tmin, tmax = self.split_string_event(ext)
                output = output + "_" + label
                if tmax == "end":
                    mantid.LoadEventNexus(Filename=filename, OutputWorkspace=output, FilterByTimeStart=tmin,
                                          LoadMonitors='1',
                                          MonitorsAsEvents='1', FilterMonByTimeStart=tmin)

                else:
                    mantid.LoadEventNexus(Filename=filename, OutputWorkspace=output, FilterByTimeStart=tmin,
                                          FilterByTimeStop=tmax,
                                          LoadMonitors='1', MonitorsAsEvents='1', FilterMonByTimeStart=tmin,
                                          FilterMonByTimeStop=tmax)
                    mantid.RenameWorkspace(output + "_monitors", "w" + str(number) + "_monitors")

                print "renaming monitors done!"
                mantid.Rebin(InputWorkspace=output, OutputWorkspace=output, Params='6000,-0.00063,110000')
                mantid.ConvertToMatrixWorkspace(output, output)
                panel_min, panel_max = self.return_panel(panel)
                mantid.CropWorkspace(InputWorkspace=output, OutputWorkspace=output, StartWorkspaceIndex=panel_min - 6,
                                     EndWorkspaceIndex=panel_max - 6)
                mantid.MaskBins(output, output, XMin=99900, XMax=106000)
                print "nexus event file chopped"
            if ext == "nxs":
                mantid.LoadNexus(Filename=filename, OutputWorkspace=output, SpectrumMin=str(panel_min),
                                 SpectrumMax=str(panel_max))
                mantid.MaskBins(InputWorkspace=output, OutputWorkspace=output, XMin=99900, XMax=106000)
                mantid.ConvertUnits(InputWorkspace=output, OutputWorkspace=output, Target="Wavelength", Emode="Elastic")
                print "standard histo nxs file loaded"

        else:
            n1, n2 = self.split_string(number)
            output = "w" + str(n1) + "_" + str(n2) + "-" + str(panel)
            filename = self.get_file_name(n1, ext)
            print "reading filename..." + filename
            panel_min, panel_max = self.return_panel(panel)
            output1 = "w" + str(n1) + "-" + str(panel)
            mantid.LoadRaw(Filename=filename, OutputWorkspace=output1, SpectrumMin=str(panel_min),
                           SpectrumMax=str(panel_max), LoadLogFiles="0")
            filename = self.get_file_name(n2, ext)
            print "reading filename..." + filename
            panel_min, panel_max = self.return_panel(panel)
            output2 = "w" + str(n2) + "-" + str(panel)
            mantid.LoadRaw(Filename=filename, OutputWorkspace=output2, SpectrumMin=str(panel_min),
                           SpectrumMax=str(panel_max), LoadLogFiles="0")
            mantid.MergeRuns(output1 + "," + output2, output)
            mantid.DeleteWorkspace(output1)
            mantid.DeleteWorkspace(output2)
            mantid.ConvertUnits(InputWorkspace=output, OutputWorkspace=output, Target="Wavelength", Emode="Elastic")

        lmin, lmax = self.getlambdarange()
        mantid.CropWorkspace(InputWorkspace=output, OutputWorkspace=output, XMin=lmin, XMax=lmax)
        monitor = self.process_incident_monitor(number, ext, spline_terms=70, debug=False)
        print "first norm to be done"
        mantid.NormaliseToMonitor(InputWorkspace=output, OutputWorkspace=output + "norm1", MonitorWorkspace=monitor)
        print "second norm to be done"
        mantid.NormaliseToMonitor(InputWorkspace=output + "norm1", OutputWorkspace=output + "norm2",
                                  MonitorWorkspace=monitor, IntegrationRangeMin=0.7, IntegrationRangeMax=10.35)
        mantid.DeleteWorkspace(output)
        mantid.DeleteWorkspace(output + "norm1")
        mantid.RenameWorkspace(InputWorkspace=output + "norm2", OutputWorkspace=output)
        mantid.ConvertUnits(InputWorkspace=output, OutputWorkspace=output, Target="TOF", EMode="Elastic")
        mantid.ReplaceSpecialValues(InputWorkspace=output, OutputWorkspace=output, NaNValue=0.0, NaNError=0.0,
                                    InfinityValue=0.0, InfinityError=0.0)
        return output

    # Focus dataset for a given panel and return the workspace
    def focus_one_panel(self, work, focus, panel):
        mantid.AlignDetectors(InputWorkspace=work, OutputWorkspace=work, CalibrationFile=self.get_cal())
        mantid.DiffractionFocussing(InputWorkspace=work, OutputWorkspace=focus, GroupingFileName=self.get_group_file())
        if panel == 5 or panel == 6:
            mantid.CropWorkspace(InputWorkspace=focus, OutputWorkspace=focus, XMin=0.3)
        return focus

    def split_workspace(self, focus):
        for i in range(0, 11):
            out = focus[0:len(focus) - 3] + "-" + str(i + 1) + "foc"
            mantid.ExtractSingleSpectrum(InputWorkspace=focus, OutputWorkspace=out, WorkspaceIndex=i)
            mantid.DeleteWorkspace(focus)
        return

    def focus(self, work, panel):
        focus = work + "foc"
        if panel != 0:
            self.focus_one_panel(work, focus, panel)
        else:
            self.focus_one_panel(work, focus, panel)
            self.split_workspace(focus)

    def process(self, number, panel, ext, se_sample="WISHcryo", empty_se_cycle="09_4", se_vana="candlestick",
                cyclevana="09_4", absorb=False, nd=0.0, xs=0.0, xa=0.0, h=0.0, r=0.0):
        w = self.read_wish_file(number, panel, ext)
        print "file read and normalized"
        if absorb:
            mantid.ConvertUnits(InputWorkspace=w, OutputWorkspace=w, Target="Wavelength", EMode="Elastic")
            mantid.CylinderAbsorption(InputWorkspace=w, OutputWorkspace="T",
                                      CylinderSampleHeight=h, CylinderSampleRadius=r, AttenuationXSection=xa,
                                      ScatteringXSection=xs, SampleNumberDensity=nd,
                                      NumberOfSlices="10", NumberOfAnnuli="10", NumberOfWavelengthPoints="25",
                                      ExpMethod="Normal")
            mantid.Divide(LHSWorkspace=w, RHSWorkspace="T", OutputWorkspace=w)
            mantid.DeleteWorkspace("T")
            mantid.ConvertUnits(InputWorkspace=w, OutputWorkspace=w, Target="TOF", EMode="Elastic")
        wfoc = self.focus(w, panel)
        print "focussing done!"
        if type(number) is int:
            wfocname = "w" + str(number) + "-" + str(panel) + "foc"
            if (len(ext) > 9):
                label, tmin, tmax = self.split_string_event(ext)
                wfocname = "w" + str(number) + "-" + str(panel) + "_" + label + "foc"
        else:
            n1, n2 = self.split_string(number)
            wfocname = "w" + str(n1) + "_" + str(n2) + "-" + str(panel) + "foc"
        if self.deleteWorkspace:
            mantid.DeleteWorkspace(w)
        panel_crop = {
            1: (0.8, 53.3),
            2: (0.5, 13.1),
            3: (0.5, 7.77),
            4: (0.4, 5.86),
            5: (0.35, 4.99),
            6: (0.35, 4.99),
            7: (0.4, 5.86),
            8: (0.5, 7.77),
            9: (0.5, 13.1),
            10: (0.8, 53.3)
        }
        x_min, x_max = panel_crop.get(panel)
        mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=x_min, XMax=x_max)
        if panel == 0:
            for i in range(1, 11):
                wfocname = "w" + str(number) + "-" + str(i) + "foc"
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.80, XMax=53.3)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.50, XMax=13.1)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.50, XMax=7.77)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.40, XMax=5.86)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.35, XMax=4.99)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.35, XMax=4.99)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.40, XMax=5.86)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.50, XMax=7.77)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.50, XMax=13.1)
                mantid.CropWorkspace(InputWorkspace=wfocname, OutputWorkspace=wfocname, XMin=0.80, XMax=53.3)
        # print "will try to load an empty with the name:"
        print panel
        print se_sample
        print empty_se_cycle
        print self.get_empty(panel, se_sample, empty_se_cycle)
        if panel == 0:
            for i in range(1, 11):
                wfocname = "w" + str(number) + "-" + str(i) + "foc"
                mantid.LoadNexusProcessed(Filename=self.get_empty(i, se_sample, empty_se_cycle), OutputWorkspace="empty")
                mantid.RebinToWorkspace(WorkspaceToRebin="empty", WorkspaceToMatch=wfocname, OutputWorkspace="empty")
                mantid.Minus(LHSWorkspace=wfocname, RHSWorkspace="empty", OutputWorkspace=wfocname)
                mantid.DeleteWorkspace("empty")
                print "will try to load a vanadium with the name:" + self.get_vanadium(i, se_vana, cyclevana)
                mantid.LoadNexusProcessed(Filename=self.get_vanadium(i, se_vana, cyclevana), OutputWorkspace="vana")
                mantid.RebinToWorkspace(WorkspaceToRebin="vana", WorkspaceToMatch=wfocname, OutputWorkspace="vana")
                mantid.Divide(LHSWorkspace=wfocname, RHSWorkspace="vana", OutputWorkspace=wfocname)
                mantid.DeleteWorkspace("vana")
                mantid.ConvertUnits(InputWorkspace=wfocname, OutputWorkspace=wfocname, Target="TOF", EMode="Elastic")
                mantid.ReplaceSpecialValues(InputWorkspace=wfocname, OutputWorkspace=wfocname, NaNValue=0.0,
                                            NaNError=0.0,
                                            InfinityValue=0.0, InfinityError=0.0)
                mantid.SaveGSS(InputWorkspace=wfocname,
                               Filename=os.path.join(self.wish_userdir, (str(number) + "-" + str(i) + ext + ".gss")),
                               Append=False, Bank=1)
                mantid.SaveFocusedXYE(wfocname,
                                      os.path.join(self.wish_userdir, (str(number) + "-" + str(i) + ext + ".dat")))
                mantid.SaveNexusProcessed(wfocname,
                                          os.path.join(self.wish_userdir, (str(number) + "-" + str(i) + ext + ".nxs")))
        else:
            mantid.LoadNexusProcessed(Filename=self.get_empty(panel, se_sample, empty_se_cycle), OutputWorkspace="empty")
            mantid.RebinToWorkspace(WorkspaceToRebin="empty", WorkspaceToMatch=wfocname, OutputWorkspace="empty")
            mantid.Minus(LHSWorkspace=wfocname, RHSWorkspace="empty", OutputWorkspace=wfocname)
            mantid.DeleteWorkspace("empty")
            print "will try to load a vanadium with the name:" + self.get_vanadium(panel, se_vana, cyclevana)
            mantid.LoadNexusProcessed(Filename=self.get_vanadium(panel, se_vana, cyclevana), OutputWorkspace="vana")
            mantid.RebinToWorkspace(WorkspaceToRebin="vana", WorkspaceToMatch=wfocname, OutputWorkspace="vana")
            mantid.Divide(LHSWorkspace=wfocname, RHSWorkspace="vana", OutputWorkspace=wfocname)
            mantid.DeleteWorkspace("vana")
            mantid.ConvertUnits(InputWorkspace=wfocname, OutputWorkspace=wfocname, Target="TOF", EMode="Elastic")
            mantid.ReplaceSpecialValues(InputWorkspace=wfocname, OutputWorkspace=wfocname, NaNValue=0.0, NaNError=0.0,
                                        InfinityValue=0.0, InfinityError=0.0)
            mantid.SaveGSS(InputWorkspace=wfocname,
                           Filename=os.path.join(self.wish_userdir, (str(number) + "-" + str(panel) + ext + ".gss")),
                           Append=False, Bank=1)
            mantid.SaveFocusedXYE(wfocname,
                                  os.path.join(self.wish_userdir, (str(number) + "-" + str(panel) + ext + ".dat")))
            mantid.SaveNexusProcessed(wfocname,
                                      os.path.join(self.wish_userdir, (str(number) + "-" + str(panel) + ext + ".nxs")))
        return wfocname

    # Create a corrected vanadium (normalise,corrected for attenuation and empty, strip peaks) and
    # save a a nexus processed file.
    # It looks like smoothing of 100 works quite well
    def create_normalised_vanadium(self, van, empty, panel, smoothing, vh, vr, cycle_van="18_2", cycle_empty="17_1"):
        self.startup("ffv81422", cycle_van)
        self.setdatafile(self.get_file_name(41870, "nxs"))
        self.setdatadir("/archive/ndxwish/Instrument/data/cycle_" + cycle_van + "/")
        wish_van = self.read_wish_file(van, panel, "nxs_event")
        self.startup("ffv81422", cycle_empty)
        self.setdatafile(self.get_file_name(38581, "nxs"))
        self.setdatadir("/archive/ndxwish/Instrument/data/cycle_" + cycle_empty + "/")
        wish_empty = self.read_wish_file(empty, panel, "nxs_event")
        mantid.Minus(LHSWorkspace=wish_van, RHSWorkspace=wish_empty, OutputWorkspace=wish_van)
        print "read van and empty"
        mantid.DeleteWorkspace(wish_empty)
        mantid.ConvertUnits(InputWorkspace=wish_van, OutputWorkspace=wish_van, Target="Wavelength", EMode="Elastic")
        mantid.CylinderAbsorption(InputWorkspace=wish_van, OutputWorkspace="T",
                                  CylinderSampleHeight=str(vh), CylinderSampleRadius=str(vr),
                                  AttenuationXSection="4.8756",
                                  ScatteringXSection="5.16", SampleNumberDensity="0.07118",
                                  NumberOfSlices="10", NumberOfAnnuli="10", NumberOfWavelengthPoints="25",
                                  ExpMethod="Normal")
        mantid.Divide(LHSWorkspace=wish_van, RHSWorkspace="T", OutputWorkspace=wish_van)
        mantid.DeleteWorkspace("T")
        mantid.ConvertUnits(InputWorkspace=wish_van, OutputWorkspace=wish_van, Target="TOF", EMode="Elastic")
        # vanfoc = WISH_focus(wish_van, panel)
        mantid.DeleteWorkspace(wish_van)
        # StripPeaks(vanfoc,vanfoc)
        # SmoothData(vanfoc,vanfoc,str(smoothing))
        return

    def create_empty(self, empty, panel):
        wish_empty = self.read_wish_file(empty, panel, "raw")
        empty_focus = self.focus(wish_empty, panel)
        return empty_focus

    # Have made no changes here as not called (may not work in future though)
    def monitors(self, rb, ext):
        # data_dir = WISH_dir()
        filename = self.get_file_name(rb, ext)
        workspace_out = "w" + str(rb)
        print "reading File..." + filename
        mantid.LoadRaw(Filename=filename, OutputWorkspace=workspace_out, SpectrumMin=str(1), SpectrumMax=str(5),
                       LoadLogFiles="0")
        mantid.NormaliseByCurrent(InputWorkspace=workspace_out, OutputWorkspace=workspace_out)
        mantid.ConvertToDistribution(workspace_out)
        return workspace_out

    def process_incident_monitor(self, number, ext, spline_terms=20, debug=False):
        if type(number) is int:
            filename = self.wish_datafile
            works = "monitor" + str(number)
            if ext == "raw":
                works = "monitor" + str(number)
                mantid.LoadRaw(Filename=filename, OutputWorkspace=works, SpectrumMin=4, SpectrumMax=4, LoadLogFiles="0")
            if ext[0] == "s":
                works = "monitor" + str(number)
                mantid.LoadRaw(Filename=filename, OutputWorkspace=works, SpectrumMin=4, SpectrumMax=4, LoadLogFiles="0")
            if ext == "nxs":
                works = "monitor" + str(number)
                mantid.LoadNexus(Filename=filename, OutputWorkspace=works, SpectrumMin=4, SpectrumMax=4)
                mantid.ConvertUnits(InputWorkspace=works, OutputWorkspace=works, Target="Wavelength", Emode="Elastic")
            if ext[0:9] == "nxs_event":
                temp = "w" + str(number) + "_monitors"
                works = "w" + str(number) + "_monitor4"
                mantid.Rebin(InputWorkspace=temp, OutputWorkspace=temp, Params='6000,-0.00063,110000',
                             PreserveEvents=False)
                mantid.ExtractSingleSpectrum(InputWorkspace=temp, OutputWorkspace=works, WorkspaceIndex=3)
        else:
            n1, n2 = self.split_string(number)
            works = "monitor" + str(n1) + "_" + str(n2)
            filename = self.get_file_name(n1, ext)
            works1 = "monitor" + str(n1)
            mantid.LoadRaw(Filename=filename, OutputWorkspace=works1, SpectrumMin=4, SpectrumMax=4, LoadLogFiles="0")
            filename = self.get_file_name(n2, ext)
            works2 = "monitor" + str(n2)
            mantid.LoadRaw(Filename=filename, OutputWorkspace=works2, SpectrumMin=4, SpectrumMax=4, LoadLogFiles="0")
            mantid.MergeRuns(InputWorkspaces=works1 + "," + works2, OutputWorkspace=works)
            mantid.DeleteWorkspace(works1)
            mantid.DeleteWorkspace(works2)
            mantid.ConvertUnits(InputWorkspace=works, OutputWorkspace=works, Target="Wavelength", Emode="Elastic")
        l_min, l_max = self.getlambdarange()
        mantid.CropWorkspace(InputWorkspace=works, OutputWorkspace=works, XMin=l_min, XMax=l_max)
        ex_regions = n.zeros((2, 4))
        ex_regions[:, 0] = [4.57, 4.76]
        ex_regions[:, 1] = [3.87, 4.12]
        ex_regions[:, 2] = [2.75, 2.91]
        ex_regions[:, 3] = [2.24, 2.50]
        mantid.ConvertToDistribution(works)
        for reg in range(0, 4):
            mantid.MaskBins(InputWorkspace=works, OutputWorkspace=works, XMin=ex_regions[0, reg],
                            XMax=ex_regions[1, reg])
        mantid.ConvertFromDistribution(works)
        return works

    def split_string_event(self, t):
        # this assumes the form nxs_event_label_tmin_tmax
        indx_ = []
        for i in range(10, len(t)):
            if (t[i] == "_"):
                indx_.append(i)
        label = t[10:indx_[0]]
        tmin = t[indx_[0] + 1:indx_[1]]
        tmax = t[indx_[1] + 1:len(t)]
        return label, tmin, tmax

    def minus_empty_cans(self, runno, empty):
        panel_list = ['-1foc', '-2foc', '-3foc', '-4foc', '-5foc', '-6foc', '-7foc', '-8foc', '-9foc', '-10foc',
                      '-1_10foc',
                      '-2_9foc', '-3_8foc', '-4_7foc', '-5_6foc']
        for p in panel_list:
            mantid.Minus(LHSWorkspace='w' + str(runno) + p, RHSWorkspace='w' + str(empty) + p,
                         OutputWorkspace='w' + str(runno) + 'minus' + str(empty) + p)
            mantid.ConvertUnits(InputWorkspace='w' + str(runno) + 'minus' + str(empty) + p,
                                OutputWorkspace='w' + str(runno) + 'minus' + str(empty) + p + '-d', Target='dSpacing')
            mantid.SaveGSS("w" + str(runno) + 'minus' + str(empty) + p,
                           os.path.join(self.wish_userdir, (str(runno) + p + ".gss")), Append=False, Bank=1)

    def main(self, input_file, output_dir):
        # Check files can be found
        self.validate(input_file, output_dir)
        # #####################################################################
        # #####     			USER SPECIFIC PART STARTS BELOW 									   ##
        # #####     			IN CASE LINES ABOVE HAVE BEEN EDITED AND SCRIPTS NO LONGER WORK   ##
        # #####     			LOG OUT AND BACK IN TO THE MACHINE								   ##
        # #####################################################################
        # ########### SETTING the paths automatically : WISH_startup(username,cycle_name) ##############
        # WISH_startup("ryb18365","15_1")

        self.setuserdir(output_dir)
        self.setdatafile(input_file)
        print(output_dir)
        print(input_file)

        i = self.get_run_number(input_file)
        for j in range(1, 11):
            self.process(i, j, "raw", "candlestick", "17_1", "candlestick", "18_2", absorb=False, nd=0.0, xs=0.0,
                         xa=0.0, h=4.0, r=0.4)
        for j in range(1, 11):
            wout = self.process(i, j, "raw", "candlestick", "17_1", "candlestick", "18_2", absorb=False, nd=0.0,
                                xs=0.0, xa=0.0, h=4.0, r=0.4)
            mantid.ConvertUnits(InputWorkspace=wout, OutputWorkspace=wout + "-d", Target="dSpacing", EMode="Elastic")

            mantid.RebinToWorkspace(WorkspaceToRebin="w" + str(i) + "-6foc", WorkspaceToMatch="w" + str(i) + "-5foc",
                                    OutputWorkspace="w" + str(i) + "-6foc", PreserveEvents='0')
            mantid.Plus(LHSWorkspace="w" + str(i) + "-5foc", RHSWorkspace="w" + str(i) + "-6foc",
                        OutputWorkspace="w" + str(i) + "-5_6foc")
            mantid.ConvertUnits(InputWorkspace="w" + str(i) + "-5_6foc",
                                OutputWorkspace="w" + str(i) + "-5_6foc" + "-d",
                                Target="dSpacing", EMode="Elastic")
            mantid.SaveGSS("w" + str(i) + "-5_6foc", os.path.join(self.wish_userdir, (str(i) + "-5_6raw" + ".gss")),
                           Append=False, Bank=1)
            mantid.SaveFocusedXYE("w" + str(i) + "-5_6foc", os.path.join(self.wish_userdir, (str(i) + "-5_6raw" + ".dat")))
            mantid.SaveNexusProcessed("w" + str(i) + "-5_6foc",
                                      os.path.join(self.wish_userdir, (str(i) + "-5_6raw" + ".nxs")))
            mantid.RebinToWorkspace(WorkspaceToRebin="w" + str(i) + "-7foc", WorkspaceToMatch="w" + str(i) + "-4foc",
                                    OutputWorkspace="w" + str(i) + "-7foc", PreserveEvents='0')
            mantid.Plus(LHSWorkspace="w" + str(i) + "-4foc", RHSWorkspace="w" + str(i) + "-7foc",
                        OutputWorkspace="w" + str(i) + "-4_7foc")
            mantid.ConvertUnits(InputWorkspace="w" + str(i) + "-4_7foc",
                                OutputWorkspace="w" + str(i) + "-4_7foc" + "-d",
                                Target="dSpacing", EMode="Elastic")
            mantid.SaveGSS("w" + str(i) + "-4_7foc", os.path.join(self.wish_userdir, (str(i) + "-4_7raw" + ".gss")),
                           Append=False, Bank=1)
            mantid.SaveFocusedXYE("w" + str(i) + "-4_7foc", os.path.join(self.wish_userdir, (str(i) + "-4_7raw" + ".dat")))
            mantid.SaveNexusProcessed("w" + str(i) + "-4_7foc",
                                      os.path.join(self.wish_userdir, (str(i) + "-4_7raw" + ".nxs")))
        mantid.RebinToWorkspace(WorkspaceToRebin="w" + str(i) + "-8foc", WorkspaceToMatch="w" + str(i) + "-3foc",
                                OutputWorkspace="w" + str(i) + "-8foc", PreserveEvents='0')
        mantid.Plus(LHSWorkspace="w" + str(i) + "-3foc", RHSWorkspace="w" + str(i) + "-8foc",
                    OutputWorkspace="w" + str(i) + "-3_8foc")
        mantid.ConvertUnits(InputWorkspace="w" + str(i) + "-3_8foc", OutputWorkspace="w" + str(i) + "-3_8foc" + "-d",
                            Target="dSpacing", EMode="Elastic")
        mantid.SaveGSS("w" + str(i) + "-3_8foc", os.path.join(self.wish_userdir, (str(i) + "-3_8raw" + ".gss")),
                       Append=False, Bank=1)
        mantid.SaveFocusedXYE("w" + str(i) + "-3_8foc", os.path.join(self.wish_userdir, (str(i) + "-3_8raw" + ".dat")))
        mantid.SaveNexusProcessed("w" + str(i) + "-3_8foc", os.path.join(self.wish_userdir, (str(i) + "-3_8raw" + ".nxs")))
        mantid.RebinToWorkspace(WorkspaceToRebin="w" + str(i) + "-9foc", WorkspaceToMatch="w" + str(i) + "-2foc",
                                OutputWorkspace="w" + str(i) + "-9foc", PreserveEvents='0')
        mantid.Plus(LHSWorkspace="w" + str(i) + "-2foc", RHSWorkspace="w" + str(i) + "-9foc",
                    OutputWorkspace="w" + str(i) + "-2_9foc")
        mantid.ConvertUnits(InputWorkspace="w" + str(i) + "-2_9foc", OutputWorkspace="w" + str(i) + "-2_9foc" + "-d",
                            Target="dSpacing", EMode="Elastic")
        mantid.SaveGSS("w" + str(i) + "-2_9foc", os.path.join(self.wish_userdir, (str(i) + "-2_9raw" + ".gss")),
                       Append=False, Bank=1)
        mantid.SaveFocusedXYE("w" + str(i) + "-2_9foc", os.path.join(self.wish_userdir, (str(i) + "-2_9raw" + ".dat")))
        mantid.SaveNexusProcessed("w" + str(i) + "-2_9foc", os.path.join(self.wish_userdir, (str(i) + "-2_9raw" + ".nxs")))
        mantid.RebinToWorkspace(WorkspaceToRebin="w" + str(i) + "-10foc", WorkspaceToMatch="w" + str(i) + "-1foc",
                                OutputWorkspace="w" + str(i) + "-10foc", PreserveEvents='0')
        mantid.Plus(LHSWorkspace="w" + str(i) + "-1foc", RHSWorkspace="w" + str(i) + "-10foc",
                    OutputWorkspace="w" + str(i) + "-1_10foc")
        mantid.ConvertUnits(InputWorkspace="w" + str(i) + "-1_10foc", OutputWorkspace="w" + str(i) + "-1_10foc" + "-d",
                            Target="dSpacing", EMode="Elastic")
        mantid.SaveGSS("w" + str(i) + "-1_10foc", os.path.join(self.wish_userdir, (str(i) + "-1_10raw" + ".gss")),
                       Append=False, Bank=1)
        mantid.SaveFocusedXYE("w" + str(i) + "-1_10foc", os.path.join(self.wish_userdir, (str(i) + "-1_10raw" + ".dat")))
        mantid.SaveNexusProcessed("w" + str(i) + "-1_10foc",
                                  os.path.join(self.wish_userdir, (str(i) + "-1_10raw" + ".nxs")))

    def create_vanadium(self):
        # ######### use the lines below to process a LoadRawvanadium run                               #################
        for j in range(1, 11):
            self.create_normalised_vanadium(41865, 38581, j, 100, 4.0, 0.15, cycle_van="18_2", cycle_empty="17_1")
            mantid.CropWorkspace(InputWorkspace="w41865-" + str(j) + "foc", OutputWorkspace="w41865-" + str(j) + "foc",
                                 XMin='0.35',
                                 XMax='5.0')
            self.Removepeaks_spline_smooth_vana("w41865-" + str(j) + "foc", j, debug=False)
            mantid.SaveNexusProcessed("w41865-" + str(j) + "foc",
                                      os.path.join(self.wish_userdir, ("vana41865-" + str(j) + "foc.nxs")))

    def run_script(self):
        if self.name == "__main__":
            self.startup("ffv81422", "18_2")
            self.setdatafile(self.get_file_name(41870, "nxs"))

            self.main(self.wish_datafile, self.wish_userdir)
