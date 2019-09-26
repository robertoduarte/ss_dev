#/****************************************************************
#KPIT Cummins Infosystems Limited Pune, India. -01-Sept-2003.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
#*****************************************************************/

# Top level makefile. It will make appliations in directories defined below

SUBDIRS= app1 app2 app3 


all:
	for subdir in $(SUBDIRS) ; do \
	echo  ; \
	echo "Making application in $${subdir}" ; \
	echo  ; \
	if [ -f ./$${subdir}/makefile ] ; then \
		if ( cd ./$${subdir} ; $(MAKE) ) ; \
		then true ; \
		else exit 1 ; \
		fi \
	else true ; \
	fi ; \
	done ; 

ram:
	for subdir in $(SUBDIRS) ; do \
	echo  ; \
	echo "Making application in $${subdir}" ; \
	echo  ; \
	if [ -f ./$${subdir}/makefile ] ; then \
		if ( cd ./$${subdir} ; $(MAKE) ram ) ; \
		then true ; \
		else exit 1 ; \
		fi \
	else true ; \
	fi ; \
	done ; 

clean:
	for subdir in $(SUBDIRS) ; do \
	echo  ; \
	echo "Making application in $${subdir}" ; \
	echo  ; \
	if [ -f ./$${subdir}/makefile ] ; then \
		if ( cd ./$${subdir} ; $(MAKE) clean ) ; \
		then true ; \
		else exit 1 ; \
		fi \
	else true ; \
	fi ; \
	done ; 

