#/bin/bash

SCRIPT_NAME=`basename $0`

while getopts f option
do
    case "$option" in
    f)  FORCE_CONVERT=true;;
    *)  exit;;
    esac
done

cd `dirname $0`

# Input/output files for entity types.
ENTITY_TYPES_IN=entity_types/entity_types.csv
ENTITY_TYPES_OUT=../vdis_entity_types_all.cpp

# Input/output files for object types.
OBJECT_TYPES_IN=object_types/object_types.csv
OBJECT_TYPES_OUT=../vdis_object_types_all.cpp

# Output files for master header/source.
ENUMS_H_OUT=../vdis_enums.h
ENUMS_CPP_OUT=../vdis_enums.cpp

# Check for input files.
if [ ! -e "$ENTITY_TYPES_IN" ]; then
    echo "$SCRIPT_NAME: missing input file: $ENTITY_TYPES_IN" >&2
    exit 1
elif [ ! -e "$OBJECT_TYPES_IN" ]; then
    echo "$OBJECT_NAME: missing input file: $OBJECT_TYPES_IN" >&2
    exit 1
fi

# Function to write the "script generated" source header.
function script_generated() {
    cat /dev/null > "$1"
    echo "// -----------------------------------------------" >> "$1"
    echo "// This file is script-generated..." >> "$1"
    echo "// -----------------------------------------------" >> "$1"
}

# =============================================================================
# Convert entity types CSV to C++ source code definitions.
# =============================================================================
CONVERT=false
if [ ! -e "$ENTITY_TYPES_OUT" ]; then
    # Output file does not exist...
    CONVERT=true
elif [ "$ENTITY_TYPES_IN" -nt "$ENTITY_TYPES_OUT" ]; then
    # Input file is newer than the output file...
    CONVERT=true
fi

