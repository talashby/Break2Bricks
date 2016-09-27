#pragma once

#include "StateMachine/ACMachine.h"
class ABreak2BricksPawn;

class ACMPlayingField : public ACMachine
{
public:
    ACMPlayingField(ABreak2BricksPawn *owner);
    virtual ~ACMPlayingField();

protected:

    virtual FName TickStateStart(int iTickType);
    FName TickStateGame(int iTickType);

private:
	ABreak2BricksPawn *pOwnerActor;
	class ABreak2BricksBlockGrid *pGridActor;

};

