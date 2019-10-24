#!/bin/bash
echo "Renaming files"
for fe in $(ls *.JPG)
do
   f=${fe%.*}; n="$f.jpg"
   echo "Renaming $fe to $n"
   mv $fe $n
done

echo "Rescaling files"
for f in $(ls *.jpg)
do
   echo "Processing $f"
   mogrify -scale 25% $f
done
