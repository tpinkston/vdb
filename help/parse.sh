#!/bin/bash

SCRIPT_NAME=`basename $0`
SOURCE_FILE="$1"
TARGET_FILE="$2"
TITLE="$3"

if [ -z "$SOURCE_FILE" ] || [ -z "$TARGET_FILE" ]; then
    echo "$SCRIPT_NAME: missing file argument(s)" >&2
    exit 1
elif [ ! -f "$SOURCE_FILE" ]; then
    echo "$SCRIPT_NAME: file does not exist: $SOURCE_FILE" >&2
    exit 1
fi

if [ $SOURCE_FILE -nt $TARGET_FILE ]; then
    echo "generating: $TARGET_FILE"
    cat /dev/null > $TARGET_FILE
    echo "// This file is auto-generated..." >> $TARGET_FILE
    echo "//" >> $TARGET_FILE
    echo "inline void print_${TITLE}_help(void)" >> $TARGET_FILE
    echo "{" >> $TARGET_FILE
    while IFS='' read line;
    do
        echo "    std::cout << \"$line\" << std::endl;" >> $TARGET_FILE
    done < $SOURCE_FILE
    echo "}" >> $TARGET_FILE
fi

