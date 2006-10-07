all: findpk.exe

findpk.exe: findpk.obj ..\..\lib\app.lib ..\..\lib\core.lib

.cpp.obj:
	cl /c /W3 /Wp64 /nologo /GF /Zi /O2 /GL -DWIN32 -DWINVER=0x0400 -D_WIN32_WINDOWS=0x0400 -D_WIN32_WINNT=0x0400 -D_WIN32_IE=0x0500 /MD -DMIKTEX -DMIKTEX_WINDOWS -DMIKTEX_DLL -DAPP_DLL -DTEXMF_DLL -DGNU_DLL -DREGEX_DLL -DGSOAP_DLL -DMPM_DLL -DEXPAT_DLL -DPOPT_DLL -DOFFICIAL -DFINAL -DNDEBUG  -I. -I../../include /TP  /EHsc -D_SECURE_SCL_THROWS=1 $<

#uncomment the following line if you want delayed loading
#delayload = /delayload:miktex-core-3.dll /delayload:miktex-app-1.dll delayimp.lib

.obj.exe:
	link -INCREMENTAL:NO /NOLOGO /LTCG /RELEASE /SUBSYSTEM:CONSOLE /DEBUG -out:$@ ..\..\lib\app.lib ..\..\lib\core.lib $(delayload) $< 
	mt -nologo -manifest $@.manifest -outputresource:$@;1
