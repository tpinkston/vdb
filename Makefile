SRC_PATH=src
OBJ_PATH=obj
VDIS=vdis/libvdis.a
TARGET=vdb

CPP=g++
CPP_FLAGS=-g -fPIC -std=c++0x -Wall -Ivdis/src -Ivdis/enumerations

GIT_BRANCH=$(shell BRANCH=`git rev-parse --abbrev-ref HEAD`; echo $${BRANCH})
GIT_COMMIT=$(shell COMMIT=`git rev-parse HEAD`; echo $${COMMIT:0:7})

OBJECTS=\
$(OBJ_PATH)/vdb_associations.o \
$(OBJ_PATH)/vdb_capture.o \
$(OBJ_PATH)/vdb_comments.o \
$(OBJ_PATH)/vdb_common.o \
$(OBJ_PATH)/vdb_entities.o \
$(OBJ_PATH)/vdb_file_reader.o \
$(OBJ_PATH)/vdb_file_stream.o \
$(OBJ_PATH)/vdb_file_types.o \
$(OBJ_PATH)/vdb_file_writer.o \
$(OBJ_PATH)/vdb_filter.o \
$(OBJ_PATH)/vdb_fires.o \
$(OBJ_PATH)/vdb_lasers.o \
$(OBJ_PATH)/vdb_list.o \
$(OBJ_PATH)/vdb_options.o \
$(OBJ_PATH)/vdb_pdu_data.o \
$(OBJ_PATH)/vdb_playback.o \
$(OBJ_PATH)/vdb_print.o \
$(OBJ_PATH)/vdb_summary.o \
$(OBJ_PATH)/vdb.o

all:
	@cd vdis; $(MAKE) --no-print-directory
	@$(MAKE) --no-print-directory $(TARGET)
.PHONY : all

help:
	@./parse_text.sh help.txt ${SRC_PATH}/vdb_help.txt
.PHONY : help

examples:
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
	@echo "Setting version to '${GIT_BRANCH}-${GIT_COMMIT}'"
	@cp ${SRC_PATH}/vdb_git.h.in ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_BRANCH.*/VDB_GIT_BRANCH "${GIT_BRANCH}"/' ${SRC_PATH}/vdb_git.h
	@sed -i 's/VDB_GIT_COMMIT.*/VDB_GIT_COMMIT "${GIT_COMMIT}"/' ${SRC_PATH}/vdb_git.h
.PHONY : version 

clean:
	@cd vdis; $(MAKE) --no-print-directory clean
	@rm -rfv ${SRC_PATH}/vdb_git.h
	@rm -rfv ${SRC_PATH}/vdb_help.txt
	@rm -rfv ${SRC_PATH}/examples/vdb_examples_*.txt
	@rm -rfv $(OBJ_PATH)
	@rm -fv $(TARGET)
.PHONY : clean

directories: $(OBJ_PATH)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(TARGET): directories help examples version $(OBJECTS)
	@echo Linking $(TARGET)
	@$(CPP) -o $(TARGET) $(OBJECTS) $(VDIS)

$(OBJ_PATH)/vdb_associations.o : $(SRC_PATH)/vdb_associations.cpp $(SRC_PATH)/vdb_associations.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_capture.o : $(SRC_PATH)/vdb_capture.cpp $(SRC_PATH)/vdb_capture.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_comments.o : $(SRC_PATH)/vdb_comments.cpp $(SRC_PATH)/vdb_comments.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_common.o : $(SRC_PATH)/vdb_common.cpp $(SRC_PATH)/vdb_common.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_entities.o : $(SRC_PATH)/vdb_entities.cpp $(SRC_PATH)/vdb_entities.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_file_reader.o : $(SRC_PATH)/vdb_file_reader.cpp $(SRC_PATH)/vdb_file_reader.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_file_stream.o : $(SRC_PATH)/vdb_file_stream.cpp $(SRC_PATH)/vdb_file_stream.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_file_types.o : $(SRC_PATH)/vdb_file_types.cpp $(SRC_PATH)/vdb_file_types.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_file_writer.o : $(SRC_PATH)/vdb_file_writer.cpp $(SRC_PATH)/vdb_file_writer.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_filter.o : $(SRC_PATH)/vdb_filter.cpp $(SRC_PATH)/vdb_filter.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_fires.o : $(SRC_PATH)/vdb_fires.cpp $(SRC_PATH)/vdb_fires.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_lasers.o : $(SRC_PATH)/vdb_lasers.cpp $(SRC_PATH)/vdb_lasers.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_list.o : $(SRC_PATH)/vdb_list.cpp $(SRC_PATH)/vdb_list.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_options.o : $(SRC_PATH)/vdb_options.cpp $(SRC_PATH)/vdb_options.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_pdu_data.o : $(SRC_PATH)/vdb_pdu_data.cpp $(SRC_PATH)/vdb_pdu_data.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_playback.o : $(SRC_PATH)/vdb_playback.cpp $(SRC_PATH)/vdb_playback.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_print.o : $(SRC_PATH)/vdb_print.cpp $(SRC_PATH)/vdb_print.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb_summary.o : $(SRC_PATH)/vdb_summary.cpp $(SRC_PATH)/vdb_summary.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdb.o : $(SRC_PATH)/vdb.cpp ${SRC_PATH}/vdb_version.h ${SRC_PATH}/vdb_git.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
	