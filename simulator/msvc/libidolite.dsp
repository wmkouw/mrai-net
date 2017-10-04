# Microsoft Developer Studio Project File - Name="libidolite" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libidolite - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libidolite.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libidolite.mak" CFG="libidolite - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libidolite - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libidolite - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libidolite - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libidolite - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "VERBOSE" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\bin\libidolite.lib"

!ENDIF 

# Begin Target

# Name "libidolite - Win32 Release"
# Name "libidolite - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\src\libido-lite\acrread.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\acrutil.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\acrwrite.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\arg.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\argexit.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\cntallo.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\cntcdg.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\cntfill.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\cntutil.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\dcmjpeg.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\dcmread.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\dcmutil.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\dicom.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\dyima.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\dyimawin32.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\dyimaxold.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\gdcmJpeg.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imaallo.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imabasic.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imachekal.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imaclear.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imadrawline.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imafill.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imagette.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imalabel.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imamax.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imapart.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imawrf.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\imazoom.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\kererr.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\kerextern.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\kernomfic.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\kerprint.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\lstallo.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\lstbasic.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\seqallo.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\sigallo.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\sigfft.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\sigmm.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\sigpalet.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\sigtoima.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\sigwrf.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\str.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\strcomp.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\utilcalc.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\volallo.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\volfft.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\volmax.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\volmin.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\volrdf.c"
# End Source File
# Begin Source File

SOURCE="..\src\libido-lite\volwrf.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
