#include "actionmanager.h"



ActionManager::ActionManager()
{
    stepInProgress = false;
    checkCalculations = false;
    started = false;
    actionCntr = 0;
    robotPtr = NULL;
}

ActionManager::ActionManager(Flags * f, SerialPort * arduino)
{
    stepInProgress = false;
    checkCalculations = false;
    started = false;
    actionCntr = 0;
    robotPtr = NULL;

    flags = f;
    arduinoPort = arduino;
}


ActionManager::~ActionManager()
{
}

bool ActionManager::start()
{
    if (actions.size() == 0)
    {
        qDebug("Error: ActionManager::start() : actions.size() == 0\n");
        return false;
    }
    if (robotPtr == NULL)
    {
        qDebug("Error: ActionManager::start() : robotPtr == NULL\n");
        return false;
    }

    actions[0]->calculate(*robotPtr);
    started = true;
    checkCalculations = true;

    return true;
}

/*
 * Każde wywołanie metody BaseAction::execute() sprawia że akcja wykonuje część swojej roboty.
 *	Jeśli akcja uzna, że skończyła, ActionManager::nextStep() przechodzi do nastepnej akcji.
 **/
void ActionManager::nextStep()
{
    if (!stepInProgress)
    {

        stepInProgress = true;

        #ifdef DEBUG_ACTION_MANAGER
        //qDebug("ActionManager::nextStep() : poczatek\nactions.size() == %d,"
        "actionCntr == %d\n", (int)actions.size(), actionCntr);
        #endif // DEBUG_ACTION_MANAGER


        if(actions.size() > 0 && actions[actionCntr]->size() > 0)
        {
            actions[actionCntr]->execute();

            qDebug("action no. %d\n", actionCntr);
#ifdef DEBUG_ACTION_MANAGER
            qDebug("ActionManager::nextStep() : execute(), actionCntr == %d", actionCntr );
#endif // DEBUG_ACTION_MANAGER

        }


        if (actions[actionCntr]->isDone() && actions[((actionCntr < actions.size() - 1) ? (actionCntr + 1) : 0)]->isCalculated())
        {
            actionCntr++;
            checkCalculations = true;
        }

        if (flags->isSet(LOOP) && actionCntr == (int)actions.size())
            actionCntr = 0;

        #ifdef DEBUG_ACTION_MANAGER
        //qDebug("ActionManager::execute() : koniec\n");
        #endif // DEBUG_ACTION_MANAGER


            stepInProgress = false;
    }
}

void ActionManager::calculations()
{
#ifdef DEBUG_ACTION_MANAGER
    qDebug("ActionManager::calculations() : actionCntr + 1 == %d", actionCntr + 1 );
#endif // DEBUG_ACTION_MANAGER

    if (actionCntr < (int)actions.size() - 1)
    {
        if (!actions[actionCntr + 1]->isCalculated())
        {
            actions[actionCntr + 1]->calculate(*robotPtr);

#ifdef DEBUG_ACTION_MANAGER
            qDebug("ActionManager::calculations() : calc + 1\n");
#endif // DEBUG_ACTION_MANAGER

        }

    }
    else if (flags->isSet(LOOP))
    {
        if (!actions[0]->isCalculated())
        {
            actions[0]->calculate(*robotPtr);

#ifdef DEBUG_ACTION_MANAGER
            qDebug("ActionManager::calculations() : calc 0\n");
#endif // DEBUG_ACTION_MANAGER

        }
    }

    checkCalculations = false;

}

////////////////////////////////////////////////////////// setters & getters & adders

void ActionManager::setFlagsPtr(Flags * ptr)
{
    flags = ptr;

    //TODO: petla przeleciec tutaj przez wszystkie akcje i zmienic w nich wskaznik na flags
}

void ActionManager::setArduinoPortPtr(SerialPort * ptr)
{
    arduinoPort = ptr;
}
