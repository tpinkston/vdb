SRC_PATH=src
OBJ_PATH=obj
BIN_PATH=bin
HELP_PATH=help

VDIS_LIB=vdis/libvdis.a

TARGET_CAPTURE=$(BIN_PATH)/vdb-capture
TARGET_COMMENT=$(BIN_PATH)/vdb-comment
TARGET_ENTITIES=$(BIN_PATH)/vdb-entities
TARGET_ENUMS=$(BIN_PATH)/vdb-enums
TARGET_LIST=$(BIN_PATH)/vdb-list
TARGET_OBJECTS=$(BIN_PATH)/vdb-objects
TARGET_PLAYBACK=$(BIN_PATH)/vdb-playback
TARGET_SUMMARIZE=$(BIN_PATH)/vdb-summarize

CPP=g++
CPP_FLAGS=-g -fPIC -std=c++0x -Wall -Ivdis/src -Ivdis/enumerations

GIT_BRANCH=$(shell BRANCH=`git rev-parse --abbrev-ref HEAD`; echo $${BRANCH})
GIT_COMMIT=$(shell COMMIT=`git rev-parse HEAD`; echo $${COMMIT:0:7})

TARGETS=\
$(TARGET_CAPTURE) \
$(TARGET_COMMENT) \
$(TARGET_ENTITIES) \
$(TARGET_ENUMS) \
$(TARGET_LIST) \
$(TARGET_OBJECTS) \
$(TARGET_PLAYBACK) \
$(TARGET_SUMMARIZE)

HELP_FILES=\
${SRC_PATH}/vdb_capture_help.h \
${SRC_PATH}/vdb_comment_help.h \
${SRC_PATH}/vdb_entities_help.h \
${SRC_PATH}/vdb_enums_help.h \
${SRC_PATH}/vdb_list_help.h \
${SRC_PATH}/vdb_objects_help.h \
${SRC_PATH}/vdb_playback_help.h \
${SRC_PATH}/vdb_summarize_help.h

OBJECTS=\
$(OBJ_PATH)/vdb_associations.o \
$(OBJ_PATH)/vdb_common.o \
$(OBJ_PATH)/vdb_file_reader.o \
$(OBJ_PATH)/vdb_file_stream.o \
$(OBJ_PATH)/vdb_file_types.o \
$(OBJ_PATH)/vdb_file_writer.o \
$(OBJ_PATH)/vdb_filter.o \
$(OBJ_PATH)/vdb_fires.o \
$(OBJ_PATH)/vdb_lasers.o \
$(OBJ_PATH)/vdb_options.o \
$(OBJ_PATH)/vdb_pdu_data.o \
$(OBJ_PATH)/vdb_print.o \
$(OBJ_PATH)/vdb_scan.o

all: $(TARGETS)
.PHONY : all

rebuild: clean all
.PHONY : all

version:
	@echo "version: ${GIT_BRANCH}-${GIT_COMMIT}"
	@cp ${SRC_PATH}/vdb_git.h.in ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_BRANCH.*/VDB_GIT_BRANCH "${GIT_BRANCH}"/' ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_COMMIT.*/VDB_GIT_COMMIT "${GIT_COMMIT}"/' ${SRC_PATH}/vdb_git.h
.PHONY : version

clean: clean_vdis
	@rm -fv ${SRC_PATH}/vdb_git.h
	@rm -fv $(HELP_FILES)
	@rm -rfv $(BIN_PATH)
	@rm -rfv $(OBJ_PATH)
	@rm -fv $(TARGETS)
.PHONY : clean

target_depends: vdis directories version $(HELP_FILES) $(OBJECTS)
.PHONY : target_depends

#### Directories:
directories: $(OBJ_PATH) $(BIN_PATH)
$(OBJ_PATH):
	@mkdir -pv $(OBJ_PATH)
$(BIN_PATH):
	@mkdir -pv $(BIN_PATH)

# VDIS library:
vdis: $(VDIS_LIB)
$(VDIS_LIB):
	@cd vdis; $(MAKE) --no-print-directory all
clean_vdis:
	@cd vdis; $(MAKE) --no-print-directory clean
.PHONY : clean_vdis

