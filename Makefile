SRC_PATH=source
BUILD_PATH=build
ANT_PATH=$(shell which ant 2> /dev/null)
GIT_BRANCH=$(shell BRANCH=`git rev-parse --abbrev-ref HEAD`; echo $${BRANCH})
GIT_COMMIT=$(shell COMMIT=`git rev-parse HEAD`; echo $${COMMIT:0:7})
THIS_FILE=$(lastword $(MAKEFILE_LIST))

build: data usage version
ifeq ("$(wildcard ${BUILD_PATH}/Makefile)", "")
	@$(MAKE) --no-print-directory -f $(THIS_FILE) builder
endif
	@echo
	@echo "build:"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile
.PHONY : build


all: data usage version
ifeq ("$(wildcard ${BUILD_PATH}/Makefile)", "")
	@$(MAKE) --no-print-directory -f $(THIS_FILE) builder
endif
	@echo
	@echo "all:"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile all
.PHONY : all


data:
	@echo
	@echo "data:"
	@./convert_data.sh
.PHONY : data


usage:
	@echo
	@echo "usage:"
	@usage/usage.sh usage/usage_main.txt ${SRC_PATH}/vdb_usage_main.txt
	@usage/usage.sh usage/usage_capture.txt ${SRC_PATH}/vdb_usage_capture.txt
	@usage/usage.sh usage/usage_playback.txt ${SRC_PATH}/vdb_usage_playback.txt
	@usage/usage.sh usage/usage_list.txt ${SRC_PATH}/vdb_usage_list.txt
	@usage/usage.sh usage/usage_query.txt ${SRC_PATH}/vdb_usage_query.txt
	@usage/usage.sh usage/usage_comment.txt ${SRC_PATH}/vdb_usage_comment.txt
	@usage/usage.sh usage/usage_uncomment.txt ${SRC_PATH}/vdb_usage_uncomment.txt
	@usage/usage.sh usage/usage_enums.txt ${SRC_PATH}/vdb_usage_enums.txt
	@usage/usage.sh usage/usage_entities.txt ${SRC_PATH}/vdb_usage_entities.txt
	@usage/usage.sh usage/usage_objects.txt ${SRC_PATH}/vdb_usage_objects.txt
.PHONY : usage


version:
	@echo
	@echo "version:"
	@echo "${GIT_BRANCH}-${GIT_COMMIT}"
	@cp ${SRC_PATH}/vdb_git.h.in ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_BRANCH.*/VDB_GIT_BRANCH "${GIT_BRANCH}"/' ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_COMMIT.*/VDB_GIT_COMMIT "${GIT_COMMIT}"/' ${SRC_PATH}/vdb_git.h
.PHONY : version 


help:
ifeq ("$(wildcard ${BUILD_PATH}/Makefile)", "")
	@$(MAKE) -f $(THIS_FILE) builder
endif
	@echo
	@echo "help: ${BUILD_PATH}/Makefile"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile help
.PHONY : help


builder: ${BUILD_PATH}/CMakeLists.txt
	@echo
	@echo "builder:"
	@cd ${BUILD_PATH}; cmake .
.PHONY : builder


clean_builder:
ifneq ("$(wildcard ${BUILD_PATH}/Makefile)", "")
	@echo
	@echo "clean_builder:"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile clean
endif
.PHONY : clean_builder


clean: clean_builder
	@echo
	@echo "clean:"
	@rm -fv vdb
	@rm -rfv ${SRC_PATH}/vdb_git.h
	@rm -rfv ${SRC_PATH}/vdb_usage_*.txt
	@rm -rfv ${SRC_PATH}/vdb_version.h
.PHONY : clean


clobber: clean
	@echo
	@echo "clobber:"
	@rm -fv ${BUILD_PATH}/cmake_install.cmake
	@rm -fv ${BUILD_PATH}/CMakeCache.txt
	@rm -fv ${BUILD_PATH}/Makefile
	@rm -rfv ${BUILD_PATH}/CMakeFiles  
.PHONY : clobber

