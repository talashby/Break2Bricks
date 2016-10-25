// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateMachine/ACMachine.h"
/**
 * 
 */
class ACMGameMenu; typedef TSharedPtr<ACMGameMenu> SP_ACMGameMenu;
class ABreak2BricksPawn;

class ACMGeneral : public ACMachine
{
public:
	ACMGeneral(ABreak2BricksPawn *owner);
	virtual ~ACMGeneral();

protected:
    virtual FName TickStateStart(int iTickType);
	FName TickStateGameMenu(int iTickType);

private:
    SP_ACMGameMenu spACMGameMenu;
	ABreak2BricksPawn *pOwnerActor;
	//class ABreak2BricksBlockGrid *pGridActor;
};

class ACMGeneral; typedef TSharedPtr<ACMGeneral> SP_ACMGeneral;