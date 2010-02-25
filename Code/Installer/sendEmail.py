import re
import smtplib
import os
import platform
from shutil import move
from time import strftime
import sys
sys.path.append('../Mantid/Build')
import buildNotification as notifier

#Email settings
smtpserver = 'outbox.rl.ac.uk'
RECIPIENTS = ['mantid-buildserver@mantidproject.org']
SENDER = 'Win' + platform.architecture()[0][:2]  + 'Installer@mantidproject.org'
#Create Subject
subject = 'Subject: Windows' + platform.architecture()[0][:2]  + ' Build Report: '

#Set up email content 
buildSuccess = True

mssgBuild = ''
mssgBuildErr = ''

localLogDir = '../../../../logs/Installer/'

#create archive directory
remoteArchiveDir,relativeLogDir = notifier.getArchiveDir('Installer')

#Get build result and errors
fileBuild = localLogDir+'build.log'
f = open(fileBuild,'r')

for line in f.readlines():
     buildResult = line
     mssgBuild = mssgBuild + line
     
f.close()
notifier.moveToArchive(fileBuild,remoteArchiveDir)

if buildResult.startswith('failed'):
	buildSuccess = False	
	
fileBuildErr = localLogDir+'error.log'
mssgBuildErr = open(fileBuildErr,'r').read()
notifier.moveToArchive(fileBuildErr,remoteArchiveDir)

last = localLogDir + 'lastBuild.txt'
try:
     lastBuild = open(last,'r').read()
except IOError:
     lastBuild = 'False'
open(last,'w').write(str(buildSuccess))

# We want to skip sending email if this AND previous build succeeded
if buildSuccess and lastBuild =='True':
     sys.exit(0)

#Construct Message
httpLinkToArchive = 'http://download.mantidproject.org/' + relativeLogDir.replace("\\","/")

message = 'Build Completed at: ' + strftime("%H:%M:%S %d-%m-%Y") + "\n"
message += 'Build Passed: ' + str(buildSuccess) + "\n"
message += 'BUILD LOG\n\n'
message += 'Build stdout <' + httpLinkToArchive + 'build.log>\n'
message += 'Build stderr <' + httpLinkToArchive + 'error.log>\n'


if buildSuccess:
	subject += '[Build Successful]\n\n\n'
else:
	subject += '[Build Failed]\n\n\n'
	
#Send Email
session = smtplib.SMTP(smtpserver)

smtpresult  = session.sendmail(SENDER, RECIPIENTS, subject  + message)

if smtpresult:
    errstr = ""
    for recip in smtpresult.keys():
        errstr = """Could not delivery mail to: %s

Server said: %s
%s

%s""" % (recip, smtpresult[recip][0], smtpresult[recip][1], errstr)
    raise smtplib.SMTPException, errstr

if buildSuccess:
    sys.exit(0)
else:
    sys.exit(1)
     
