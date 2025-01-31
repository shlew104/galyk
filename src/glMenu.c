#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <goldilocks.h>
#include "glMenu.h"
#include "glSelect.h"
#include "glShutdown.h"
#include "glStartup.h"
#include "glJoin.h"
#include "glCommon.h"
#include "glCompareScn.h"
#include "glGetMemberInfo.h"
#include "glRebalance.h"

/* Define */
char gNodeName[100] = "ALL";
char gDbNodeName[100] = "";
char gDataBuffer[256] = "";

/* GAYLK Main Menu */
glStatus glGalykMenu( SQLHDBC     aDbc,
                      glDsnInfo * aDsnInfo,
                      int         aDsnCount )
{
    /* Define */
    char sNumStr[10];
    int  sNum;
    char sMore[10] = "y";
    int  i;
    int  sCount = 0;
    int  sDsnCount = aDsnCount;
    int  sIsValidName = 0;

    while(1)
    {
        /* Main Menu, default Node is ALL */
        printf("\n==========================================\n");
        printf("  Welcome to G A L Y K !  \n");
        printf("==========================================\n");
        printf(" [1] STATUS \n");
        printf(" [2] CHOOSE NODE \n");
        printf(" [3] STARTUP \n");
        printf(" [4] SHUTDOWN \n");
        printf(" [5] REBALANCE \n");
        printf(" [6] EXIT \n");
        printf("==========================================\n");
        printf("%s> Enter number. \n", gNodeName);

        /* get menu number */
        fgets(sNumStr, sizeof(sNumStr), stdin);
        sNumStr[strcspn(sNumStr, "\n")] = '\0';

        /* type casting */
        sNum = atoi(sNumStr);

        /* Main Menu */
        switch(sNum)
        {
            /* STATUS Menu */
            case 1:
            {
                printf("%s> [STATUS] selected.\n", gNodeName);
                printf("Node Name: %s\n", gNodeName);

                /* ALL Node */
                if (strcasecmp(gNodeName, "ALL") == 0)
                {
                    /* for */
                    for (i=0; i<sDsnCount; i++)
                    {
                        if( SQLConnect ( aDbc,
                                         (SQLCHAR*)aDsnInfo[i].mName, /* 인자로 받은 dsn 이름 */
                                         SQL_NTS,
                                         (SQLCHAR*)"sys",
                                         SQL_NTS,
                                         (SQLCHAR*)"gliese as sysdba",
                                         SQL_NTS ) != SQL_SUCCESS )
                        {
                            printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                            GL_THROW(RAMP_SKIP_STATUS_ALL);
                        }

                        if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                        {
                            printf("glSelectMemberName failed.\n");
                            GL_THROW(RAMP_SKIP_STATUS_ALL);
                        }
                        printf("Connection : %s\n", gDbNodeName);

                        if( glSelectStatus( aDbc, aDsnInfo[i].mStatus ) != GL_SUCCESS )
                        {
                            printf("glSelectStatus failed.\n");
                            GL_THROW(RAMP_SKIP_STATUS_ALL);
                        } 
                        printf("STATUS : %s\n", aDsnInfo[i].mStatus);

                        if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                        {
                            printf("SQLDisconnect failed.\n");
                            GL_THROW(RAMP_SKIP_STATUS_ALL);
                        }
                        GL_RAMP(RAMP_SKIP_STATUS_ALL);  
                        SQLDisconnect(aDbc);
                     } 
                     /* for */
                } 
                /* ALL node */

                /* Choose Node */
                else
                {
                
                    if( SQLConnect ( aDbc,
                                     (SQLCHAR*)gNodeName,
                                     SQL_NTS,
                                     (SQLCHAR*)"sys",
                                     SQL_NTS,
                                     (SQLCHAR*)"gliese as sysdba",
                                     SQL_NTS ) != SQL_SUCCESS )
                    {
                        printf("%s SQLConnect failed.\n", gNodeName);
                        GL_THROW(RAMP_SKIP_STATUS_NODE);
                    }

                    if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                    {
                        printf("glSelectMemberName failed.\n");
                        GL_THROW(RAMP_SKIP_STATUS_NODE);
                    }
                    printf("Connection : %s\n", gDbNodeName);

                    if( glSelectStatus( aDbc, gDataBuffer ) != GL_SUCCESS )
                    {
                        printf("glSelectStatus failed.\n");
                        GL_THROW(RAMP_SKIP_STATUS_NODE);
                    } 
                    printf("STATUS : %s\n", gDataBuffer);

                    if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                    {
                        printf("SQLDisconnect failed.\n");
                        GL_THROW(RAMP_SKIP_STATUS_NODE);
                    }
                    GL_RAMP(RAMP_SKIP_STATUS_NODE);  
                    SQLDisconnect(aDbc);
                } 
                /* Choose node */
                break;
            }
        
            /* CHOOSE NODE Menu */
            case 2:
            {
                printf("%s> [CHOOSE NODE] selected. \n", gNodeName);

                /* print node(DSN) list */
                printf("\n==========================================\n");
                printf("[DSN LIST]\n");
                printf("==========================================\n");
                for( sCount = 0; sCount < sDsnCount; sCount++ )
                {
                        printf("[DSN#%d] : %s\n", sCount + 1, aDsnInfo[sCount].mName );
                }
                printf("==========================================\n");

                printf("%s> Enter node's DSN name :\n", gNodeName);
               
                /* get Node Name */
                fgets(gNodeName, sizeof(gNodeName), stdin);
                gNodeName[strcspn(gNodeName, "\n")] = '\0';

                /* Invalid node name check */
                if (gNodeName[0] == '\0') 
                {
                    printf("%s> Invalid node name. try again.", gNodeName);
                    GL_THROW(RAMP_SKIP_CHOOSE_NODE);
                }

                /* Is In the DSN names */
                for( i = 0; i < sDsnCount; i++ )
                {
                    if( strcmp( gNodeName, aDsnInfo[i].mName ) == 0 )
                    {
                        sIsValidName++;
                        break;
                    }
                }

                if( sIsValidName == 0 )
                {
                    printf("%s> Invalid node name. try again.", gNodeName);
                    GL_THROW(RAMP_SKIP_CHOOSE_NODE);
                }


                /* Main page with the Entered Node */
                printf("\n--------------------------------------\n");
                printf("\n%s> Welcome GOLDILOCKS Database [ %s ] ! \n", gNodeName, gNodeName);
                
                GL_RAMP(RAMP_SKIP_CHOOSE_NODE);
                break;
            }

            /* STARTUP Menu */
            case 3:
            {
                printf("%s> [STARTUP] selected.\n\n", gNodeName);

                /* STARTUP */
                if( glStartupMenu( aDbc, 
                                   gNodeName,
                                   aDsnInfo,
                                   sDsnCount ) != GL_SUCCESS )
                {
                    printf("glStartupMenu failed.\n");
                    GL_THROW(RAMP_SKIP_STARTUP_MENU);
                }
                GL_RAMP(RAMP_SKIP_STARTUP_MENU); 

                break;
            }

            /* SHUTDOWN Menu */
            case 4:
            {
                printf( "%s> [SHUTDOWN] selected.\n\n", gNodeName );

                printf( "Shutdown GOLDILOCKS Database? [y/n] \n" );
                fgets( sMore, sizeof( sMore ), stdin );
                sMore[ strcspn( sMore, "\n" ) ] = '\0';
                /* YES selected */
                if( sMore[0] == 'y' || sMore[0] == 'Y' )
                {
                    /* ALL Node */
                    if ( strcasecmp( gNodeName, "ALL" ) == 0 )
                    {
                        /* for */
                        for ( i=0; i<sDsnCount; i++ )
                        {
                            if( SQLConnect ( aDbc,
                                             (SQLCHAR*)aDsnInfo[i].mName,
                                             SQL_NTS,
                                             (SQLCHAR*)"sys",
                                             SQL_NTS,
                                             (SQLCHAR*)"gliese as sysdba",
                                             SQL_NTS ) != SQL_SUCCESS )
                            {
                                printf( "%s SQLConnect failed.\n", aDsnInfo[i].mName );
                                GL_THROW(RAMP_SKIP_SHUTDOWN_ALL);
                            }

                            if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                            {
                                printf( "glSelectMemberName failed.\n" );
                                GL_THROW(RAMP_SKIP_SHUTDOWN_ALL);
                            }
                            printf( "Connection : %s\n", gDbNodeName );

                            if( glShutdownAbort(aDbc) != GL_SUCCESS )
                            {
                                printf( "glShutdownAbort failed.\n" );
                                GL_THROW(RAMP_SKIP_SHUTDOWN_ALL);
                            }
                            printf("Shutdown success\n");

                            if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                            {
                                printf( "SQLDisconnect failed.\n" );
                                GL_THROW(RAMP_SKIP_SHUTDOWN_ALL);
                            }
                            GL_RAMP( RAMP_SKIP_SHUTDOWN_ALL );  

                            snprintf( aDsnInfo[i].mStatus,  
                                      MAX_STATUS_LENGTH,
                                      "UNKNOWN" );

                            SQLDisconnect( aDbc );
                        } 
                        /* for */
                    } 
                    /* ALL node */

                    /* Choose Node */
                    else
                    {
                        if( SQLConnect ( aDbc,
                                         (SQLCHAR*)gNodeName,
                                         SQL_NTS,
                                         (SQLCHAR*)"sys",
                                         SQL_NTS,
                                         (SQLCHAR*)"gliese as sysdba",
                                         SQL_NTS ) != SQL_SUCCESS )
                        {
                            printf("%s SQLConnect failed.\n", gNodeName);
                            GL_THROW(RAMP_SKIP_SHUTDOWN_CHOOSE_NODE);
                        }

                        if( glSelectMemberName(aDbc, gDbNodeName) != GL_SUCCESS )
                        {
                            printf("glSelectMemberName failed.\n");
                            GL_THROW(RAMP_SKIP_SHUTDOWN_CHOOSE_NODE);
                        }
                        printf("Connection : %s\n", gDbNodeName);

                        if( glShutdownAbort(aDbc) != GL_SUCCESS )
                        {
                            printf("glShutdownAbort failed.\n");
                            GL_THROW(RAMP_SKIP_SHUTDOWN_CHOOSE_NODE);
                        }
                        printf("Shutdown success\n");

                        if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                        {
                            printf("SQLDisconnect failed.\n");
                            GL_THROW(RAMP_SKIP_SHUTDOWN_CHOOSE_NODE);
                        }
                        GL_RAMP(RAMP_SKIP_SHUTDOWN_CHOOSE_NODE);  
                        SQLDisconnect(aDbc);
                    } 
                    /* Choose Node */
                }
                /* NO selected */
                else
                {
                    printf("Canceled.\n");
                    break;
                }
                break;
            }    

            /* REBALANCE Menu */
            case 5:
            {
                printf("%s> [REBALANCE] selected.\n", gNodeName);

                if( SQLConnect ( aDbc,
                                 (SQLCHAR*)gNodeName,
                                 SQL_NTS,
                                 (SQLCHAR*)"sys",
                                 SQL_NTS,
                                 (SQLCHAR*)"gliese as sysdba",
                                 SQL_NTS ) != SQL_SUCCESS )
                {
                    printf( "%s SQLConnect failed.\n", gNodeName );
                    GL_THROW(RAMP_SKIP_REBALANCE_ALL);
                }

                if( glDatabaseRebalance( aDbc, gDataBuffer ) != GL_SUCCESS )
                {
                    printf( "glDatabaseRebalance failed.\n" );
                    GL_THROW(RAMP_SKIP_REBALANCE_ALL);
                }
                printf("Rebalance success\n");

                if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                {
                    printf( "SQLDisconnect failed.\n" );
                    GL_THROW(RAMP_SKIP_REBALANCE_ALL);
                }
                GL_RAMP( RAMP_SKIP_REBALANCE_ALL );  
                SQLDisconnect( aDbc );
                break;
            }

            /* EXIT Menu */
            case 6:
            {
                printf("Terminate Service? [y/n] \n");
                fgets(sMore, sizeof(sMore), stdin);
                sMore[strcspn(sMore, "\n")] = '\0';

                if(sMore[0] == 'y' || sMore[0] == 'Y')
                {
                    printf("\n%s> Terminating service. Thank you for using GOLDILOCKS Database.\n\n", gNodeName);
                    GL_THROW(RAMP_SKIP_EXIT);
                }
                else
                {
                    printf("Cancled.\n");
                    break;
                }
            }
            /* Invalid number check */
            default:
            {
                printf("Invalid number. Try again.\n");
                break;
            }
        }
    }
    /* Repeat Main Page after Menu tab */
    GL_RAMP(RAMP_SKIP_EXIT);
    return GL_SUCCESS;
}

