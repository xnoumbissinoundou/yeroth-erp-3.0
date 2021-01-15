#!/bin/bash

ALL_SQL_UPGRADE_FILES="$1"

while read LINE; do
    cat "$LINE" >> "assemblate_result.sql"
done < "$ALL_SQL_UPGRADE_FILES"
