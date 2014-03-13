/************************************************************

 LJUCEApplication.cpp

    @author Christophe Berbizier (cberbizier@peersuasive.com)
    @license GPLv3
    @copyright 


(c) 2013, Peersuasive Technologies

*************************************************************/

#include "LJUCEApplication_inh.h"
const char LJUCEApplication::className[] = "LJUCEApplication";

const Luna<LJUCEApplication>::PropertyType LJUCEApplication::properties[] = {
    {0,0}
};

const Luna<LJUCEApplication>::FunctionType LJUCEApplication::methods[] = {
    method( LJUCEApplication, getApplicationName ),
    method( LJUCEApplication, getApplicationVersion ),
    method( LJUCEApplication, moreThanOneInstanceAllowed ),
    method( LJUCEApplication, initialise ),
    method( LJUCEApplication, initialised ),
    method( LJUCEApplication, suspended ),
    method( LJUCEApplication, resumed ),
    method( LJUCEApplication, unhandledException ),
    method( LJUCEApplication, shutdown ),
    method( LJUCEApplication, systemRequestedQuit ),
    method( LJUCEApplication, anotherInstanceStarted ),
    method( LJUCEApplication, quit ),

    // artificial
    method( LJUCEApplication, setDoubleClickTimeout ),

    {0,0}
};

const Luna<LJUCEApplication>::StaticType LJUCEApplication::statics[] = {
    smethod( LJUCEApplication, userHomeDirectory ),
    smethod( LJUCEApplication, userDocumentsDirectory ),
    smethod( LJUCEApplication, userDesktopDirectory ),
    smethod( LJUCEApplication, userMusicDirectory ),
    smethod( LJUCEApplication, userMoviesDirectory ),
    smethod( LJUCEApplication, userPicturesDirectory ),
    smethod( LJUCEApplication, userApplicationDataDirectory ),
    smethod( LJUCEApplication, commonApplicationDataDirectory ),
    smethod( LJUCEApplication, commonDocumentsDirectory ),
    smethod( LJUCEApplication, tempDirectory ),
    smethod( LJUCEApplication, currentExecutableFile ),
    smethod( LJUCEApplication, currentApplicationFile ),
    smethod( LJUCEApplication, invokedExecutableFile ),
    smethod( LJUCEApplication, hostApplicationPath ),
    smethod( LJUCEApplication, globalApplicationsDirectory ),

    {0,0}
};

LJUCEApplication::LJUCEApplication(lua_State *L)
    : LBase(L, "LJUCEApplication", false)
{
    myName( LUA::checkAndGetString(2, "LJUCEApplication") );
}

LJUCEApplication::~LJUCEApplication() {
    DBG("destroying MW...");
    if ( mainWindow )
        mainWindow = nullptr;

    DBG("END OF LJUCEApplication");
}

// Special paths

int LJUCEApplication::s_userHomeDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::userHomeDirectory).getFullPathName() );
}

int LJUCEApplication::s_userDocumentsDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::userDocumentsDirectory).getFullPathName() );
}

int LJUCEApplication::s_userDesktopDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::userDesktopDirectory).getFullPathName() );
}

int LJUCEApplication::s_userMusicDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::userMusicDirectory).getFullPathName() );
}

int LJUCEApplication::s_userMoviesDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::userMoviesDirectory).getFullPathName() );
}

int LJUCEApplication::s_userPicturesDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::userPicturesDirectory).getFullPathName() );
}

int LJUCEApplication::s_userApplicationDataDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::userApplicationDataDirectory).getFullPathName() );
}

int LJUCEApplication::s_commonApplicationDataDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::commonApplicationDataDirectory).getFullPathName() );
}

int LJUCEApplication::s_commonDocumentsDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::commonDocumentsDirectory).getFullPathName() );
}

int LJUCEApplication::s_tempDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::tempDirectory).getFullPathName() );
}

int LJUCEApplication::s_currentExecutableFile(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::currentExecutableFile).getFullPathName() );
}

int LJUCEApplication::s_currentApplicationFile(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::currentApplicationFile).getFullPathName() );
}

int LJUCEApplication::s_invokedExecutableFile(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::invokedExecutableFile).getFullPathName() );
}

int LJUCEApplication::s_hostApplicationPath(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::hostApplicationPath).getFullPathName() );
}

