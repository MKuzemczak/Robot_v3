#ifndef ACTIONTYPE_H
#define ACTIONTYPE_H

#include <QString>
#include <QDebug>

typedef enum
{
    STRAIGHT_LINE,
    FREE,
    ARCH,
    LOCAL_CHANGE,
    DELAY,
    SINGLE,
    CONST_STRAIGHT,
    GRIPPER,
    ALL_ANGLES
} ActionType;

QString actionTypeToString(ActionType type);
ActionType stringToActionType(QString s);

#endif // ACTIONTYPE_H
