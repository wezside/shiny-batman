#include <signal.h>
#include <errno.h>

#include "signalhandler.hpp"

bool wezside::SignalHandler::mbGotExitSignal = false;

/**
* Default Contructor.
*/
wezside::SignalHandler::SignalHandler()
{
}

/**
* Destructor.
*/
wezside::SignalHandler::~SignalHandler()
{
}

/**
* Returns the bool flag indicating whether we received an exit signal
* @return Flag indicating shutdown of program
*/
bool wezside::SignalHandler::gotExitSignal()
{
    return mbGotExitSignal;
}

/**
* Sets the bool flag indicating whether we received an exit signal
*/
void wezside::SignalHandler::setExitSignal(bool _bExitSignal)
{
    mbGotExitSignal = _bExitSignal;
}

/**
* Sets exit signal to true.
* @param[in] _ignored Not used but required by function prototype
*                     to match required handler.
*/
void wezside::SignalHandler::exitSignalHandler(int _ignored)
{
    mbGotExitSignal = true;
}

/**
* Set up the signal handlers for CTRL-C.
*/
void wezside::SignalHandler::setupSignalHandlers()
{
    if (signal((int) SIGINT, wezside::SignalHandler::exitSignalHandler) == SIG_ERR)
    {
        throw SignalException("!!!!! Error setting up signal handlers !!!!!");
    }
}