#### Target executables:
$(TARGET_CAPTURE): target_depends $(SRC_PATH)/vdb_capture.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_capture.cpp
$(TARGET_COMMENT): target_depends $(SRC_PATH)/vdb_comment.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_comment.cpp
$(TARGET_ENTITIES): target_depends $(SRC_PATH)/vdb_entities.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_entities.cpp
$(TARGET_ENUMS): target_depends $(SRC_PATH)/vdb_enums.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_enums.cpp
$(TARGET_LIST): target_depends $(SRC_PATH)/vdb_list.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_list.cpp
$(TARGET_OBJECTS): target_depends $(SRC_PATH)/vdb_objects.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_objects.cpp
$(TARGET_PLAYBACK): target_depends $(SRC_PATH)/vdb_playback.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_playback.cpp
$(TARGET_SUMMARIZE): target_depends $(SRC_PATH)/vdb_summarize.*
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(OBJECTS) $(VDIS_LIB) ${SRC_PATH}/vdb_summarize.cpp

#### Help files:
${SRC_PATH}/vdb_capture_help.h: $(HELP_PATH)/help_capture.txt
	@$(HELP_PATH)/parse.sh $< $@
${SRC_PATH}/vdb_comment_help.h: $(HELP_PATH)/help_comment.txt
	@$(HELP_PATH)/parse.sh $< $@
${SRC_PATH}/vdb_entities_help.h: $(HELP_PATH)/help_entities.txt
	@$(HELP_PATH)/parse.sh $< $@
${SRC_PATH}/vdb_enums_help.h: $(HELP_PATH)/help_enums.txt
	@$(HELP_PATH)/parse.sh $< $@
${SRC_PATH}/vdb_list_help.h: $(HELP_PATH)/help_list.txt
	@$(HELP_PATH)/parse.sh $< $@
${SRC_PATH}/vdb_objects_help.h: $(HELP_PATH)/help_objects.txt
	@$(HELP_PATH)/parse.sh $< $@
${SRC_PATH}/vdb_playback_help.h: $(HELP_PATH)/help_playback.txt
	@$(HELP_PATH)/parse.sh $< $@
${SRC_PATH}/vdb_summarize_help.h: $(HELP_PATH)/help_summarize.txt
	@$(HELP_PATH)/parse.sh $< $@

#### Object files:
$(OBJ_PATH)/vdb_associations.o : $(SRC_PATH)/vdb_associations.cpp $(SRC_PATH)/vdb_associations.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_common.o : $(SRC_PATH)/vdb_common.cpp $(SRC_PATH)/vdb_common.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_entities.o : $(SRC_PATH)/vdb_entities.cpp $(SRC_PATH)/vdb_entities.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_enums.o : $(SRC_PATH)/vdb_enums.cpp $(SRC_PATH)/vdb_enums.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_file_reader.o : $(SRC_PATH)/vdb_file_reader.cpp $(SRC_PATH)/vdb_file_reader.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_file_stream.o : $(SRC_PATH)/vdb_file_stream.cpp $(SRC_PATH)/vdb_file_stream.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_file_types.o : $(SRC_PATH)/vdb_file_types.cpp $(SRC_PATH)/vdb_file_types.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_file_writer.o : $(SRC_PATH)/vdb_file_writer.cpp $(SRC_PATH)/vdb_file_writer.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_filter.o : $(SRC_PATH)/vdb_filter.cpp $(SRC_PATH)/vdb_filter.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_fires.o : $(SRC_PATH)/vdb_fires.cpp $(SRC_PATH)/vdb_fires.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_lasers.o : $(SRC_PATH)/vdb_lasers.cpp $(SRC_PATH)/vdb_lasers.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_list.o : $(SRC_PATH)/vdb_list.cpp $(SRC_PATH)/vdb_list.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_options.o : $(SRC_PATH)/vdb_options.cpp $(SRC_PATH)/vdb_options.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_pdu_data.o : $(SRC_PATH)/vdb_pdu_data.cpp $(SRC_PATH)/vdb_pdu_data.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_playback.o : $(SRC_PATH)/vdb_playback.cpp $(SRC_PATH)/vdb_playback.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_print.o : $(SRC_PATH)/vdb_print.cpp $(SRC_PATH)/vdb_print.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_scan.o : $(SRC_PATH)/vdb_scan.cpp $(SRC_PATH)/vdb_scan.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdb_summarize.o : $(SRC_PATH)/vdb_summarize.cpp $(SRC_PATH)/vdb_summarize.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
	