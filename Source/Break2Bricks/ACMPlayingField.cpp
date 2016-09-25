

#include "Break2Bricks.h"
#include "ACMPlayingField.h"


static const FName g_ssGame("Game");

ACMPlayingField::ACMPlayingField() : ACMachine("ACMPlayingField")
{
    REGISTER_ACSTATE(ACMPlayingField, Game);

    // Set defaults
    Size = 3;
    BlockSpacing = 300.f;
}


ACMPlayingField::~ACMPlayingField()
{
}

FName ACMPlayingField::TickStateStart(int iTickType)
{
    if (ACMachine::TICK_StateStarted == iTickType)
    {
/*
        // Number of blocks
        const int32 NumBlocks = Size * Size;

        // Loop to spawn each block
        for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
        {
            const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
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
