#include "glCommon.h"
#include "glCompareScn.h"

/* Compare SCN func */
glStatus glCompareScn ( glDsnInfo * aDsnInfo, int aMaxNodeNum, int * aMaxScnNodeIndex )
{
    int sMaxGlobalScn = 0;
	int sMaxDomainScn = 0;
	int sLoopCount    = 0;

    for( sLoopCount = 0; sLoopCount < aMaxNodeNum; sLoopCount++ )
	{
	    /* Compare Global Scn */
		if( sMaxGlobalScn < aDsnInfo[sLoopCount].mGlobalScn )
		{
			sMaxGlobalScn = aDsnInfo[sLoopCount].mGlobalScn;
			sMaxDomainScn = aDsnInfo[sLoopCount].mDomainScn;
			* aMaxScnNodeIndex = sLoopCount;
		}
		/* sMaxGlobalScn == mGlobalScn */
		else if( sMaxGlobalScn == aDsnInfo[sLoopCount].mGlobalScn )
		{
			/* Compare Domain Scn */
			if( sMaxDomainScn < aDsnInfo[sLoopCount].mDomainScn )
			{
				sMaxGlobalScn = aDsnInfo[sLoopCount].mGlobalScn;
				sMaxDomainScn = aDsnInfo[sLoopCount].mDomainScn;
				* aMaxScnNodeIndex = sLoopCount;
			}
		}
	}

	return GL_SUCCESS;
}
