#include "glCommon.h"
#include "glSelect.h"
#include "glPrintDiagnosticRecord.h"
#include "glMenu.h"

glStatus glSelectOneValue( SQLHDBC aDbc, char * aSql, char * aDataBuffer)
{
    SQLHSTMT        sStmt    = NULL;
    int             sStatus  = 0;

    GL_TRY_THROW ( SQLAllocHandle ( SQL_HANDLE_STMT,
                                    aDbc,
                                    &sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_ALLOC_STMT );
    sStatus = 1;

    GL_TRY_THROW ( SQLExecDirect ( sStmt,
                                   (SQLCHAR*) aSql,
                                   SQL_NTS ) == SQL_SUCCESS,
                   RAMP_ERR_SELECT_ONE_STRING ); 

    GL_TRY_THROW ( SQLBindCol( sStmt,
                               1,
                               SQL_C_CHAR,
                               aDataBuffer,
                               BUF_LEN,
                               NULL ) == SQL_SUCCESS,
                   RAMP_ERR_BIND_COL );

    GL_TRY_THROW ( SQLFetch ( sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_FETCH );

    sStatus = 0;

    GL_TRY_THROW ( SQLFreeHandle ( SQL_HANDLE_STMT, sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_FREE_STMT );

    return GL_SUCCESS;

    GL_CATCH ( RAMP_ERR_ALLOC_STMT )
    {
        printf ("Error at SQLAllocHandle(stmt)\n");
    }

    GL_CATCH ( RAMP_ERR_SELECT_ONE_STRING )
    {
        printf ("Error at select one string\n");
    }

    GL_CATCH ( RAMP_ERR_BIND_COL )
    {
        printf ("Error at Bind Column\n");
    }

    GL_CATCH ( RAMP_ERR_FETCH )
    {
        printf ("Error at Fetch\n");
    }

    GL_CATCH ( RAMP_ERR_FREE_STMT )
    {
        printf ("Error at SQLFreeHandle(stmt)\n");
    }

    GL_FINISH;

    switch ( sStatus )
    {
        case 1:
            (void) SQLFreeHandle ( SQL_HANDLE_STMT, sStmt );
    }

    return GL_FAILURE;
}

glStatus glSelectMemberName( SQLHDBC aDbc, char * aDataBuffer)
{
    GL_TRY ( glSelectOneValue ( aDbc , 
                                "select LOCAL_MEMBER_NAME from x$instance@local", 
                                aDataBuffer ) == GL_SUCCESS );

    return GL_SUCCESS;

    GL_FINISH;

    return GL_FAILURE;

}

glStatus glSelectScn( SQLHDBC aDbc, char * aDataBuffer )
{
    GL_TRY ( glSelectOneValue ( aDbc , 
                                "select LOCAL_SCN from x$agable_info@local", 
                                aDataBuffer) == GL_SUCCESS );

    return GL_SUCCESS;

    GL_FINISH;

    return GL_FAILURE;

}

glStatus glSelectStatus( SQLHDBC aDbc, char * aDataBuffer)
{
    GL_TRY ( glSelectOneValue ( aDbc ,
                                "select STATUS from x$instance@local",
                                aDataBuffer ) == GL_SUCCESS );

    return GL_SUCCESS;

    GL_FINISH;

    return GL_FAILURE;

}
