TRUE=1
FALSE=0
SRC_PATH=source
BUILD_PATH=build
PCAP_VERSION="libpcap-1.7.4"
PCAP_PATH="${BUILD_PATH}/${PCAP_VERSION}"
ANT_PATH=$(shell which ant 2> /dev/null)
GIT_BRANCH=$(shell BRANCH=`git rev-parse --abbrev-ref HEAD`; echo $${BRANCH})
GIT_COMMIT=$(shell COMMIT=`git rev-parse HEAD`; echo $${COMMIT:0:7})
THIS_FILE=$(lastword $(MAKEFILE_LIST))

build: pcap data help examples version builder
	@echo
	@echo "build:"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile
.PHONY : build


all: pcap data help examples version builder
	@echo
	@echo "all:"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile all
.PHONY : all


data:
	@echo
	@echo "data:"
	@./vdis/data/update.sh
.PHONY : data


help:
	@echo
	@echo "help:"
	@./parse_text.sh help.txt ${SRC_PATH}/vdb_help.txt
.PHONY : help


examples:
	@echo
	@echo "examples:"
	@./parse_text.sh examples/examples-capture.txt ${SRC_PATH}/examples/vdb_examples_capture.txt
	@./parse_text.sh examples/examples-playback.txt ${SRC_PATH}/examples/vdb_examples_playback.txt
	@./parse_text.sh examples/examples-list.txt ${SRC_PATH}/examples/vdb_examples_list.txt
	@./parse_text.sh examples/examples-summary.txt ${SRC_PATH}/examples/vdb_examples_summary.txt
	@./parse_text.sh examples/examples-comment.txt ${SRC_PATH}/examples/vdb_examples_comment.txt
	@./parse_text.sh examples/examples-uncomment.txt ${SRC_PATH}/examples/vdb_examples_uncomment.txt
	@./parse_text.sh examples/examples-enums.txt ${SRC_PATH}/examples/vdb_examples_enums.txt
	@./parse_text.sh examples/examples-entities.txt ${SRC_PATH}/examples/vdb_examples_entities.txt
	@./parse_text.sh examples/examples-objects.txt ${SRC_PATH}/examples/vdb_examples_objects.txt
.PHONY : examples


version:
	@echo
	@echo "version:"
	@echo "${GIT_BRANCH}-${GIT_COMMIT}"
	@cp ${SRC_PATH}/vdb_git.h.in ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_BRANCH.*/VDB_GIT_BRANCH "${GIT_BRANCH}"/' ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_COMMIT.*/VDB_GIT_COMMIT "${GIT_COMMIT}"/' ${SRC_PATH}/vdb_git.h
.PHONY : version 


pcap:
	@echo
	@echo "pcap: ${PCAP_VERSION}"
	@if [ -d ${PCAP_PATH} ]; then echo "Already decompressed..."; fi
	@if [ ! -d ${PCAP_PATH} ]; then \
		echo "Decompressing..."; \
		cd ${BUILD_PATH}; \
		tar -xf ${PCAP_VERSION}.tar.gz; \
		find ${PCAP_VERSION} -name "*.h" -exec chmod -w {} \;; \
		find ${PCAP_VERSION} -name "*.c" -exec chmod -w {} \;; \
	fi
	@if [ -e ${PCAP_PATH}/Makefile ]; then echo "Already configured..."; fi
	@if [ ! -e ${PCAP_PATH}/Makefile ]; then \
	echo "Configuring..."; \
		cd ${PCAP_PATH}; \
		./configure; \
		echo; \
	fi
	@if [ -e ${PCAP_PATH}/libpcap.a ]; then echo "Already built..."; fi
	@if [ ! -e ${PCAP_PATH}/libpcap.a ]; then \
		echo "Building..."; \
		cd ${PCAP_PATH}; \
		$(MAKE) --no-print-directory -f Makefile; \
		echo; \
	fi
.PHONY : pcap


clean_pcap:
	@if [ -e ${PCAP_PATH}/Makefile ]; then \
		echo; \
		echo "clean_pcap:"; \
		cd ${PCAP_PATH}; \
		$(MAKE) --no-print-directory -f Makefile clean; \
	fi
.PHONY : clean_pcap


builder: ${BUILD_PATH}/CMakeLists.txt
	@if [ ! -e ${BUILD_PATH}/Makefile ]; then \
		echo; \
		echo "builder:"; \
		cd ${BUILD_PATH}; cmake --no-print-directory .; \
	fi
.PHONY : builder


clean_builder:
	@if [ -e ${BUILD_PATH}/Makefile ]; then \
		echo; \
		echo "clean_builder:"; \
		cd ${BUILD_PATH}; \
		$(MAKE) --no-print-directory -f Makefile clean; \
	fi
.PHONY : clean_builder


clean: clean_pcap clean_builder
	@echo
	@echo "clean:"
	@rm -fv vdb
	@rm -rfv ${SRC_PATH}/vdb_help.txt
	@rm -rfv ${SRC_PATH}/vdb_git.h
	@rm -rfv ${SRC_PATH}/vdb_version.h
	@rm -rfv ${SRC_PATH}/examples/vdb_examples_*.txt
.PHONY : clean


clobber: clean
	@echo
	@echo "clobber:"
	@rm -fv ${BUILD_PATH}/cmake_install.cmake
	@rm -fv ${BUILD_PATH}/CMakeCache.txt
	@rm -fv ${BUILD_PATH}/Makefile
	@rm -rfv ${BUILD_PATH}/CMakeFiles  
	@rm -rfv ${PCAP_PATH}  
.PHONY : clobber

