######################################################################
# Generic Component Framework Library from VCreate Logic
######################################################################

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = source 

isEmpty(IGNORE_COMPONENTS): SUBDIRS += components
isEmpty(IGNORE_TOOLS): SUBDIRS += tools
isEmpty(IGNORE_EXAMPLES): SUBDIRS += examples


