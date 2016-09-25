#pragma once

#include "StateMachine/ACMachine.h"
//#include "GameFramework/Actor.h"

//UCLASS(minimalapi)
class ACMPlayingField : /*public AActor, */public ACMachine
{
    //GENERATED_BODY()

    /** Dummy root component */
	//UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class USceneComponent* DummyRoot;
public:
    ACMPlayingField();
    virtual ~ACMPlayingField();

	/** Number of blocks along each side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		float BlockSpacing;

protected:

    virtual FName TickStateStart(int iTickType);
    FName TickStateGame(int iTickType);

private:


};

