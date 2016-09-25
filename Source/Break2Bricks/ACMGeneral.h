// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateMachine/ACMachine.h"
/**
 * 
 */
class ACMPlayingField; typedef TSharedPtr<ACMPlayingField> SP_ACMPlayingField;

class ACMGeneral : public ACMachine
{
public:
	ACMGeneral();
	virtual ~ACMGeneral();

protected:
    virtual FName TickStateStart(int iTickType);
    FName TickStateGame(int iTickType);

private:
    SP_ACMPlayingField spACMPlayingField;
};

class ACMGeneral; typedef TSharedPtr<ACMGeneral> SP_ACMGeneral;