if [ "$CONVERT" == true ] || [ "$FORCE_CONVERT" == true ]; then
    echo "$SCRIPT_NAME: updating: $ENTITY_TYPES_OUT"
    script_generated "$ENTITY_TYPES_OUT"
    echo "" >> "$ENTITY_TYPES_OUT"
    echo "#include \"vdis_entity_types.h\"" >> "$ENTITY_TYPES_OUT"
    echo "" >> "$ENTITY_TYPES_OUT"
    echo "void vdis::entity_types::add_all(void)" >> "$ENTITY_TYPES_OUT"
    echo "{" >> "$ENTITY_TYPES_OUT"
    while IFS='' read line;
    do
        if [ -z "$line" ]; then
            # Blank line
            echo >> "$ENTITY_TYPES_OUT"
        elif [[ $line == \#* ]]; then
            # Comment line
            echo "    //${line:1}" >> "$ENTITY_TYPES_OUT"
        else
            # Definition line
            echo "    add($line);" >> "$ENTITY_TYPES_OUT"
        fi
    done < $ENTITY_TYPES_IN
    echo "}" >> "$ENTITY_TYPES_OUT"
    echo "" >> "$ENTITY_TYPES_OUT"
fi

# =============================================================================
# Convert object types CSV to C++ source code definitions.
# =============================================================================
CONVERT=false
if [ ! -e "$OBJECT_TYPES_OUT" ]; then
    # Output file does not exist...
    CONVERT=true
elif [ "$OBJECT_TYPES_IN" -nt "$OBJECT_TYPES_OUT" ]; then
    # Input file is newer than the output file...
    CONVERT=true
fi

if [ "$CONVERT" == true ] || [ "$FORCE_CONVERT" == true ]; then
    echo "$SCRIPT_NAME: updating: $OBJECT_TYPES_OUT"
    script_generated "$OBJECT_TYPES_OUT"
    echo "" >> "$OBJECT_TYPES_OUT"
    echo "#include \"vdis_object_types.h\"" >> "$OBJECT_TYPES_OUT"
    echo "" >> "$OBJECT_TYPES_OUT"
    echo "void vdis::object_types::add_all(void)" >> "$OBJECT_TYPES_OUT"
    echo "{" >> "$OBJECT_TYPES_OUT"
    while IFS='' read line;
    do
        if [ -z "$line" ]; then
            # Blank line
            echo >> "$OBJECT_TYPES_OUT"
        elif [[ $line == \#* ]]; then
            # Comment line
            echo "    //${line:1}" >> "$OBJECT_TYPES_OUT"
        else
            # Definition line
            echo "    add($line);" >> "$OBJECT_TYPES_OUT"
        fi
    done < $OBJECT_TYPES_IN
    echo "}" >> "$OBJECT_TYPES_OUT"
    echo "" >> "$OBJECT_TYPES_OUT"
fi

# =============================================================================
# Convert enumeration CSV files to C++ source code definitions.
# If one enumeration is updating then the ENUMS_H_OUT and ENUMS_CPP_OUT
# files will need updating.
# =============================================================================
CONVERTED=false
ENUM_NAMES=()
for csv in `ls enumerations`; do
    ENUM_IN="enumerations/$csv"
    ENUM_OUT=`echo "../enumerations/$csv" | sed 's/.csv/.h/'`
    ENUM_NAME_LOWER=`echo "$csv" | sed 's/.csv//' | awk '{print tolower($0)}'`
    ENUM_NAME_UPPER=`echo "$csv" | sed 's/.csv//' | awk '{print toupper($0)}'`
    ENUM_NAMES+=("$ENUM_NAME_LOWER")
    
    CONVERT=false
    if [ ! -e "$ENUM_OUT" ]; then
        CONVERT=true
    elif [ "$ENUM_IN" -nt "$ENUM_OUT" ]; then
        CONVERT=true
    fi

    if [ "$CONVERT" == true ] || [ "$FORCE_CONVERT" == true ]; then
        echo "$SCRIPT_NAME: updating: $ENUM_OUT"
        script_generated "$ENUM_OUT"
        echo "" >> "$ENUM_OUT"
        echo "#ifndef ${ENUM_NAME_UPPER}_H" >> "$ENUM_OUT"
        echo "#define ${ENUM_NAME_UPPER}_H" >> "$ENUM_OUT"
        echo "" >> "$ENUM_OUT"
        echo "namespace vdis" >> "$ENUM_OUT"
        echo "{" >> "$ENUM_OUT"
        echo "    typedef enum" >> "$ENUM_OUT"
        echo "    {" >> "$ENUM_OUT"
        while IFS='' read line;
        do
            if [ -z "$line" ]; then
                # Blank line
                echo >> "$ENUM_OUT"
            elif [[ $line == \#* ]]; then
                # Comment line
                echo "        //${line:1}" >> "$ENUM_OUT"
            else
                # Definition line
                TOKENS=(${line//,/ })
                echo "        ${TOKENS[1]} = ${TOKENS[0]}," >> "$ENUM_OUT"
            fi
        done < $ENUM_IN
        echo "        ${ENUM_NAME_UPPER}_END" >> "$ENUM_OUT"
        echo "    } ${ENUM_NAME_LOWER}_e;" >> "$ENUM_OUT"
        echo "}" >> "$ENUM_OUT"
        echo "" >> "$ENUM_OUT"
        echo "#endif" >> "$ENUM_OUT"
        CONVERTED=true
    fi
done

# =============================================================================
# Update the master header and source file if needed.
# =============================================================================
if [ "$CONVERTED" == true ]; then
    # Update the header file.
    echo "$SCRIPT_NAME: updating: $ENUMS_H_OUT"
    script_generated "$ENUMS_H_OUT"
    echo "" >> "$ENUMS_H_OUT"
    echo "#ifndef VDIS_ENUMS_H" >> "$ENUMS_H_OUT"
    echo "#define VDIS_ENUMS_H" >> "$ENUMS_H_OUT"
    echo "" >> "$ENUMS_H_OUT"
    echo "#include <ostream>" >> "$ENUMS_H_OUT"
    echo "" >> "$ENUMS_H_OUT"
    echo "#include \"vdis_enumerations.h\"" >> "$ENUMS_H_OUT"
    echo "" >> "$ENUMS_H_OUT"
    for NAME in "${ENUM_NAMES[@]}"; do
        echo "#include \"${NAME}.h\"" >> "$ENUMS_H_OUT"
    done < $ENUM_IN
    echo "" >> "$ENUMS_H_OUT"
    for NAME in "${ENUM_NAMES[@]}"; do
        NAME_UPPER=`echo "$NAME" | awk '{print toupper($0)}'`
        echo "#define ENUM_${NAME_UPPER} \"${NAME_UPPER}\"" >> "$ENUMS_H_OUT"
    done < $ENUM_IN
    echo "" >> "$ENUMS_H_OUT"
    for NAME in "${ENUM_NAMES[@]}"; do
        NAME_UPPER=`echo "$NAME" | awk '{print toupper($0)}'`
        echo "" >> "$ENUMS_H_OUT"
        echo "inline std::ostream &operator<<(std::ostream &out, vdis::${NAME}_e value)" >> "$ENUMS_H_OUT"
        echo "{" >> "$ENUMS_H_OUT"
        echo "    return vdis::enumerations::print(ENUM_${NAME_UPPER}, value, out);" >> "$ENUMS_H_OUT"
        echo "}" >> "$ENUMS_H_OUT"
    done < $ENUM_IN
    echo "" >> "$ENUMS_H_OUT"
    echo "#endif" >> "$ENUMS_H_OUT"
    echo "" >> "$ENUMS_H_OUT"

    # Update the source file.
    echo "$SCRIPT_NAME: updating: $ENUMS_CPP_OUT"
    script_generated "$ENUMS_CPP_OUT"
    echo "" >> "$ENUMS_CPP_OUT"
    echo "#include \"vdis_enumerations.h\"" >> "$ENUMS_CPP_OUT"
    echo "#include \"vdis_enums.h\"" >> "$ENUMS_CPP_OUT"
    echo "" >> "$ENUMS_CPP_OUT"
    echo "void vdis::enumerations::add_all(void)" >> "$ENUMS_CPP_OUT"
    echo "{" >> "$ENUMS_CPP_OUT"
    for csv in `ls enumerations`; do
        ENUM_IN="enumerations/$csv"
        ENUM_NAME_UPPER=`echo "$csv" | sed 's/.csv//' | awk '{print toupper($0)}'`
        echo >> "$ENUMS_CPP_OUT"
        echo "    // From file: $csv" >> "$ENUMS_CPP_OUT"
        echo "    //" >> "$ENUMS_CPP_OUT"
        while IFS='' read line;
        do
            if [ -z "$line" ]; then
                # Blank line
                echo >> "$ENUMS_CPP_OUT"
            elif [[ $line == \#* ]]; then
                # Comment line
                echo "    //${line:1}" >> "$ENUMS_CPP_OUT"
            else
                # Definition line
                TOKENS=(${line//,/ })
                echo "    add(ENUM_${ENUM_NAME_UPPER}, ${TOKENS[0]}, \"${TOKENS[1]}\");" >> "$ENUMS_CPP_OUT"
            fi
            TOKENS=(${line//,/ })
        done < $ENUM_IN
        echo "" >> "$ENUMS_CPP_OUT"
    done
    echo "}" >> "$ENUMS_CPP_OUT"
    echo "" >> "$ENUMS_CPP_OUT"
fi



