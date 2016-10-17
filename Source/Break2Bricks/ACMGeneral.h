// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateMachine/ACMachine.h"
/**
 * 
 */
class ACMPlayingField; typedef TSharedPtr<ACMPlayingField> SP_ACMPlayingField;
class ABreak2BricksPawn;

class ACMGeneral : public ACMachine
{
public:
	ACMGeneral(ABreak2BricksPawn *owner);
	virtual ~ACMGeneral();

	void StartMenuStartGameButtonClicked();

protected:
    virtual FName TickStateStart(int iTickType);
	FName TickStateGameMenu(int iTickType);
    FName TickStateGame(int iTickType);

private:
    SP_ACMPlayingField spACMPlayingField;
	ABreak2BricksPawn *pOwnerActor;
	class ABreak2BricksBlockGrid *pGridActor;
	class UBreak2BricksGameMenu* pGameMenu;
	//class ABreak2BricksGameMenu *pGameMenuActor;
};

class ACMGeneral; typedef TSharedPtr<ACMGeneral> SP_ACMGeneral;