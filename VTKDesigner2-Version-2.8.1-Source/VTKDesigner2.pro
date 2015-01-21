######################################################################
# VTK Designer 2 Project from VCreate Logic (P) Ltd Bangalore
######################################################################

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = GCF Main Components

#!isEmpty(COMMERCIAL_BUILD) {
#        !exists( $${PROJECT_BASE}/CommercialComponents/CommercialComponents.pro ) {
#                error( "Cannot find " $${PROJECT_BASE}"/CommercialComponents/CommercialComponents.pro");
#        }

#        SUBDIRS += CommercialComponents
#        message("Building GNU/GPL and Commercial components")
#}

