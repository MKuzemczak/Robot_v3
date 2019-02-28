
#include "actionmanager.h"



ActionManager::ActionManager()
{
    stepInProgress = false;
    checkCalculations = false;
    started = false;
    calculationsRunning = false;
    actionCntr = 0;
    robotPtr = nullptr;

    calculationThread.start();
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

    calculationThread.start();
}


ActionManager::~ActionManager()
{
    calculationThread.quit();
}

bool ActionManager::start()
{
    if (actions.size() == 0)
    {
        emit writeToTerminal("Error: ActionManager::start() : actions.size() == 0\n");
        return false;
    }
    if (robotPtr == nullptr)
    {
        emit writeToTerminal("Error: ActionManager::start() : robotPtr == NULL\n");
        return false;
    }

    actionCntr = 0;
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
    actionCntr = 0;
    if(calculationsRunning)
    {
        QTimer::singleShot(0, currentlyCalculated, SLOT(moveToParentThread));
    }

    for(int i = 0; i < static_cast<int>(actions.size()); i++)
    {
        actions[i]->clear();
    }
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


        if(!actions[actionCntr]->isDone() && actions[actionCntr]->isCalculated())
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
            emit stepped();
            int index;
            if(actionCntr < static_cast<int>(actions.size()) - 1)
            {
                index = actionCntr + 1;
            }
            else
            {
                if(flags->isSet(LOOP))
                {
                    index = 0;
                }
                else
                {
                    emit finished();
                    flags->set(STOP);
                    started = false;
                    stepInProgress = false;
                    return;
                }
            }

            if(actions[index]->isCalculated())
            {
                actionCntr++;
                checkCalculations = true;

                if(actionCntr == static_cast<int>(actions.size()))
                {
                    actionCntr = 0;
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

                if(!flags->isSet(STOP))
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
    disconnect(currentlyCalculated, SIGNAL(calculationsFinished()), nullptr, nullptr);
    disconnect(this, SIGNAL(startActionCalculations(Robot *)), nullptr, nullptr);

    currentlyCalculated = nullptr;
    calculationsRunning = false;
}

void ActionManager::stopCalculationThreadFailed()
{
    qDebug() << "error: calculations failed in action: ";
    emit writeToTerminal("error: calculations failed in action: ");
    emit writeToTerminal(((actionCntr < static_cast<int>(actions.size()) - 1) ? (actionCntr + 1) : 0));


    started = false;

    disconnect(currentlyCalculated, SIGNAL(calculationsFinished()), nullptr, nullptr);
    disconnect(this, SIGNAL(startActionCalculations(Robot *)), nullptr, nullptr);

    currentlyCalculated = nullptr;
    calculationsRunning = false;
}

void ActionManager::deleteAction(int i)
{
    actions.erase(i);
}

void ActionManager::moveAction(int oldIndex, int newIndex)
{
    if(oldIndex == newIndex)
    {
        qDebug() << "ActionManager::moveAction() : indeksy takie same.";
        emit writeToTerminal("Indeksy takie same.");
        return;
    }

    if(oldIndex < newIndex)
    {
        if(newIndex == static_cast<int>(actions.size()) - 1)
        {
            actions.push_back(actions[oldIndex]);
        }
        else
        {
            actions.insert(actions.iteratorAt(newIndex + 1), actions[oldIndex]);
        }
        actions.erase(oldIndex);
        return;
    }

    actions.insert(actions.iteratorAt(newIndex), actions[oldIndex]);
    actions.erase(oldIndex + 1);
}

////////////////////////////////////////////////////////// setters & getters & adders


int ActionManager::size()
{
    return static_cast<int>(actions.size());
}

void ActionManager::addStraightLineMovAction(Eigen::Vector3d & start,
                                             Eigen::Vector3d & dest,
                                             int spd)
{
    actions.push_back(new StraightLineMovAction(start,
                                                dest,
                                                spd,
                                                arduinoPort,
                                                flags));
    actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    emit writeToTerminal("akcja ruchu po linii prostej dodana");
}

void ActionManager::addFreeMovAction(Eigen::Vector3d & dest,
                                     int spd)
{
    actions.push_back(new FreeMovAction(dest,
                                        spd,
                                        arduinoPort,
                                        flags));
    actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    emit writeToTerminal("Akcja ruchu wolnego dodana");

}

void ActionManager::addArchMovAction(Eigen::Vector3d start,
                                     Eigen::Vector3d inter,
                                     Eigen::Vector3d dest,
                                     int spd)
{
    actions.push_back(new ArchMovAction(start,
                                        inter,
                                        dest,
                                        spd,
                                        arduinoPort,
                                        flags));
    actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    emit writeToTerminal("Akcja ruchu po łuku dodana");
}

void ActionManager::addConstTCPOrientAction(Eigen::Vector3d & start,
                                            Eigen::Vector3d & dest,
                                            int spd)
{
    actions.push_back(new ConstTCPOrientAction(start,
                                               dest,
                                               spd,
                                               arduinoPort,
                                               flags));
    actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    emit writeToTerminal("Akcja ruchu ze stałą orientacją ostatniego członu dodana");
#ifdef DEBUG_ACTION_MANAGER
    qDebug() << "ActionManager::addConstTCPOrientAction() : added thread: "
             << actions[static_cast<int>(actions.size()) - 1]->thread();
#endif
}

void ActionManager::addSetSingleJointAction(int joint,
                                            int thetaDeg,
                                            int spd,
                                            bool constTCPlocation)
{
    actions.push_back(new SetSingleJointAction(joint,
                                               thetaDeg,
                                               spd,
                                               constTCPlocation,
                                               arduinoPort,
                                               flags));
    actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    emit writeToTerminal("Akcja ustawienia jednego przegubu dodana");
}

void ActionManager::addGripperAction(int set)
{
    actions.push_back(new GripperAction(set, arduinoPort, flags));
    actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    emit writeToTerminal("Akcja chwytaka dodana");
}

void ActionManager::addSetAllAnglesAction(Lista<int> l,
                                          int spd)
{
    actions.push_back(new setAllAnglesAction(l,
                                             spd,
                                             arduinoPort,
                                             flags));
    actions[static_cast<int>(actions.size()) - 1]->setParentThreadPtr(this->thread());
    emit writeToTerminal("Akcja ustawiania wszytskich przegubów dodana");
}

bool ActionManager::isCheckCalculations()
{
    return checkCalculations;
}

void ActionManager::setRobotPtr(Robot * ptr)
{
    robotPtr = ptr;
}

bool ActionManager::isStarted()
{
    return started;
}

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
