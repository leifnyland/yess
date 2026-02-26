#!/bin/bash

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
gparent_path="$(dirname "$parent_path")"
results="$gparent_path/tst/"
yess="$parent_path/yess"
dir="/u/css/classes/3481/234/lab10tests"

tests=( error1 error2 error3 error4 error5 error6
        error7 error8 error9 error10 error11 error12
        error13 error14 nop irmovq1 irmovq2
        addq andq subq xorq addqfor1 addqfor2 addqfor3
        addqfor4 addqfor5 addqfor6 rrmovq cmovle cmovl
        cmove cmovne cmovge cmovg mrmovq1 mrmovq2
        pushq1 pushq2 popq1 popq2 pushqpopq rmmovq
        memfor1 memfor2 except1 except2 except3 except4 except5 except6
        except7 loaduse1 loaduse2 jmp je jg jge jl jle jne
        addOne sum jecall
      )

numTests=0
numPasses=0

if [ ! -e $results ]; then
    mkdir $results
fi

if [ "$(ls -A $results)" ]; then
   rm $results*
fi

for atest in ${tests[@]}
do
   infile="$dir/$atest.yo"
   if [ ! -r  $infile ];
   then
      echo "Uh oh. $infile is not readable or doesn't exist."
      echo "Notify your instructor."
      exit
   fi
   studoutfile="$atest.sdump"
   instoutfile="$dir/$atest.idump"
   rm -f $studoutfile
   $yess $infile > $studoutfile
   rm -f diffs
   diff $instoutfile $studoutfile > diffs
   if [ ! -e $studoutfile ] ||  [ ! -s $studoutfile ] || [ -s diffs ]; then
      cp $instoutfile $results
      cp $infile $results
      mv $studoutfile $results
      echo "Testing $infile ... failed"
      cat diffs
      rm -f diffs
   else
      rm -f diffs
      rm -f $studoutfile
      echo "Testing $infile ... passed"
      numPasses=$(($numPasses+1))
   fi
   numTests=$(($numTests+1))
done
echo " "
echo "$numPasses passed out of $numTests tests."

if [ $numPasses -ne $numTests ]; then
   echo "Files saved in your $results directory."
   echo "Input files end with a .yo."
   echo "Your output files end with a .sdump."
   echo "Correct output files end with a .idump."
fi 

