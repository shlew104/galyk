#ifndef GL_MENU_H
#define GL_MENU_H

#include "glCommon.h"

glStatus glGalykMenu( SQLHDBC      aDbc, 
                      glDsnInfo  * aDsnInfo, 
                      int          aDsnCount );

glStatus glStartupMenu( SQLHDBC      aDbc, 
                        char       * gNodeName,
                        glDsnInfo  * aDsnInfo,
                        int          aDsnCount ); 

#endif
