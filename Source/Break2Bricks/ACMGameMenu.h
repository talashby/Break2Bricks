// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateMachine/ACMachine.h"
/**
 * 
 */
class ACMPlayingField; typedef TSharedPtr<ACMPlayingField> SP_ACMPlayingField;
class ABreak2BricksPawn;

class ACMGameMenu : public ACMachine
{
public:
	ACMGameMenu(ABreak2BricksPawn *owner);
	virtual ~ACMGameMenu();

protected:
    virtual FName TickStateStart(int iTickType);
	FName TickStateGameMenu(int iTickType);
	FName TickStateModalDialog(int iTickType);
	FName TickStateModalDialogLevelInfo(int iTickType);
	FName TickStateModalDialogWorkshop(int iTickType);
	FName TickStateModalDialogUpgrade(int iTickType);
	FName TickStateFirstStart(int iTickType);
    FName TickStateGame(int iTickType);

private:
	void ShowModalDialog(class UMyButtonFeatureBase *pUMyButtonFeature_);
	void ShowLevelInfo(class UMyButtonLevel *pUMyButtonLevel_, bool bBackButtonVisible = true) const;

	SP_ACMPlayingField spACMPlayingField;
	ABreak2BricksPawn *pOwnerActor;
	class ABreak2BricksBlockGrid *pGridActor;
	class UBreak2BricksGameMenu *pGameMenu;
	//class ABreak2BricksGameMenu *pGameMenuActor;
	class UMyButtonFeatureBase *pUMyButtonPressed;
	int32 iModalDialogUpgrade_SelectedItem;
};

class ACMGameMenu; typedef TSharedPtr<ACMGameMenu> SP_ACMGameMenu;