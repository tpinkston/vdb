SRC_PATH=src
OBJ_PATH=obj
TEST_PATH=test
LIB_NAME=libvdis.a

CPP=g++
CPP_FLAGS=-g -fPIC -std=c++0x -Wall -Ienumerations

OBJECTS=\
$(OBJ_PATH)/vdis_appearance.o \
$(OBJ_PATH)/vdis_byte_buffer.o \
$(OBJ_PATH)/vdis_byte_stream.o \
$(OBJ_PATH)/vdis_data_types.o \
$(OBJ_PATH)/vdis_datum_records.o \
$(OBJ_PATH)/vdis_entity_types_all.o \
$(OBJ_PATH)/vdis_entity_types.o \
$(OBJ_PATH)/vdis_enumerations.o \
$(OBJ_PATH)/vdis_enums.o \
$(OBJ_PATH)/vdis_integer.o \
$(OBJ_PATH)/vdis_logger.o \
$(OBJ_PATH)/vdis_network.o \
$(OBJ_PATH)/vdis_object_types_all.o \
$(OBJ_PATH)/vdis_object_types.o \
$(OBJ_PATH)/vdis_pdus.o \
$(OBJ_PATH)/vdis_services.o \
$(OBJ_PATH)/vdis_standard_variable_records.o \
$(OBJ_PATH)/vdis_string.o \
$(OBJ_PATH)/vdis_variable_parameter_records.o

TEST_OBJECTS=\
$(OBJ_PATH)/vdis_test_byte_buffer.o \
$(OBJ_PATH)/vdis_test_byte_stream.o \
$(OBJ_PATH)/vdis_test_common.o \
$(OBJ_PATH)/vdis_test_entity_types.o \
$(OBJ_PATH)/vdis_test_object_types.o \
$(OBJ_PATH)/vdis_test_pdus.o \
$(OBJ_PATH)/vdis_test_services.o

TARGET=vdis_test
TARGET_INCLUDES=-I$(TEST_PATH) -I$(SRC_PATH)

all: $(TARGET)

clean:
	@rm -rfv $(TARGET) $(LIB_NAME) $(OBJ_PATH)
.PHONY: clean

data:
	@data/update.sh
.PHONY: data

#### Directories:
directories: $(OBJ_PATH)
$(OBJ_PATH):
	@mkdir -pv $(OBJ_PATH)
	
#### Library:
$(LIB_NAME): $(OBJECTS)
	@echo linking: $(LIB_NAME)
	@ar -rs $(LIB_NAME) $(OBJECTS)

#### Target executables:
$(TARGET) : data directories $(LIB_NAME) $(TEST_OBJECTS) $(TEST_PATH)/vdis_test.h $(TEST_PATH)/vdis_test.cpp
	@echo linking: $@
	@$(CPP) $(CPP_FLAGS) -o $@ $(TEST_OBJECTS) $(LIB_NAME) $(TARGET_INCLUDES) ${TEST_PATH}/vdis_test.cpp

#### Object files:
$(OBJ_PATH)/vdis_appearance.o : $(SRC_PATH)/vdis_appearance.cpp $(SRC_PATH)/vdis_appearance.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_byte_buffer.o : $(SRC_PATH)/vdis_byte_buffer.cpp $(SRC_PATH)/vdis_byte_buffer.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_byte_stream.o : $(SRC_PATH)/vdis_byte_stream.cpp $(SRC_PATH)/vdis_byte_stream.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_data_types.o : $(SRC_PATH)/vdis_data_types.cpp $(SRC_PATH)/vdis_data_types.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_datum_records.o : $(SRC_PATH)/vdis_datum_records.cpp $(SRC_PATH)/vdis_datum_records.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_entity_types_all.o : $(SRC_PATH)/vdis_entity_types_all.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_entity_types.o : $(SRC_PATH)/vdis_entity_types.cpp $(SRC_PATH)/vdis_entity_types.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_enumerations.o : $(SRC_PATH)/vdis_enumerations.cpp $(SRC_PATH)/vdis_enumerations.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_enums.o : $(SRC_PATH)/vdis_enums.cpp $(SRC_PATH)/vdis_enums.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_integer.o : $(SRC_PATH)/vdis_integer.cpp $(SRC_PATH)/vdis_integer.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_logger.o : $(SRC_PATH)/vdis_logger.cpp $(SRC_PATH)/vdis_logger.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_network.o : $(SRC_PATH)/vdis_network.cpp $(SRC_PATH)/vdis_network.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_object_types_all.o : $(SRC_PATH)/vdis_object_types_all.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_object_types.o : $(SRC_PATH)/vdis_object_types.cpp $(SRC_PATH)/vdis_object_types.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_pdus.o : $(SRC_PATH)/vdis_pdus.cpp $(SRC_PATH)/vdis_pdus.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_services.o : $(SRC_PATH)/vdis_services.cpp $(SRC_PATH)/vdis_services.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_standard_variable_records.o : $(SRC_PATH)/vdis_standard_variable_records.cpp $(SRC_PATH)/vdis_standard_variable_records.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_string.o : $(SRC_PATH)/vdis_string.cpp $(SRC_PATH)/vdis_string.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<
$(OBJ_PATH)/vdis_variable_parameter_records.o : $(SRC_PATH)/vdis_variable_parameter_records.cpp $(SRC_PATH)/vdis_variable_parameter_records.h
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

#### Test object files:
$(OBJ_PATH)/vdis_test_byte_buffer.o : $(TEST_PATH)/vdis_test_byte_buffer.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $< $(TARGET_INCLUDES)
$(OBJ_PATH)/vdis_test_byte_stream.o : $(TEST_PATH)/vdis_test_byte_stream.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $< $(TARGET_INCLUDES)
$(OBJ_PATH)/vdis_test_common.o : $(TEST_PATH)/vdis_test_common.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $< $(TARGET_INCLUDES)
$(OBJ_PATH)/vdis_test_entity_types.o : $(TEST_PATH)/vdis_test_entity_types.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $< $(TARGET_INCLUDES)
$(OBJ_PATH)/vdis_test_object_types.o : $(TEST_PATH)/vdis_test_object_types.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $< $(TARGET_INCLUDES)
$(OBJ_PATH)/vdis_test_pdus.o : $(TEST_PATH)/vdis_test_pdus.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $< $(TARGET_INCLUDES)
$(OBJ_PATH)/vdis_test_services.o : $(TEST_PATH)/vdis_test_services.cpp
	@echo compiling: $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $< $(TARGET_INCLUDES)
