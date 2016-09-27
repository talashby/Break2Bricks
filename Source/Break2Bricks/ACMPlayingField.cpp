

#include "Break2Bricks.h"
#include "ACMPlayingField.h"
#include "Break2BricksBlockGrid.h"
#include "Break2BricksBlock.h"
#include "Break2BricksPawn.h"
#include "EngineUtils.h"

static const FName g_ssGame("Game");

ACMPlayingField::ACMPlayingField(ABreak2BricksPawn *owner) : ACMachine("ACMPlayingField")
{
	pOwnerActor = owner;

    REGISTER_ACSTATE(ACMPlayingField, Game);
}


ACMPlayingField::~ACMPlayingField()
{
}

FName ACMPlayingField::TickStateStart(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
		// Spawn a block
		TSubclassOf<ABreak2BricksPawn> ClassToFind;
		TArray<AActor*> FoundActors;
		//UGameplayStatics::GetAllActorsOfClass(pOwnerActor->GetWorld(), ClassToFind, FoundActors);
		//int ttt = FoundActors.Num();
		//check(FoundActors.Num() != 1);

		TActorIterator<ABreak2BricksBlockGrid> ActorItr = TActorIterator<ABreak2BricksBlockGrid>(pOwnerActor->GetWorld());
		if (ActorItr)
		{
			pGridActor = *ActorItr;
		}
		else
		{
			return ErrorState("ABreak2BricksBlockGrid not found");
		}

		//ABreak2BricksBlockGrid* NewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlockGrid>();

        // Loop to spawn each block
		for (int32 BlockIndexX = 0; BlockIndexX < pGridActor->SizeX; ++BlockIndexX)
		{
			for (int32 BlockIndexY = 0; BlockIndexY < pGridActor->SizeY; ++BlockIndexY)
			{
				const float YOffset = BlockIndexX * pGridActor->BlockSpacing - (pGridActor->SizeX/2.f * pGridActor->BlockSpacing);
				const float XOffset = BlockIndexY * pGridActor->BlockSpacing - (pGridActor->SizeY/2.f * pGridActor->BlockSpacing);
				// Make postion vector, offset from Grid location
				const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + pGridActor->GetActorLocation();

				// Spawn a block
				ABreak2BricksBlock* NewBlock = pOwnerActor->GetWorld()->SpawnActor<ABreak2BricksBlock>(BlockLocation, FRotator(0, 0, 0));

				// Tell the block about its owner
				if (NewBlock != nullptr)
				{
					NewBlock->OwningGrid = pGridActor;
				}
			}
		}
        /*for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
        {
            const float XOffset = (BlockIndex / Size) * pGridActor->BlockSpacing; // Divide by dimension
            const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

                                                                    // Make postion vector, offset from Grid location
            const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

            // Spawn a block
            ABreak2BricksBlock* NewBlock = GetWorld()->SpawnActor<ABreak2BricksBlock>(BlockLocation, FRotator(0, 0, 0));

            // Tell the block about its owner
            if (NewBlock != nullptr)
            {
                NewBlock->OwningGrid = this;
            }
        }*/

        return g_ssGame;
    }
    return "";
}

FName ACMPlayingField::TickStateGame(int iTickType)
{
    return "";
}
