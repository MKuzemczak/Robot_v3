
#include "actionmanager.h"



ActionManager::ActionManager()
{
    stepInProgress = false;
    checkCalculations = false;
    started = false;
    actionCntr = 0;
    robotPtr = nullptr;
}

ActionManager::ActionManager(Flags * f, SerialPort * arduino, QObject * parent) :
    QObject (parent)
{
    stepInProgress = false;
    checkCalculations = false;
    started = false;
    actionCntr = 0;
    robotPtr = nullptr;

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
    if (robotPtr == nullptr)
    {
        qDebug("Error: ActionManager::start() : robotPtr == NULL\n");
        return false;
    }

    actions[0]->calculate(*robotPtr);
    started = true;
    checkCalculations = true;
    calculations();

    nextStep();

    return true;
}

void ActionManager::stop()
{
    started = false;
    checkCalculations = false;
}

/*
 * Każde wywołanie metody BaseAction::execute() sprawia że akcja wykonuje część swojej roboty.
 *	Jeśli akcja uzna, że skończyła, ActionManager::nextStep() przechodzi do nastepnej akcji.
 **/
void ActionManager::nextStep()
{
    if (!stepInProgress && started && static_cast<int>(actions.size()) > 0)
    {

        stepInProgress = true;

        #ifdef DEBUG_ACTION_MANAGER
        //qDebug("ActionManager::nextStep() : poczatek\nactions.size() == %d,"
        //"actionCntr == %d\n", (int)actions.size(), actionCntr);
        #endif // DEBUG_ACTION_MANAGER


        if(actions[actionCntr]->size() > 0)
        {
            actions[actionCntr]->execute();

            qDebug("action no. %d\n", actionCntr);
#ifdef DEBUG_ACTION_MANAGER
            qDebug("ActionManager::nextStep() : execute(), actionCntr == %d", actionCntr );
#endif // DEBUG_ACTION_MANAGER

        }


        if (actions[actionCntr]->isDone() && actions[((actionCntr < static_cast<int>(actions.size()) - 1) ? (actionCntr + 1) : 0)]->isCalculated())
        {
            actionCntr++;
            checkCalculations = true;
            calculations();
        }

        if (flags->isSet(LOOP) && actionCntr == static_cast<int>(actions.size()))
        {
            actionCntr = 0;

#ifdef DEBUG_ACTION_MANAGER
            qDebug() << "ActionManager::nextStep() : LOOP";
#endif
        }

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

    int number;
    bool go = false;

    if (actionCntr < static_cast<int>(actions.size()) - 1)
    {
        number = actionCntr + 1;
        go = true;

    }
    else if (flags->isSet(LOOP))
    {
        number = 0;
        go = true;
    }

    if(go)
    {
        if (!actions[number]->isCalculated())
        {
            actions[number]->moveToThread(&calculationThread);

            connect(this, SIGNAL(startActionCalculations(Robot *)),
                    actions[number], SLOT(calcSlot(Robot *)));

            connect(actions[number], SIGNAL(calculationsFinished()),
                    this, SLOT(stopCalculationThread()));

            currentlyCalculated = actions[number];

            calculationThread.start();

            emit startActionCalculations(robotPtr);
            //actions[number]->calculate(*robotPtr);

#ifdef DEBUG_ACTION_MANAGER
            qDebug("ActionManager::calculations() : calc + 1\n");
#endif // DEBUG_ACTION_MANAGER

        }
    }

    checkCalculations = false;

}

void ActionManager::stopCalculationThread()
{
    calculationThread.quit();
//  currentlyCalculated->moveToThread(this->thread());

    disconnect(currentlyCalculated, SIGNAL(calculationsFinished()), nullptr, nullptr);
    disconnect(this, SIGNAL(startActionCalculations(Robot *)), nullptr, nullptr);

    currentlyCalculated = nullptr;
}

////////////////////////////////////////////////////////// setters & getters & adders

void ActionManager::setFlagsPtr(Flags * ptr)
{
    flags = ptr;

    for(int i = 0; i < static_cast<int>(actions.size()); i++)
    {
        actions[i]->setFlagsPtr(ptr);
    }
}

void ActionManager::setArduinoPortPtr(SerialPort * ptr)
{
    arduinoPort = ptr;

    for(int i = 0; i < static_cast<int>(actions.size()); i++)
    {
        actions[i]->setArduinoPortPtr(ptr);
    }
}
