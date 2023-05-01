#!/bin/bash

SOURCE_FOLDER="/home/ning/Workspace/HW12/Practicum2"
TARGET_USERNAME="ning"
TARGET_IP="10.211.55.6"
DESTINATION_FOLDER="/home/ning/Workspace"

while true; do
    inotifywait -r -e modify,attrib,close_write,move,create,delete $SOURCE_FOLDER
    rsync -avz --progress --delete $SOURCE_FOLDER $TARGET_USERNAME@$TARGET_IP:$DESTINATION_FOLDER
done
