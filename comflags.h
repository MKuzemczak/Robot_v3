#ifndef COMFLAGS_H
#define COMFLAGS_H

#include <QObject>

#define NUM_OF_FLAGS 5

#define FLAG_OFFSET 'A'

#define ARDUINO_CONNECTED FLAG_OFFSET+0x00
#define ARDUINO_MOV_FIN FLAG_OFFSET+0x01
#define SIM_INPUTS FLAG_OFFSET+0x02
#define STOP FLAG_OFFSET+0x03
#define LOOP FLAG_OFFSET+0x04



class Flags : public QObject
{
    Q_OBJECT

    char tab[NUM_OF_FLAGS];

    int currentlySetFlag;

public:
    Flags()
    {

    }

    Flags(QObject * parent) : QObject(parent) {}

    void set(int i)
    {
        tab[i-FLAG_OFFSET] = 1;

        if(i != LOOP && i != SIM_INPUTS && i != STOP)
            currentlySetFlag = i - FLAG_OFFSET;

        if(i == ARDUINO_MOV_FIN)
            emit movFinReceived();
    }

    void reset(int i)
    {
        tab[i - FLAG_OFFSET] = 0;

        if (i != LOOP && i != SIM_INPUTS && i != STOP)
            currentlySetFlag = -1;
    }

    void toggle(int i)
    {
        if (tab[i - FLAG_OFFSET] == 1)
        {
            tab[i - FLAG_OFFSET] = 0;

            if (i != LOOP && i != SIM_INPUTS && i != STOP)
                currentlySetFlag = -1;
        }
        else
        {
            tab[i - FLAG_OFFSET] = 1;

            if (i != LOOP && i != SIM_INPUTS && i != STOP)
                currentlySetFlag = i - FLAG_OFFSET;
        }
    }

    int getCurrentRcvFlag()
    {
        return currentlySetFlag;
    }

    void resetCurrentRcvFlag()
    {
        reset(currentlySetFlag);
    }

    bool isSet(int i)
    {
        if (tab[i - FLAG_OFFSET] == 1)
            return true;

        return false;

    }

signals:
    void movFinReceived();
};




#endif // COMFLAGS_H
