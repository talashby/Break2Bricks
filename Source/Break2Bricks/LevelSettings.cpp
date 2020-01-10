
#include "Break2Bricks.h"
#include "LevelSettings.h"
#include "Break2BricksBlock.h"

FLevelSettings::FLevelSettings()
{
	BombsNum = 0;
	ColorBombsNum = 0;
	ManaBonus = false;
	TileTypesNum = 2;
	AutoAddTiles = false;
	AutoAddTilesRandomColumn = false;
}

void FLevelSettings::Add(const FLevelSettings &oFLevelSettings)
{
	BombsNum += oFLevelSettings.BombsNum;
	ColorBombsNum += oFLevelSettings.ColorBombsNum;
	ManaBonus |= oFLevelSettings.ManaBonus;
	ManaBonusChance += oFLevelSettings.ManaBonusChance;

	for (const FLevelItemsChance &itr_ext : oFLevelSettings.LevelItemsChance)
	{
		bool bAdded = false;
		for (FLevelItemsChance &itr_int : LevelItemsChance)
		{
			if (itr_ext.TileClass == itr_int.TileClass)
			{
				itr_int.TileChance += itr_ext.TileChance;
				bAdded = true;
			}
		}
		if (!bAdded)
		{
			LevelItemsChance.Push(itr_ext);
		}
	}
}
