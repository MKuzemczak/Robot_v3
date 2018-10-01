
#include "actionmanager.h"



ActionManager::ActionManager()
{
    stepInProgress = false;
    checkCalculations = false;
    started = false;
    calculationsRunning = false;
    actionCntr = 0;
    robotPtr = nullptr;
}

ActionManager::ActionManager(Flags * f, SerialPort * arduino, QObject * parent) :
    QObject (parent)
{
    stepInProgress = false;
    checkCalculations = false;
    started = false;
    calculationsRunning = false;
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
    if (!stepInProgress &&
            started &&
            static_cast<int>(actions.size()) > 0 &&
            !flags->isSet(STOP))
    {

        stepInProgress = true;

#ifdef DEBUG_ACTION_MANAGER
        qDebug("ActionManager::nextStep() : poczatek\nactions.size() == %d,"
               "actionCntr == %d", static_cast<int>(actions.size()), actionCntr);
#endif // DEBUG_ACTION_MANAGER


        if(!actions[actionCntr]->isDone())
        {
#ifdef DEBUG_ACTION_MANAGER
            qDebug("ActionManager::nextStep() : before actions[actionCntr]->execute();\n"
                   ", actionCntr == %d", actionCntr );
#endif // DEBUG_ACTION_MANAGER

            actions[actionCntr]->execute();


#ifdef DEBUG_ACTION_MANAGER
            qDebug("ActionManager::nextStep() : after actions[actionCntr]->execute();\n"
                   ", actionCntr == %d", actionCntr );
#endif // DEBUG_ACTION_MANAGER

        }


        if (actions[actionCntr]->isDone())
        {
            if(actions[((actionCntr < static_cast<int>(actions.size()) - 1) ? (actionCntr + 1) : 0)]->isCalculated())
            {
                actionCntr++;
                checkCalculations = true;

                if (flags->isSet(LOOP) && actionCntr == static_cast<int>(actions.size()))
                {
                    actionCntr = 0;

#ifdef DEBUG_ACTION_MANAGER
                    qDebug() << "ActionManager::nextStep() : LOOP";
#endif
                }
            }
            else
            {
                QTimer::singleShot(200, this, SLOT(nextStep()));
            }

            calculations();
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

    if(!calculationsRunning && !flags->isSet(STOP))
    {
        calculationsRunning = true;

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
        else
            calculationsRunning = false;

        if(go)
        {
            if (!actions[number]->isCalculated())
            {

                actions[number]->moveToThread(&calculationThread);

                connect(this, SIGNAL(startActionCalculations(Robot *)),
                        actions[number], SLOT(calcSlot(Robot *)));

                connect(actions[number], SIGNAL(calculationsFinished()),
                        this, SLOT(stopCalculationThreadFinished()));

                connect(actions[number], SIGNAL(calculationsFailed()),
                        this, SLOT(stopCalculationThreadFailed()));


                currentlyCalculated = actions[number];

                calculationThread.start();

                emit startActionCalculations(robotPtr);

#ifdef DEBUG_ACTION_MANAGER
                qDebug("ActionManager::calculations(), emit startActionCalculations(robotPtr);\n");
#endif // DEBUG_ACTION_MANAGER

            }
            else
                calculationsRunning = false;
        }

        checkCalculations = false;
    }

}

void ActionManager::stopCalculationThreadFinished()
{
    calculationThread.quit();

    disconnect(currentlyCalculated, SIGNAL(calculationsFinished()), nullptr, nullptr);
    disconnect(this, SIGNAL(startActionCalculations(Robot *)), nullptr, nullptr);

    currentlyCalculated = nullptr;
    calculationsRunning = false;
}

void ActionManager::stopCalculationThreadFailed()
{
    calculationThread.quit();

    qDebug() << "error: calculations failed in action "
             << ((actionCntr < static_cast<int>(actions.size()) - 1) ? (actionCntr + 1) : 0);

    flags->set(STOP);
    started = false;

    disconnect(currentlyCalculated, SIGNAL(calculationsFinished()), nullptr, nullptr);
    disconnect(this, SIGNAL(startActionCalculations(Robot *)), nullptr, nullptr);

    currentlyCalculated = nullptr;
    calculationsRunning = false;
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