int LJUCEApplication::s_globalApplicationsDirectory(lua_State*) {
    return LUA::returnString( File::getSpecialLocation(File::globalApplicationsDirectory).getFullPathName() );
}

void LJUCEApplication::initialised() {
    if(hasCallback("initialised"))
        callback("initialised");
}
int LJUCEApplication::initialised(lua_State*) {
    set("initialised");
    return 0;
}

void LJUCEApplication::initialise (const String& commandLine) {
    int rc = callback("initialise", 1, { commandLine });
    if (rc != 1 ) {
        String error = LUA::getError();
        std::cout << "error at initialise method (" << rc << "): " << error << std::endl;
        lua_pushstring(LUA::Get(), error.toRawUTF8());
        lua_error(LUA::Get());
    }
    else
        mainWindow = LUA::from_luce<LComponent,Component>();
    initialised();
}

int LJUCEApplication::initialise( lua_State *L ) {
    set("initialise");
    return 0;
}

void LJUCEApplication::suspended() {
    if(hasCallback("suspended"))
        callback("suspended");
}
int LJUCEApplication::suspended(lua_State*) {
    set("suspended");
    return 0;
}

void LJUCEApplication::resumed() {
    if(hasCallback("resumed"))
        callback("resumed");
}
int LJUCEApplication::resumed(lua_State*) {
    set("resumed");
    return 0;
}
void LJUCEApplication::systemRequestedQuit() {
    if(hasCallback("systemRequestedQuit")) { 
        if(! callback("systemRequestedQuit") )
            JUCEApplication::quit();
    }
    else
        JUCEApplication::quit();
    /*
    int rc = callback( "systemRequestedQuit" );
    if (rc != 1)
        JUCEApplication::quit();
    */
}
int LJUCEApplication::systemRequestedQuit(lua_State *L) {
    set("systemRequestedQuit");
    return 0;
}

int LJUCEApplication::quit(lua_State *L) {
    JUCEApplication::quit();
    return 0;
}

const String LJUCEApplication::getApplicationName() {
    if(hasCallback("getApplicationName")) {
        callback("getApplicationName");
        return LUA::checkAndGetString(-1, myName());
    }
    return myName();
    //return ProjectInfo::projectName;
}
int LJUCEApplication::getApplicationName(lua_State *L) { 
    set("getApplicationName");
    return 0;
}

const String LJUCEApplication::getApplicationVersion() {
    if(hasCallback("getApplicationVersion")) {
        callback("getApplicationVersion");
        return LUA::checkAndGetString(-1, ProjectInfo::versionString);
    }
    return ProjectInfo::versionString; 
}
int LJUCEApplication::getApplicationVersion(lua_State *L) {
    set("getApplicationVersion");
    return 0;
}

bool LJUCEApplication::moreThanOneInstanceAllowed() {
    if ( hasCallback("moreThanOneInstanceAllowed") ) {
        callback("moreThanOneInstanceAllowed", 1);
        return LUA::checkAndGetBoolean(-1, true);
    }
    return true;
}
int LJUCEApplication::moreThanOneInstanceAllowed(lua_State *L) { 
    set("moreThanOneInstanceAllowed");
    return 0;
}

/// not a cb !
void LJUCEApplication::shutdown() {
    //callback("shutdown");
}
int LJUCEApplication::shutdown(lua_State *L) {
    // set("shutdown");
    this->shutdown();
    return 0;
}

void LJUCEApplication::anotherInstanceStarted (const String& commandLine) {
    if(hasCallback("anotherInstanceStarted"))
        callback("anotherInstanceStarted", 0, { commandLine } );
}
int LJUCEApplication::anotherInstanceStarted (lua_State *L) {
    set("anotherInstanceStarted");
    return 0;
}

void LJUCEApplication::unhandledException(const std::exception *e, const String& source, int line) {
    if(hasCallback("unhandledException")) {
        callback("unhandledException", 0, { e->what(), source, line });
    }
    else {
        lua_pushfstring(LUA::Get(), "Unhandled exception from JUCE: %s: %s, %d\n", e->what(), source.toRawUTF8(), line);
        lua_error(LUA::Get());
    }
}
int LJUCEApplication::unhandledException(lua_State *L) {
    set("unhandledException");
    return 0;
}

int LJUCEApplication::setDoubleClickTimeout(lua_State*) {
    MouseEvent::setDoubleClickTimeout(LUA::getNumber());
    return 0;
}
