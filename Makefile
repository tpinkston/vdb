SRC_PATH=src
HELP_PATH=help
BUILD_PATH=build

HELP_FILES=\
${SRC_PATH}/vdb_capture_help.h \
${SRC_PATH}/vdb_comment_help.h \
${SRC_PATH}/vdb_entities_help.h \
${SRC_PATH}/vdb_enums_help.h \
${SRC_PATH}/vdb_extract_help.h \
${SRC_PATH}/vdb_global_help.h \
${SRC_PATH}/vdb_list_help.h \
${SRC_PATH}/vdb_objects_help.h \
${SRC_PATH}/vdb_playback_help.h \
${SRC_PATH}/vdb_summarize_help.h

GIT_BRANCH=$(shell BRANCH=`git rev-parse --abbrev-ref HEAD`; echo $${BRANCH})
GIT_COMMIT=$(shell COMMIT=`git rev-parse HEAD`; echo $${COMMIT:0:7})

VDB_BRANCH=$(shell BRANCH=`[ -e ${SRC_PATH}/vdb_git.h ] && grep VDB_GIT_BRANCH ${SRC_PATH}/vdb_git.h | cut -d' ' -f3 | sed 's/"//g'`; echo $${BRANCH})
VDB_COMMIT=$(shell COMMIT=`[ -e ${SRC_PATH}/vdb_git.h ] && grep VDB_GIT_COMMIT ${SRC_PATH}/vdb_git.h | cut -d' ' -f3 | sed 's/"//g'`; echo $${COMMIT})

build: ${HELP_FILES} data version builder
	@echo
	@echo "build:"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile
.PHONY : build


all: ${HELP_FILES} data version builder
	@echo
	@echo "all:"
	@cd ${BUILD_PATH}; $(MAKE) --no-print-directory -f Makefile all
.PHONY : all


data:
	@echo
	@echo "data:"
	@./vdis/data/update.sh
.PHONY : data


version:
	@echo
	@echo "version:"
	@if [ "${GIT_BRANCH}" != "${VDB_BRANCH}" ] || [ "${GIT_COMMIT}" != "${VDB_COMMIT}" ]; then \
		echo "Updating git fields: ${GIT_BRANCH}-${GIT_COMMIT}"; \
		cp ${SRC_PATH}/vdb_git.h.in ${SRC_PATH}/vdb_git.h; \
		sed -i 's/VDB_GIT_BRANCH.*/VDB_GIT_BRANCH "${GIT_BRANCH}"/' ${SRC_PATH}/vdb_git.h; \
		sed -i 's/VDB_GIT_COMMIT.*/VDB_GIT_COMMIT "${GIT_COMMIT}"/' ${SRC_PATH}/vdb_git.h; \
	fi
.PHONY : version


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


clean: clean_builder
	@echo
	@echo "clean:"
	@rm -rfv vdb
	@rm -rfv $(HELP_FILES)
	@rm -rfv ${SRC_PATH}/vdb_git.h
	@rm -rfv ${SRC_PATH}/vdb_version.h
.PHONY : clean


clobber: clean
	@echo
	@echo "clobber:"
	@rm -rfv ${BUILD_PATH}/cmake_install.cmake
	@rm -rfv ${BUILD_PATH}/CMakeCache.txt
	@rm -rfv ${BUILD_PATH}/Makefile
	@rm -rfv ${BUILD_PATH}/CMakeFiles  
.PHONY : clobber

${SRC_PATH}/vdb_capture_help.h: $(HELP_PATH)/help_capture.txt
	@$(HELP_PATH)/parse.sh $< $@ capture
${SRC_PATH}/vdb_comment_help.h: $(HELP_PATH)/help_comment.txt
	@$(HELP_PATH)/parse.sh $< $@ comment
${SRC_PATH}/vdb_entities_help.h: $(HELP_PATH)/help_entities.txt
	@$(HELP_PATH)/parse.sh $< $@ entities
${SRC_PATH}/vdb_enums_help.h: $(HELP_PATH)/help_enums.txt
	@$(HELP_PATH)/parse.sh $< $@ enums
${SRC_PATH}/vdb_extract_help.h: $(HELP_PATH)/help_extract.txt
	@$(HELP_PATH)/parse.sh $< $@ extract
${SRC_PATH}/vdb_global_help.h: $(HELP_PATH)/help_global.txt
	@$(HELP_PATH)/parse.sh $< $@ global
${SRC_PATH}/vdb_list_help.h: $(HELP_PATH)/help_list.txt
	@$(HELP_PATH)/parse.sh $< $@ list
${SRC_PATH}/vdb_objects_help.h: $(HELP_PATH)/help_objects.txt
	@$(HELP_PATH)/parse.sh $< $@ objects
${SRC_PATH}/vdb_playback_help.h: $(HELP_PATH)/help_playback.txt
	@$(HELP_PATH)/parse.sh $< $@ playback
${SRC_PATH}/vdb_summarize_help.h: $(HELP_PATH)/help_summarize.txt
	@$(HELP_PATH)/parse.sh $< $@ summarize

