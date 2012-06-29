#define MSG_ADD_DATAREF 0x01000000
#define XPLM200 1

//xplm
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>
#include <XPLMPlanes.h>
//ppl
#include <settings.h>
#include <pluginpath.h>
//us
#include <string>
#include <sstream>

using namespace PPL;
using namespace std;

PluginPath thisPluginPath;
Settings switchIni(thisPluginPath.prependPluginPath("/AircraftSwitcher.ini"), 1, 0);
//prependPluginPath("/AircraftSwitcher puts the .ini one level up from the *.xpl file.

XPLMCommandRef loadAcf[9] = {NULL};
int loadAcfCB( XPLMCommandRef cmdRef, XPLMCommandPhase inPhase, void *);
// one callback shared by eight similar commands. Identification is by comparing cmdRef with loadAcf[i]

PLUGIN_API int XPluginStart(
        char *        outName,
        char *        outSig,
        char *        outDesc) {

    // Plugin Info
    strcpy(outName, "AircraftSwitcher0.10");
    strcpy(outSig,  "Dozer.AircraftSwitcher");
    strcpy(outDesc, "Loads preset aircraft on command. v0.10");

    switchIni.loadFromFile();

    for(int i = 1; i <= 8; i++) {
        //loop through commandrefs and concatenate together the name and description
        stringstream commandName, commandDesc;
        commandName << "Dozer/AircraftSwitcher/load_aircraft_" << i;
        commandDesc << "Launches aircraft " << i << " as defined in AircraftSwitcher.ini";
        loadAcf[i] = XPLMCreateCommand(commandName.str().c_str(), commandDesc.str().c_str());
        XPLMRegisterCommandHandler(loadAcf[i], loadAcfCB, 1,0);
    }

    return 1;
}


PLUGIN_API void XPluginStop(void) {
    for (int i = 1; i <= 8; i++){
        XPLMUnregisterCommandHandler(loadAcf[i], loadAcfCB, 0, 0);
    }
}

PLUGIN_API void XPluginDisable(void) {}

PLUGIN_API int XPluginEnable(void) { return 1; }

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, long, void *) {}

int loadAcfCB( XPLMCommandRef cmdRef, XPLMCommandPhase inPhase, void * ) {
    if (inPhase == xplm_CommandEnd) {

        string aircraftPath;
        stringstream inikey;    //for concatenating the .ini key

        for (int i = 1; i<=8; i++) {
            if(cmdRef == loadAcf[i] ) {
                switchIni.loadFromFile();   //reload from .ini in case file has been updated

                inikey << "Aircraft" << i;  //concatenate to create .ini Key

                aircraftPath = switchIni.get("AircraftPaths",inikey.str()); //read from ini

                XPLMSetUsersAircraft(aircraftPath.c_str()); //load selected aircraft
            }
        }
    }
    return 0;
}
