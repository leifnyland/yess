#!/bin/bash

dir="/u/classes/3481/234/lab4"
./lab4 > student.output
fullpath=$dir/instructor.output
#diff $dir/instructor.output student.output > diffs
diff $fullpath student.output > diffs


if [ -s diffs ]; then
   echo ' '
   echo 'Does not produce expected output.'
   echo "It should produce identical output to the contents of $fullpath"
   echo ' '
else
   echo ' '
   echo 'All tests pass.'
fi
rm diffs
rm student.output
