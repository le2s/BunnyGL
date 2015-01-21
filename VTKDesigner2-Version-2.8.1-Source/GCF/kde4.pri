######################################################################
# Generic Component Framework Library from VCreate Logic
# Project include file that enables the use of KDE/KParts 
# in GCF
######################################################################

unix {

    # Users can provide a kde4_config.pri file that 
    # defines the values of KDE4_INCLUDE_DIR and 
    # KDE4_LIB_DIR
    exists( kde4_config.pri ) {
        include(kde4_config.pri)
    } else {
        # This project include file basically defines
        # KDE4_INCLUDE_DIR and KDE4_LIB_DIR as automatically
        # as possible

        exists( /usr/include/kde4/kparts/part.h ) {
            KDE4_INCLUDE_DIR = /usr/include/kde4
        } else {
            exists( $$(KDE4_DIR)/include/kparts/part.h ) {   
                KDE4_INCLUDE_DIR = $$(KDE4_DIR)/include
            }
        }

        exists( /usr/lib/kde4/devel/libkparts.so ) {
            KDE4_LIB_DIR = /usr/lib/kde4/devel
        } else {
            exists( /usr/lib/kde4/libkparts.so ) {
                KDE4_LIB_DIR = /usr/lib/kde4/devel
            } else {
                exists( $$(KDE4_DIR)/lib/libkparts.so ) {
                    KDE4_LIB_DIR = $$(KDE4_DIR)/lib
                }
            }
        }
    }
}


