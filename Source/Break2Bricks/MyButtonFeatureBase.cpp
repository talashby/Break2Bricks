// Fill out your copyright notice in the Description page of Project Settings.

#include "Break2Bricks.h"
#include "MyButtonFeatureBase.h"




UMyButtonFeatureBase::UMyButtonFeatureBase() : UButton()
{
	IsOpen = false;
	OpenCost = 100;
	OpenCostCups = 0;
}
