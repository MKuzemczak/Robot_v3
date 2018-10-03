#include "actiontype.h"

QString actionTypeToString(ActionType type)
{
    switch(type)
    {
    case STRAIGHT_LINE:
        return QString("Linia prosta");
        break;
    case FREE:
        return QString("Wolny ruch");
        break;
    case ARCH:
        return QString("Ruch po łuku");
        break;
    case LOCAL_CHANGE:
        return QString("Local change");
        break;
    case DELAY:
        QString("Postój");
        break;
    case SINGLE:
        QString("Pojedynczy przegub");
        break;
    case CONST_STRAIGHT:
        QString("constTCPorient");
        break;
    case GRIPPER:
        QString("Chwytak");
        break;
    }
}

ActionType stringToActionType(QString s)
{
    if (s == "Linia prosta")
        return STRAIGHT_LINE;
    else if(s == "Wolny ruch")
        return FREE;
    else if(s == "Ruch po łuku")
        return ARCH;
    else if(s == "Local change")
        return LOCAL_CHANGE;
    else if(s == "Postój")
        return DELAY;
    else if(s == "Pojedynczy przegub")
        return SINGLE;
    else if(s == "constTCPorient")
        return CONST_STRAIGHT;
    else if(s == "Chwytak")
        return GRIPPER;
}
