SRC_PATH=src
OBJ_PATH=obj
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

all: data directories $(LIB_NAME)
.PHONY: all

clean:
	@rm -rfv $(LIB_NAME) $(OBJ_PATH)
.PHONY: clean

data:
	@data/update.sh
.PHONY: data

directories: $(OBJ_PATH)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)
	
$(LIB_NAME): $(OBJECTS)
	@echo Linking $(LIB_NAME)
	@ar -rs $(LIB_NAME) $(OBJECTS)
	
$(OBJ_PATH)/vdis_appearance.o : $(SRC_PATH)/vdis_appearance.cpp $(SRC_PATH)/vdis_appearance.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_byte_buffer.o : $(SRC_PATH)/vdis_byte_buffer.cpp $(SRC_PATH)/vdis_byte_buffer.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_byte_stream.o : $(SRC_PATH)/vdis_byte_stream.cpp $(SRC_PATH)/vdis_byte_stream.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_data_types.o : $(SRC_PATH)/vdis_data_types.cpp $(SRC_PATH)/vdis_data_types.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_datum_records.o : $(SRC_PATH)/vdis_datum_records.cpp $(SRC_PATH)/vdis_datum_records.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_entity_types_all.o : $(SRC_PATH)/vdis_entity_types_all.cpp
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_entity_types.o : $(SRC_PATH)/vdis_entity_types.cpp $(SRC_PATH)/vdis_entity_types.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_enumerations.o : $(SRC_PATH)/vdis_enumerations.cpp $(SRC_PATH)/vdis_enumerations.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_enums.o : $(SRC_PATH)/vdis_enums.cpp $(SRC_PATH)/vdis_enums.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_integer.o : $(SRC_PATH)/vdis_integer.cpp $(SRC_PATH)/vdis_integer.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_logger.o : $(SRC_PATH)/vdis_logger.cpp $(SRC_PATH)/vdis_logger.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_network.o : $(SRC_PATH)/vdis_network.cpp $(SRC_PATH)/vdis_network.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_object_types_all.o : $(SRC_PATH)/vdis_object_types_all.cpp
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_object_types.o : $(SRC_PATH)/vdis_object_types.cpp $(SRC_PATH)/vdis_object_types.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_pdus.o : $(SRC_PATH)/vdis_pdus.cpp $(SRC_PATH)/vdis_pdus.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_services.o : $(SRC_PATH)/vdis_services.cpp $(SRC_PATH)/vdis_services.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_standard_variable_records.o : $(SRC_PATH)/vdis_standard_variable_records.cpp $(SRC_PATH)/vdis_standard_variable_records.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_string.o : $(SRC_PATH)/vdis_string.cpp $(SRC_PATH)/vdis_string.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

$(OBJ_PATH)/vdis_variable_parameter_records.o : $(SRC_PATH)/vdis_variable_parameter_records.cpp $(SRC_PATH)/vdis_variable_parameter_records.h
	@echo Compiling $<
	@$(CPP) $(CPP_FLAGS) -c -o $@ $<

