// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateMachine/ACMachine.h"
/**
 * 
 */
class ACMGeneral : public ACMachine
{
public:
	ACMGeneral();
	virtual ~ACMGeneral();

protected:
    virtual FName TickStateStart(int iTickType);
    FName TickStateGame();
};

class ACMGeneral; typedef TSharedPtr<ACMGeneral> SP_ACMGeneral;