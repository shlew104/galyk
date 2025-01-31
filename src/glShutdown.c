#include "glCommon.h"
#include "glShutdown.h"
#include "glPrintDiagnosticRecord.h"
#include "glSelect.h"

glStatus glShutdownAbort( SQLHDBC aDbc )
{
    SQLHSTMT        sStmt    = NULL;
    int             sStatus  = 0; 

    GL_TRY_THROW( SQLAllocHandle ( SQL_HANDLE_STMT,
                                   aDbc,
                                   &sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_ALLOC_STMT );

    sStatus = 1;

    GL_TRY_THROW( SQLExecDirect ( sStmt,
                                  (SQLCHAR*)"ALTER SYSTEM CLOSE DATABASE ABORT",
                                  SQL_NTS ) == SQL_SUCCESS,
                  RAMP_ERR_SHUTDOWN_ABORT );              

    sStatus = 0;

    GL_TRY_THROW ( SQLFreeHandle ( SQL_HANDLE_STMT, sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_FREE_STMT );

    return GL_SUCCESS;

    GL_CATCH( RAMP_ERR_ALLOC_STMT )
    {
        printf("Error at SQLAllocHandle(stmt)\n");
    }

    GL_CATCH( RAMP_ERR_SHUTDOWN_ABORT )
    {
        printf("Error at Shutdown\n");
    }

    GL_CATCH( RAMP_ERR_FREE_STMT )
    {
        printf("Error at SQLFreeHandle(stmt)\n");
    }

    GL_FINISH;

    switch ( sStatus )
    {
        case 1:
            (void)SQLFreeHandle ( SQL_HANDLE_STMT, sStmt );
    }

    return GL_FAILURE;

}
