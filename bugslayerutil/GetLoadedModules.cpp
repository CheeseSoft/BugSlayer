/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "pch.h"
#include "BugslayerUtil.h"
#include <VersionHelpers.h>

// The project internal header file.
#include "Internal.h"

// The documentation for this function is in BugslayerUtil.h.
BOOL BUGSUTIL_DLLINTERFACE __stdcall
                           GetLoadedModules ( DWORD     dwPID        ,
                                              UINT      uiCount      ,
                                              HMODULE * paModArray   ,
                                              LPDWORD   pdwRealCount   )
{
    // Do the debug checking.
    ASSERT ( NULL != pdwRealCount ) ;
    ASSERT ( FALSE == IsBadWritePtr ( pdwRealCount , sizeof ( UINT ) ));
#ifdef _DEBUG
    if ( 0 != uiCount )
    {
        ASSERT ( NULL != paModArray ) ;
        ASSERT ( FALSE == IsBadWritePtr ( paModArray                   ,
                                          uiCount *
                                                sizeof ( HMODULE )   ));
    }
#endif

    // Do the parameter checking for real.  Note that I only check the
    // memory in paModArray if uiCount is > 0.  The user can pass zero
    // in uiCount if they are just interested in the total to be
    // returned so they could dynamically allocate a buffer.
    if ( ( TRUE == IsBadWritePtr ( pdwRealCount , sizeof(UINT) ) )    ||
         ( ( uiCount > 0 ) &&
           ( TRUE == IsBadWritePtr ( paModArray ,
                                     uiCount * sizeof(HMODULE) ) ) )   )
    {
        SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
        return ( FALSE ) ;
    }

	if (IsWindowsXPOrGreater())
	{
		// This is NT 4 so call its specific version in PSAPI.DLL
		return (NT4GetLoadedModules(dwPID,
			uiCount,
			paModArray,
			pdwRealCount));
	}
    else
    {
        // Win9x and Win2K go through tool help.
        return ( TLHELPGetLoadedModules ( dwPID         ,
                                          uiCount       ,
                                          paModArray    ,
                                          pdwRealCount   ) ) ;
    }
}

