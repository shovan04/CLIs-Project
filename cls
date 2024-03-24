#! /bin/bash
if(sudo sync; echo 3 > /proc/sys/vm/drop_caches)
then
    clear
    echo "Cache cleared";
fi