/* STARTUP Menu */
glStatus glStartupMenu( SQLHDBC      aDbc, 
                        char       * gNodeName,
                        glDsnInfo  * aDsnInfo,
                        int          aDsnCount )
{
    /* Define */
    int    sStartupNum;
    char   sNumStr[10];
    int    i;
    int    sDsnCount = aDsnCount;
    int    sOpenIndex = -1;
    int    sMaxScnNodeIndex = -1; 

    printf("------------------------------------------\n");
    printf("  [ STARTUP ] MENU  \n");
    printf("------------------------------------------\n");
    printf(" [1] MOUNT \n");
    printf(" [2] LOCAL OPEN \n");
    printf(" [3] GLOBAL OPEN \n");
    printf(" [4] JOIN \n");
    printf("------------------------------------------\n");
    printf("%s> Enter number.\n", gNodeName);

    /* get menu number */
    fgets(sNumStr, sizeof(sNumStr), stdin);
    sNumStr[strcspn(sNumStr, "\n")] = '\0';

    /* type casting */
    sStartupNum = atoi(sNumStr);

    /* Startup Menu */
    switch(sStartupNum)
    {
        /* MOUNT Menu */
        case 1:
        {
            printf("%s> [MOUNT] selected.\n", gNodeName);
            
            /* ALL Node */
            if (strcasecmp(gNodeName, "ALL") == 0)
            {
                /* for */
                for (i=0; i<sDsnCount; i++)
                {
                    if( SQLConnect ( aDbc,
                                     (SQLCHAR*)aDsnInfo[i].mName,
                                     SQL_NTS,
                                     (SQLCHAR*)"sys",
                                     SQL_NTS,
                                     (SQLCHAR*)"gliese as sysdba with startup",
                                     SQL_NTS ) != SQL_SUCCESS )
                    {
                        printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                        GL_THROW(RAMP_SKIP_MOUNT_ALL);
                    }

                    if( glStartupMount( aDbc, aDsnInfo[i].mStatus ) != GL_SUCCESS )
                    {
                        printf("glStartupMount failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNT_ALL);
                    }
                    printf("Mount success\n");

                    if( glSelectMemberName( aDbc, gDbNodeName) != GL_SUCCESS )
                    {
                        printf("glSelectMemberName failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNT_ALL);
                    }
                    printf("Connection : %s\n", gDbNodeName);

                    if( glSelectStatus( aDbc, aDsnInfo[i].mStatus) != GL_SUCCESS )
                    {
                        printf("glSelectStatus failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNT_ALL);
                    } 
                    printf("STATUS : %s\n", aDsnInfo[i].mStatus);

                    if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                    {
                        printf("SQLDisconnect failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNT_ALL);
                    }
                    GL_RAMP(RAMP_SKIP_MOUNT_ALL);  
                    SQLDisconnect(aDbc);
                } 
                /* for */
            } 
            /* ALL node */

            /* Choose Node */
            else
            {
                if( SQLConnect ( aDbc,
                                 (SQLCHAR*)gNodeName,
                                 SQL_NTS,
                                 (SQLCHAR*)"sys",
                                 SQL_NTS,
                                 (SQLCHAR*)"gliese as sysdba with startup",
                                 SQL_NTS ) != SQL_SUCCESS )
                {
                    printf("%s SQLConnect failed.\n", gNodeName);
                    GL_THROW(RAMP_SKIP_MOUNT_NODE);
                }

                if( glStartupMount( aDbc, gDataBuffer ) != GL_SUCCESS )
                {
                    printf("glStartupMount failed.\n");
                    GL_THROW(RAMP_SKIP_MOUNT_NODE);
                }
                printf("Mount success\n");

                if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                {
                    printf("glSelectMemberName failed.\n");
                    GL_THROW(RAMP_SKIP_MOUNT_NODE);
                }
                printf("Connection : %s\n", gDbNodeName);

                if( glSelectStatus( aDbc, gDataBuffer ) != GL_SUCCESS )
                {
                    printf("glSelectStatus failed.\n");
                    GL_THROW(RAMP_SKIP_MOUNT_NODE);
                } 
                printf("STATUS : %s\n", gDataBuffer);

                if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                {
                    printf("SQLDisconnect failed.\n");
                    GL_THROW(RAMP_SKIP_MOUNT_NODE);
                }
                GL_RAMP(RAMP_SKIP_MOUNT_NODE);  
                SQLDisconnect( aDbc );
            } 
            /* Choose node */
            break;
        }
        /* LOCAL OPEN Menu */
        case 2:
        {
            printf("%s> [LOCAL OPEN] selected.\n", gNodeName);

            /* ALL Node */
            if ( strcasecmp( gNodeName, "ALL" ) == 0 )
            {
                /* MOUNTED */
                if ( strcasecmp( gDataBuffer, "MOUNTED" ) == 0 )
                {
                    /* for */
                    for ( i=0; i<sDsnCount; i++ )
                    {
                        if( SQLConnect ( aDbc,
                                         (SQLCHAR*)aDsnInfo[i].mName,
                                         SQL_NTS,
                                         (SQLCHAR*)"sys",
                                         SQL_NTS,
                                         (SQLCHAR*)"gliese as sysdba",
                                         SQL_NTS ) != SQL_SUCCESS )
                        {
                            printf( "%s SQLConnect failed.\n", aDsnInfo[i].mName );
                            GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_ALL);
                        }

                        if( glStartupLocalOpen( aDbc, aDsnInfo[i].mStatus ) != GL_SUCCESS )
                        {
                            printf( "glStartupLocalOpen failed.\n" );
                            GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_ALL);
                        }

                        if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                        {
                            printf("glSelectMemberName failed.\n");
                            GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_ALL);
                        }
                        printf( "Connection : %s\n", gDbNodeName );

                        if( glSelectStatus( aDbc, aDsnInfo[i].mStatus ) != GL_SUCCESS )
                        {
                            printf( "glSelectStatus failed.\n" );
                            GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_ALL);
                        } 
                        printf("STATUS : %s\n", aDsnInfo[i].mStatus);

                        if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                        {
                            printf( "SQLDisconnect failed.\n" );
                            GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_ALL);
                        }
                        GL_RAMP( RAMP_SKIP_MOUNTED_LOCALOPEN_ALL );  
                        SQLDisconnect( aDbc );
                    } 
                    /* for */
                } 
                /* MOUNTED */

                /* Not Mounted */
                else
                {
                    /* for */
                    for ( i=0; i < sDsnCount; i++)
                    {
                        if( SQLConnect ( aDbc,
                                         (SQLCHAR*)aDsnInfo[i].mName,
                                         SQL_NTS,
                                         (SQLCHAR*)"sys",
                                         SQL_NTS,
                                         (SQLCHAR*)"gliese as sysdba with startup",
                                         SQL_NTS ) != SQL_SUCCESS )
                        {
                            printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                            GL_THROW(RAMP_SKIP_LOCALOPEN_ALL);
                        }

                        if( glStartupMount( aDbc, aDsnInfo[i].mStatus ) != GL_SUCCESS )
                        {
                            printf("glStartupMount failed.\n");
                            GL_THROW(RAMP_SKIP_LOCALOPEN_ALL);
                        }
                        printf("Mount success\n");

                        if( glStartupLocalOpen( aDbc, aDsnInfo[i].mStatus ) != GL_SUCCESS )
                        {
                            printf("glStartupLocalOpen failed.\n");
                            GL_THROW(RAMP_SKIP_LOCALOPEN_ALL);
                        }

                        if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                        {
                            printf("glSelectMemberName failed.\n");
                            GL_THROW(RAMP_SKIP_LOCALOPEN_ALL);
                        }
                        printf("Connection : %s\n", gDbNodeName);

                        if( glSelectStatus( aDbc, aDsnInfo[i].mStatus ) != GL_SUCCESS )
                        {
                            printf("glSelectStatus failed.\n");
                            GL_THROW(RAMP_SKIP_LOCALOPEN_ALL);
                        } 
                        printf("STATUS : %s\n", aDsnInfo[i].mStatus );

                        if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                        {
                            printf("SQLDisconnect failed.\n");
                            GL_THROW(RAMP_SKIP_LOCALOPEN_ALL);
                        }
                        GL_RAMP(RAMP_SKIP_LOCALOPEN_ALL);  
                        SQLDisconnect( aDbc );
                    } 
                    /* for */
                } 
                /* Not Mounted */
            } 
            /* ALL Node */

            /* Choose Node */
            else
            {
                /* MOUNTED */
                if (strcasecmp( gDataBuffer, "MOUNTED" ) == 0)
                {
                    if( SQLConnect ( aDbc,
                                     (SQLCHAR*)gNodeName,
                                     SQL_NTS,
                                     (SQLCHAR*)"sys",
                                     SQL_NTS,
                                     (SQLCHAR*)"gliese as sysdba",
                                     SQL_NTS ) != SQL_SUCCESS )
                    {
                        printf("%s SQLConnect failed.\n", gNodeName);
                        GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_NODE);
                    }

                    if( glStartupLocalOpen( aDbc, gDataBuffer ) != GL_SUCCESS )
                    {
                        printf("glStartupLocalOpen failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_NODE);
                    }

                    if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                    {
                        printf("glSelectMemberName failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_NODE);
                    }
                    printf("Connection : %s\n", gDbNodeName);

                    if( glSelectStatus( aDbc, gDataBuffer ) != GL_SUCCESS )
                    {
                        printf("glSelectStatus failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_NODE);
                    } 
                    printf("STATUS : %s\n", gDataBuffer);

                    if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                    {
                        printf("SQLDisconnect failed.\n");
                        GL_THROW(RAMP_SKIP_MOUNTED_LOCALOPEN_NODE);
                    }
                    GL_RAMP(RAMP_SKIP_MOUNTED_LOCALOPEN_NODE);  
                    SQLDisconnect( aDbc );
                } 
                /* Mounted */

                /* Not Mounted */
                else
                {
                    if( SQLConnect ( aDbc,
                                     (SQLCHAR*)gNodeName,
                                     SQL_NTS,
                                     (SQLCHAR*)"sys",
                                     SQL_NTS,
                                     (SQLCHAR*)"gliese as sysdba with startup",
                                     SQL_NTS ) != SQL_SUCCESS )
                    {
                        printf("%s SQLConnect failed.\n", gNodeName);
                        GL_THROW(RAMP_SKIP_LOCALOPEN_NODE);
                    }

                    if( glStartupMount( aDbc, gDataBuffer ) != GL_SUCCESS )
                    {
                        printf("glStartupMount failed.\n");
                        GL_THROW(RAMP_SKIP_LOCALOPEN_NODE);
                    }
                    printf("Mount success\n");

                    if( glStartupLocalOpen( aDbc, gDataBuffer ) != GL_SUCCESS )
                    {
                        printf("glStartupLocalOpen failed.\n");
                        GL_THROW(RAMP_SKIP_LOCALOPEN_NODE);
                    }

                    if( glSelectMemberName( aDbc, gDbNodeName) != GL_SUCCESS )
                    {
                        printf("glSelectMemberName failed.\n");
                        GL_THROW(RAMP_SKIP_LOCALOPEN_NODE);
                    }
                    printf("Connection : %s\n", gDbNodeName);

                    if( glSelectStatus( aDbc, gDataBuffer ) != GL_SUCCESS )
                    {
                        printf("glSelectStatus failed.\n");
                        GL_THROW(RAMP_SKIP_LOCALOPEN_NODE);
                    } 
                    printf("STATUS : %s\n", gDataBuffer);

                    if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                    {
                        printf("SQLDisconnect failed.\n");
                        GL_THROW(RAMP_SKIP_LOCALOPEN_NODE);
                    }
                    GL_RAMP(RAMP_SKIP_LOCALOPEN_NODE);  
                    SQLDisconnect( aDbc );
                } 
                /* Not Mounted */
            } 
            /* Choose node */
            break;
        }
        /* GLOBAL OPEN Menu */
        case 3:
        {
            printf("%s> [GLOBAL OPEN] selected.\n", gNodeName);

            /* ALL Node */
            if (strcasecmp(gNodeName, "ALL") == 0)
            {
                /* for All Node Status Check */
                for ( i=0; i < sDsnCount; i++ )
                {

                    /* Node 중 OPEN이 하나라도 있는지 Check */
                    if( strcmp( aDsnInfo[i].mStatus, "OPEN" ) == 0 )
                    {
                        sOpenIndex = i;
                        printf("%s : %s, openindex : %d\n", aDsnInfo[i].mName, aDsnInfo[i].mStatus, sOpenIndex);
                        continue;
                    }

                    /* Node 중 LOCAL OPEN 미만 STATUS가 있는지 Check */
                    if( strcmp( aDsnInfo[i].mStatus, "LOCAL OPEN" ) != 0 )
                    {   
                        printf("%s is not LOCAL OPEN and not OPEN. STATUS : %s\n", aDsnInfo[i].mName,
                               aDsnInfo[i].mStatus);
                        GL_THROW(RAMP_SKIP_GLOBAL_ALL);
                    }

                }

                /* if */
                /* OPEN인 STATUS가 없는 경우 즉, 전체 Node가 LOCAL OPEN 인 경우 */
                if ( sOpenIndex == -1 )
                {    
                    /* for */
                    for ( i=0; i < sDsnCount; i++ )
                    {
                        if( SQLConnect ( aDbc,
                                         (SQLCHAR*)aDsnInfo[i].mName,
                                         SQL_NTS,
                                         (SQLCHAR*)"sys",
                                         SQL_NTS,
                                         (SQLCHAR*)"gliese as sysdba",
                                         SQL_NTS ) != SQL_SUCCESS )
                        {
                            printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                            GL_THROW(RAMP_SKIP_GLOBAL_ALL);
                        }

                        GL_TRY_THROW( glGetScn( aDbc, aDsnInfo, i ) == GL_SUCCESS, RAMP_SKIP_GLOBAL_ALL );

                        if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                        {
                            printf("SQLDisconnect failed.\n");
                        }
                    }
                    /* end for */

                    GL_TRY_THROW( glCompareScn ( aDsnInfo, sDsnCount, &sMaxScnNodeIndex ) == GL_SUCCESS, RAMP_SKIP_GLOBAL_ALL );
                    printf("MAX : %d\n", sMaxScnNodeIndex);

                    if( SQLConnect ( aDbc,
                                     (SQLCHAR*)aDsnInfo[sMaxScnNodeIndex].mName,
                                     SQL_NTS,
                                     (SQLCHAR*)"sys",
                                     SQL_NTS,
                                     (SQLCHAR*)"gliese as sysdba",
                                     SQL_NTS ) != SQL_SUCCESS )
                    {
                        printf("%s SQLConnect failed.\n", aDsnInfo[sMaxScnNodeIndex].mName);
                        GL_THROW(RAMP_SKIP_GLOBAL_ALL);
                    }
                    
                    printf ( "%s started GLOBAL OPEN.\n", aDsnInfo[sMaxScnNodeIndex].mName );
                    GL_TRY_THROW ( glStartupGlobalOpen ( aDbc, gDataBuffer ) == GL_SUCCESS, RAMP_SKIP_GLOBAL_ALL );


                    if( glSelectStatus( aDbc, aDsnInfo[sMaxScnNodeIndex].mStatus ) != GL_SUCCESS )
                    {
                        printf("%s select status failed.\n", aDsnInfo[sMaxScnNodeIndex].mName);
                        GL_THROW(RAMP_SKIP_GLOBAL_ALL);
                    }
                    if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                    {
                        printf("SQLDisconnect failed.\n");
                    }


                    for ( i=0; i < sDsnCount; i++ )
                    {
                        if ( i != sMaxScnNodeIndex )
                        {
                            if( SQLConnect ( aDbc,
                                             (SQLCHAR*)aDsnInfo[i].mName,
                                             SQL_NTS,
                                             (SQLCHAR*)"sys",
                                             SQL_NTS,
                                             (SQLCHAR*)"gliese as sysdba",
                                             SQL_NTS ) != SQL_SUCCESS )
                            {
                                printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                                GL_THROW(RAMP_SKIP_GLOBAL_ALL);
                            }

                            printf ( "%s started JOIN.\n", aDsnInfo[i].mName );
                            GL_TRY_THROW ( glJoinDatabase ( aDbc, gDataBuffer ) == GL_SUCCESS, RAMP_SKIP_GLOBAL_ALL );
                            if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                            {
                                printf("SQLDisconnect failed.\n");
                            }       
                        }
                    }
                }

                /* Node 중 OPEN이 하나라도 있는 경우 */
                else
                {
                    for ( i=0; i < sDsnCount; i++ )
                    {
                        if ( strcmp( aDsnInfo[i].mStatus, "OPEN" ) != 0 )
                        {
                            if( SQLConnect ( aDbc,
                                             (SQLCHAR*)aDsnInfo[i].mName,
                                             SQL_NTS,
                                             (SQLCHAR*)"sys",
                                             SQL_NTS,
                                             (SQLCHAR*)"gliese as sysdba",
                                             SQL_NTS ) != SQL_SUCCESS )
                            {
                                printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                                GL_THROW(RAMP_SKIP_GLOBAL_ALL);
                            }

                            printf ( "%s started JOIN.\n", aDsnInfo[i].mName );
                            GL_TRY_THROW ( glJoinDatabase ( aDbc, gDataBuffer ) == GL_SUCCESS, RAMP_SKIP_GLOBAL_ALL );
                            if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                            {
                                printf("SQLDisconnect failed.\n");
                            }       
                        }
                    }

                }
                GL_RAMP(RAMP_SKIP_GLOBAL_ALL);  
                SQLDisconnect(aDbc);
                /* end if */
            }
            /* ALL node */

            /* Choose Node */
            else
            {
                if( SQLConnect ( aDbc,
                                 (SQLCHAR*)gNodeName,
                                 SQL_NTS,
                                 (SQLCHAR*)"sys",
                                 SQL_NTS,
                                 (SQLCHAR*)"gliese as sysdba",
                                 SQL_NTS ) != SQL_SUCCESS )
                {
                    printf("%s SQLConnect failed.\n", gNodeName);
                    GL_THROW(RAMP_SKIP_GLOBAL_NODE);
                }

                if( glStartupGlobalOpen( aDbc, gDataBuffer ) != GL_SUCCESS )
                {
                    printf("glStartupGlobalOpen failed.\n");
                    GL_THROW(RAMP_SKIP_GLOBAL_NODE);
                }

                if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                {
                    printf("glSelectMemberName failed.\n");
                    GL_THROW(RAMP_SKIP_GLOBAL_NODE);
                }
                printf("Connection : %s\n", gDbNodeName);

                if( glSelectStatus( aDbc, gDataBuffer ) != GL_SUCCESS )
                {
                    printf("glSelectStatus failed.\n");
                    GL_THROW(RAMP_SKIP_GLOBAL_NODE);
                } 
                printf("STATUS : %s\n", gDataBuffer);

                if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                {
                    printf("SQLDisconnect failed.\n");
                    GL_THROW(RAMP_SKIP_GLOBAL_NODE);
                }
                GL_RAMP(RAMP_SKIP_GLOBAL_NODE);  
                SQLDisconnect(aDbc);
            } 
            /* Choose node */
            break;
        }
        /* JOIN Menu */
        case 4:
        {
            printf("%s> [JOIN] selected.\n", gNodeName);
            /* ALL Node */
            if (strcasecmp(gNodeName, "ALL") == 0)
            {
                /* for All Node Status Check */
                for ( i=0; i < sDsnCount; i++ )
                {
                    /* Node 중 OPEN이 하나라도 있는지 Check */
                    if( strcmp( aDsnInfo[i].mStatus, "OPEN" ) == 0 )
                    {
                        sOpenIndex = i;
                    }

                    /* LOCAL OPEN 미만 STATUS가 있는지 Check */
                    if( strcmp( aDsnInfo[i].mStatus, "OPEN" ) != 0 && strcmp( aDsnInfo[i].mStatus, "LOCAL OPEN" ) != 0 )
                    {   
                        printf("%s is not LOCAL OPEN and not OPEN. STATUS : %s\n", aDsnInfo[i].mName,
                               aDsnInfo[i].mStatus);
                        GL_THROW(RAMP_SKIP_JOIN_ALL);
                    }

                    printf("%s : %s, openindex : %d\n", aDsnInfo[i].mName, aDsnInfo[i].mStatus, sOpenIndex);

                }

                /* if */
                /* OPEN인 STATUS가 없는 경우 즉, 전체 Node가 LOCAL OPEN 인 경우 */
                if ( sOpenIndex == -1 )
                {    
                    /* for */
                    for ( i=0; i < sDsnCount; i++ )
                    {
                        if( SQLConnect ( aDbc,
                                         (SQLCHAR*)aDsnInfo[i].mName,
                                         SQL_NTS,
                                         (SQLCHAR*)"sys",
                                         SQL_NTS,
                                         (SQLCHAR*)"gliese as sysdba",
                                         SQL_NTS ) != SQL_SUCCESS )
                        {
                            printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                            GL_THROW(RAMP_SKIP_JOIN_ALL);
                        }

                        GL_TRY_THROW( glGetScn( aDbc, aDsnInfo, i ) == GL_SUCCESS, RAMP_SKIP_JOIN_ALL );

                        if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                        {
                            printf("SQLDisconnect failed.\n");
                        }
                    }
                    /* end for */

                    GL_TRY_THROW( glCompareScn ( aDsnInfo, sDsnCount, &sMaxScnNodeIndex ) == GL_SUCCESS, RAMP_SKIP_JOIN_ALL );
                    printf("MAX : %d\n", sMaxScnNodeIndex);

                    if( SQLConnect ( aDbc,
                                     (SQLCHAR*)aDsnInfo[sMaxScnNodeIndex].mName,
                                     SQL_NTS,
                                     (SQLCHAR*)"sys",
                                     SQL_NTS,
                                     (SQLCHAR*)"gliese as sysdba",
                                     SQL_NTS ) != SQL_SUCCESS )
                    {
                        printf("%s SQLConnect failed.\n", aDsnInfo[sMaxScnNodeIndex].mName);
                        GL_THROW(RAMP_SKIP_JOIN_ALL);
                    }

                    printf ( "%s started GLOBAL OPEN.\n", aDsnInfo[sMaxScnNodeIndex].mName );
                    GL_TRY_THROW ( glStartupGlobalOpen ( aDbc, gDataBuffer ) == GL_SUCCESS, RAMP_SKIP_JOIN_ALL );


                    if( glSelectStatus( aDbc, aDsnInfo[sMaxScnNodeIndex].mStatus ) != GL_SUCCESS )
                    {
                        printf("%s select status failed.\n", aDsnInfo[sMaxScnNodeIndex].mName);
                        GL_THROW(RAMP_SKIP_JOIN_ALL);
                    }
                    if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                    {
                        printf("SQLDisconnect failed.\n");
                    }


                    for ( i=0; i < sDsnCount; i++ )
                    {
                        if ( i != sMaxScnNodeIndex )
                        {
                            if( SQLConnect ( aDbc,
                                             (SQLCHAR*)aDsnInfo[i].mName,
                                             SQL_NTS,
                                             (SQLCHAR*)"sys",
                                             SQL_NTS,
                                             (SQLCHAR*)"gliese as sysdba",
                                             SQL_NTS ) != SQL_SUCCESS )
                            {
                                printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                                GL_THROW(RAMP_SKIP_JOIN_ALL);
                            }

                            printf ( "%s started JOIN.\n", aDsnInfo[i].mName );
                            GL_TRY_THROW ( glJoinDatabase ( aDbc, gDataBuffer ) == GL_SUCCESS, RAMP_SKIP_JOIN_ALL );
                            if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                            {
                                printf("SQLDisconnect failed.\n");
                            }       
                        }
                    }
                }

                /* Node 중 OPEN이 하나라도 있는 경우 */
                else
                {
                    for ( i=0; i < sDsnCount; i++ )
                    {
                        if ( strcmp( aDsnInfo[i].mStatus, "OPEN" ) != 0 )
                        {
                            if( SQLConnect ( aDbc,
                                             (SQLCHAR*)aDsnInfo[i].mName,
                                             SQL_NTS,
                                             (SQLCHAR*)"sys",
                                             SQL_NTS,
                                             (SQLCHAR*)"gliese as sysdba",
                                             SQL_NTS ) != SQL_SUCCESS )
                            {
                                printf("%s SQLConnect failed.\n", aDsnInfo[i].mName);
                                GL_THROW(RAMP_SKIP_JOIN_ALL);
                            }

                            printf ( "%s started JOIN.\n", aDsnInfo[i].mName );
                            GL_TRY_THROW ( glJoinDatabase ( aDbc, gDataBuffer ) == GL_SUCCESS, RAMP_SKIP_JOIN_ALL );
                            if( SQLDisconnect( aDbc ) != SQL_SUCCESS )
                            {
                                printf("SQLDisconnect failed.\n");
                            }       
                        }
                    }

                }
                GL_RAMP(RAMP_SKIP_JOIN_ALL);  
                SQLDisconnect(aDbc);
                /* end if */
            }
            /* ALL node */


            /* Choose Node */
            else
            {    
                if( SQLConnect ( aDbc,
                                 (SQLCHAR*)gNodeName,
                                 SQL_NTS,
                                 (SQLCHAR*)"sys",
                                 SQL_NTS,
                                 (SQLCHAR*)"gliese as sysdba",
                                 SQL_NTS ) != SQL_SUCCESS )
                {
                    printf("%s SQLConnect failed.\n", gNodeName);
                    GL_THROW(RAMP_SKIP_JOIN);
                }

                if( glJoinDatabase( aDbc, gDataBuffer ) != GL_SUCCESS )
                {
                    printf("glJoinDatabase failed.\n");
                    GL_THROW(RAMP_SKIP_JOIN);
                }

                if( glSelectMemberName( aDbc, gDbNodeName ) != GL_SUCCESS )
                {
                    printf("glSelectMemberName failed.\n");
                    GL_THROW(RAMP_SKIP_JOIN);
                }
                printf("Connection : %s\n", gDbNodeName);

                if( glSelectStatus( aDbc, gDataBuffer ) != GL_SUCCESS )
                {
                    printf("glSelectStatus failed.\n");
                    GL_THROW(RAMP_SKIP_JOIN);
                } 
                printf("STATUS : %s\n", gDataBuffer);

                if( SQLDisconnect(aDbc) != SQL_SUCCESS )
                {
                    printf("SQLDisconnect failed.\n");
                    GL_THROW(RAMP_SKIP_JOIN);
                }
                GL_RAMP(RAMP_SKIP_JOIN);  
                SQLDisconnect(aDbc);
            }
                break;
        }

        /* Invalid number */
        default:
        {
            printf("Invalid number. Try again.\n");
            break;
        }
        
    } 
    return GL_SUCCESS;
}
