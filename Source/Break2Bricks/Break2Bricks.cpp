// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Break2Bricks.h"


IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, Break2Bricks, "Break2Bricks");


//General Log
DEFINE_LOG_CATEGORY(LogGeneral);

namespace LogHelper
{
    bool CheckLogLevel(int iLogSeverityLevel)
    {
        if ((iLogSeverityLevel & ELogVerbosity::VerbosityMask) <= ELogVerbosity::COMPILED_IN_MINIMUM_VERBOSITY && (ELogVerbosity::Warning & ELogVerbosity::VerbosityMask) <= FLogCategoryLogGeneral::CompileTimeVerbosity)
        {
            return true;
        }
        return false;
    }
}