#!/bin/bash

SCRIPT_NAME=`basename $0`
SOURCE_FILE="$1"
TARGET_FILE="$2"

if [ -z "$SOURCE_FILE" ] || [ -z "$TARGET_FILE" ]; then
    echo "$SCRIPT_NAME: missing file argument(s)" >&2
    exit 1
elif [ ! -f "$SOURCE_FILE" ]; then
    echo "$SCRIPT_NAME: file does not exist: $SOURCE_FILE" >&2
    exit 1
fi

if [ $SOURCE_FILE -nt $TARGET_FILE ]; then
    echo "Updating $TARGET_FILE"
    echo "// AUTO GENERATED..." > $TARGET_FILE
    echo >> $TARGET_FILE
    while IFS='' read line;
    do
        echo "USAGE(\"$line\")" >> $TARGET_FILE
    done < $SOURCE_FILE
fi

