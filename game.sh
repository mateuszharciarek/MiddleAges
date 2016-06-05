#!/bin/bash

metric=8
digits='^[0-9]+$'

n=10
k=100
s=1
x1=0
y1=0
x2=0
y2=0
ai1="0"
ai2="0"

argnumber=$#


i=1

# wprowadzanie n
if [ "$i" -le "$argnumber" ] && [ ${!i} == "-n" ] ; then
  ((i++))
  if [ "$i" -le "$argnumber" ] && [[ ${!i} =~ $digits ]] && [ "${!i}" -lt 2147483648 ] ; then
    n=${!i};
    ((i++))
  else
    exit 1
  fi
fi

# wprowadzanie k
if [ "$i" -le "$argnumber" ] && [ ${!i} = "-k" ] ; then
  ((i++))
  if [ "$i" -le "$argnumber" ] && [[ ${!i} =~ $digits ]] ; then
    k=${!i};
    ((i++))
  else
    exit 1
  fi
fi

# wprowadzanie s
if [ "$i" -le "$argnumber" ] && [ ${!i} = "-s" ] ; then
  ((i++))
  if [ "$i" -le "$argnumber" ] && [[ ${!i} =~ $digits ]] ; then
    s=${!i};
    ((i++))
  else
    exit 1
  fi
fi

# wprowadzanie x1,y1
if [ "$i" -le "$argnumber" ] && [ ${!i} = "-p1" ] ; then
  ((i++))
  if [ "$i" -le "$argnumber" ] && [[ ${!i} =~ ^[0-9,0-9]+$ ]] ; then
    tmp=${!i}
    array=(${tmp//,/ })
    x1=${array[0]};
    y1=${array[1]}
    ((i++))
  else
    exit 1
  fi
fi

# wprowadzanie x2,y2
if [ "$i" -le "$argnumber" ] && [ ${!i} = "-p2" ] ; then
  ((i++))
  if [ "$i" -le "$argnumber" ] && [[ ${!i} =~ ^[0-9,0-9]+$ ]] ; then
    tmp=${!i}
    array=(${tmp//,/ })
    x2=${array[0]};
    y2=${array[1]}
    ((i++))
  else
    exit 1
  fi
fi

# wprowadzanie ai1
if [ "$i" -le "$argnumber" ] && [ ${!i} = "-ai1" ] ; then
  ((i++))
  if [ "$i" -le "$argnumber" ] && [ -f ${!i} ] ; then
    ai1=${!i};
    ((i++))
  else
    exit 1
  fi
fi

# wprowadzanie ai2
if [ "$i" -le "$argnumber" ] && [ ${!i} = "-ai2" ] ; then
  ((i++))
  if [ "$i" -le "$argnumber" ] && [ -f ${!i} ] ; then
    ai2=${!i};
    ((i++))
  else
    exit 1
  fi
fi

if [ "$i" -le "$argnumber" ] ; then
  exit 1
fi

#------------------------------------------------------------
#both points are initialize
if [ $x1 -ne 0 ] && [ $y1 -ne 0 ] && [ $x2 -ne 0 ] &&  [ $y2 -ne 0 ] ; then

  if [ "$x1" -gt "$x2" ] ; then
    abs_x=$(($x1 - $x2))
  else
    abs_x=$(($x2 - $x1))
  fi

  if [ "$y1" -gt "$y2" ] ; then
    abs_y=$(($y1 - $y2))
  else
    abs_y=$(($y2 - $y1))
  fi

  if [ "$metric" -gt "$abs_x" ] && [ "$metric" -gt "$abs_y" ] ; then
    exit 1
  fi
#points aren't initialize or one point isn't initialize
else
  #no points initialize
  if ( [ "$x1" -eq 0 ] && [ "$y1" -eq 0 ] ) && ( [ "$x2" -eq 0 ] && [ "$y2" -eq 0 ] ) ; then
    abs_x=0
    abs_y=0
   range=$(( ( RANDOM % ( $n - $metric ) )  + 1 ))
   y1=$(( ( RANDOM % ( $range ) )  + 1 ))
   if [  "$(( n - range - metric ))" -le "0" ] ; then
    y2=$n
   else
    y2=$(( ( RANDOM % ( $n - $range - $metric ) )  + $range + $metric + 1 ))
   fi
   x1=$(( ( RANDOM % ( $n - 3 ) )  + 1 ))
   x2=$(( ( RANDOM % ( $n - 3) )  + 1 ))

   var_bool=$(( ( RANDOM % ( 2 ) )  ))
   if [ "$var_bool" -eq "1" ] ; then
    tmp_xr=$x1
    tmp_yr=$y1
    x1=$x2
    y1=$y2
    x2=$tmp_xr
    y2=$tmp_yr
   fi

  #one poin is initialize
  else
    if [ "$x1" -eq 0 ] && [ "$y1" -eq 0 ] ; then
      init_x=$x2
      init_y=$y2
    else
      init_x=$x1
      init_y=$y1
    fi

    #it is inpossible to draw second point
    if [ $(( $init_x - $metric - 3 )) -le 0 ] && [ $(( $n - $init_x - $metric - 3 )) -le 0 ] && [ $(( $init_y - $metric )) -le 0 ] && [ $(( $n - $init_y - $metric )) -le 0 ] ; then
      exit 1
    fi

    abs_x=0
    abs_y=0
    while [ "$abs_x" -lt "$metric" ] && [ "$abs_y" -lt "$metric" ] ; do
      sleep 0.1
      tmp_x=$(( ( RANDOM % ( $n - 3 ) )  + 1 )) #[1,n]
      sleep 0.1
      tmp_y=$(( ( RANDOM % ( $n ) )  + 1 )) #[1,n]

    if [ "$tmp_x" -gt "$init_x" ] ; then
      abs_x=$(($tmp_x - $init_x))
    else
      abs_x=$(($init_x - $tmp_x))
    fi
    if [ $tmp_y -gt $init_y ] ; then
      abs_y=$(($tmp_y - $init_y))
    else
      abs_y=$(($init_y - $tmp_y))
    fi
    done

    if [ "$init_x" -eq "$x1" ] && [ "$init_y" -eq "$y1" ] ; then
      x2=$tmp_x
      y2=$tmp_y
    else
      x1=$tmp_x
      y1=$tmp_y
    fi
  fi
fi

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 3<>$PIPE
rm $PIPE

PIPE=$(mktemp -u)
mkfifo $PIPE
exec 4<>$PIPE
rm $PIPE


#---------------

PIPE2=$(mktemp -u)
mkfifo $PIPE2
exec 5<>$PIPE2
rm $PIPE2

PIPE2=$(mktemp -u)
mkfifo $PIPE2
exec 6<>$PIPE2
rm $PIPE2


#-------------

PIPE3=$(mktemp -u)
mkfifo $PIPE3
exec 7<>$PIPE3
rm $PIPE3

PIPE3=$(mktemp -u)
mkfifo $PIPE3
exec 8<>$PIPE3
rm $PIPE3


if [ "$ai1" == "0" ] && [ "$ai2" == "0" ] ; then
 #(echo -e "INIT $n $k 1 $x1 $y1 $x2 $y2\nINIT $n $k 2 $x1 $y1 $x2 $y2" && cat) | ./sredniowiecze_gui_with_libs.sh -human1 -human2

 echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&7
 echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&7
 ./sredniowiecze_gui_with_libs.sh -human1 -human2 <&7 &
 PID_GUI=$!
 wait $PID_GUI
 exit $?
else

 #human vs ai2
  if [ "$ai1" == "0" ] && [ "$ai2" != "0" ] ; then

    echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&5
    echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&7
    echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&7
    $ai2 <&5 >&6 &
    PID_AI2=$!
    ./sredniowiecze_gui_with_libs.sh -human1 <&7 >&8 &
    PID_GUI=$!

    i=0
    a=""
    while [ $i -le $k ] ; do
    a=""
      while [ "$a" != "END_TURN" ] && [ "$a" != "!" ] ; do
	a="!"
	if ! kill -0 $PID_GUI ; then
	  kill $PID_AI2
	  wait $PID_AI2
	  exit_code=$?
	  if [ $exit_code -lt 42 ] ; then
	    exit 0
	  fi
	  exit 1
	fi
	read -t 1 a <&8
	if [ "$a" != "" ] ; then
	  echo $a
	  echo $a >&5
	fi
      done

      a=""

      while [ "$a" != "END_TURN" ] && [ "$a" != "!" ] ; do
	a="!"
	if ! kill -0 $PID_GUI && ! kill -0 $PID_AI2 ; then
	  wait $PID_AI2
	  exit_code=$?
	  if [ "$exit_code" -ge "42" ] ; then
	    exit 1
	  else
	    exit 0
	  fi
	fi
	read -t 1 a <&6
	if [ "$a" != "" ] ; then
	  echo $a
	  echo $a >&7
	fi
	if ! kill -0 $PID_GUI && ! kill -0 $PID_AI2 ; then
	  wait $PID_AI2
	  exit_code=$?
	  if [ "$exit_code" -ge "42" ] ; then
	    exit 1
	  else
	    exit 0
	  fi
	fi
	#to nizej chyba niekoniecznie
	if ! kill -0 $PID_AI2 ; then
	  wait $PID_AI2
	  exit_code=$?
	  if [ "$exit_code" -ge "42" ] ; then
	    exit 1
	  else
	    exit 0
	  fi
	fi

      done
  ((i++))
  done
     if ! kill -0 $PID_GUI ; then
	  wait $PID_AI1
	  exit_code=$?
	  if [ "$exit_code" == "42" ] ; then
	    exit 1
	  else
	    exit 0
	  fi
      fi
  fi

  #ai1 vs human
  if [ "$ai1" != "0" ] && [ "$ai2" == "0" ] ; then

    echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
    echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&7
    echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&7
    $ai1 <&3 >&4 &
    PID_AI1=$!
    ./sredniowiecze_gui_with_libs.sh -human2 <&7 >&8 &
    PID_GUI=$!

    i=0
    a=""
     while [ $i -le $k ] ; do
     a=""
     while [ "$a" != "END_TURN" ] && [ "$a" != "!" ] ; do
       a="!"
       if ! kill -0 $PID_GUI && ! kill -0 $PID_AI1 ; then
	  wait $PID_AI1
	  exit_code=$?
	  if [ "$exit_code" == "42" ] ; then
	    exit 1
	  else
	    exit 0
	  fi
      fi
      read -t 1 a <&4
       if [ "$a" != "" ] ; then
	echo $a
	echo $a >&7
       fi
      if ! kill -0 $PID_GUI && ! kill -0 $PID_AI1 ; then
	  wait $PID_AI1
	  exit_code=$?
	  if [ "$exit_code" == "42" ] ; then
	    exit 1
	  else
	    exit 0
	  fi
      fi
     done
     a=""

     while [ "$a" != "END_TURN" ] && [ "$a" != "!" ] ; do
       a="!"
       if ! kill -0 $PID_GUI ; then

	  kill $PID_AI1
	  wait $PID_AI1
	  exit_code=$?
	  if [ $exit_code -lt 42 ] ; then
	    exit 0
	  fi
	  exit 1
       fi
       read -t 1 a <&8
       if [ "$a" != "" ] ; then
	echo $a
	echo $a >&3
       fi
     done
   ((i++))
   done
   if ! kill -0 $PID_GUI ; then
	  wait $PID_AI1
	  exit_code=$?
	  if [ "$exit_code" == "42" ] ; then
	    exit 1
	  else
	    exit 0
	  fi
      fi
  fi
  #ai1 vs ai2
  if [ "$ai1" != "0" ] && [ "$ai2" != "0" ] ; then
    echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&3
    sleep 0.1
    echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&5
    sleep 0.1
    echo "INIT $n $k 1 $x1 $y1 $x2 $y2" >&7
    sleep 0.1
    echo "INIT $n $k 2 $x1 $y1 $x2 $y2" >&7
    sleep 0.1

    $ai1 <&3 >&4 &
    PID_AI1=$!
    sleep 0.1
    $ai2 <&5 >&6 &
    PID_AI2=$!
    sleep 0.1
    ./sredniowiecze_gui_with_libs.sh <&7 >&8 &
    PID_GUI=$!
    sleep 0.1

    i=0
    a=""
    echo "INIT $n $k 1-2 $x1 $y1 $x2 $y2"
    while [ $i -lt $k ] ; do

    if ! kill -0 $PID_AI1 ; then
      kill_ai1=1
    else
      kill_ai1=0
    fi
   if ! kill -0 $PID_AI2 ; then
      kill_ai2=1
    else
      kill_ai2=0
    fi
   if ! kill -0 $PID_GUI ; then
      kill_gui=1
    else
      kill_gui=0
    fi
    #------------------------------------------------------>

    if [ "$kill_gui" == "1" ] || ( [ "$kill_ai1" == "1" ] && [ "$kill_ai2" == "1" ] ) ; then
      if [ "$kill_gui" == "1" ] ; then
	kill $PID_AI1
	kill $PID_AI2
	wait $PID_AI1
	exit_code_ai1=$?
	wait $PID_AI2
	exit_code_ai2=$?
	if [ $exit_code_ai1 -ge 42 ] || [ $exit_code_ai2 -ge 42 ] ; then
	  exit 1
	else
	  exit 0
	fi
      fi

      if [ "$kill_ai1" == "1" ] ; then

      a="!"
      read -t 1  a <&4
      if [ "$a" != "" ] ; then
      echo $a
      echo $a >&5
      echo $a >&7
      fi
      a="!"
      read -t 1  a <&6
      if [ "$a" != "" ] ; then
      echo $a
      echo $a >&3
      echo $a >&7
      fi

	kill $PID_AI2
	wait $PID_AI1
	exit_code_ai1=$?
	wait $PID_AI2
	exit_code_ai2=$?
	if [ $exit_code_ai1 -ge 42 ] || [ $exit_code_ai2 -ge 42 ] ; then
	  exit 0
	else
	  exit 1
	fi
      fi

      if [ "$kill_ai2" == "1" ] ; then

	kill $PID_AI1
	wait $PID_AI1
	exit_code_ai1=$?
	wait $PID_AI2
	exit_code_ai2=$?
	if [ $exit_code_ai1 -ge 42 ] || [ $exit_code_ai2 -ge 42 ] ; then
	  exit 1
	else
	  exit 0
	fi
      fi
    fi

    if [ $kill_ai1 == "1" ] ; then
      wait $PID_AI1
      exit_code_ai1=$?
      if [ $exit_code_ai1 -ge 42 ] ; then
	exit 1
      fi
    fi

    if [ "$kill_ai2" == "1" ] ; then
      wait $PID_AI2
      exit_code_ai2=$?
      if [ $exit_code_ai2 -ge 42 ] ; then
	exit 1
      fi
    fi
    #---------------------------------------------------------<

    a=""
      while [ "$a" != "END_TURN" ] && [ "$a" != "!" ] ; do

       a="!"
       read -t 1  a <&4
	if [ "$a" != "" ] ; then
	  echo $a
	  echo $a >&5
	  echo $a >&7
	else
	 a="END_TURN"
	 i=$k
	fi

      if ! kill -0 $PID_AI1 ; then
	kill $PID_AI1
	 wait $PID_AI1
	 exit_code_ai1=$?
	 if [ $exit_code_ai1 -ge 42 ] ; then
	  exit 1
	 fi
       fi

      done
      sleep $s
      a=""

      while [ "$a" != "END_TURN" ] && [ "$a" != "!" ] ; do

       a="!"
       read -t 1 a <&6
	if [ "$a" != "" ] ; then
	  echo $a
	  echo $a >&3
	  echo $a >&7
	else
	 a="END_TURN"
	 i=$k
	fi

      if ! kill -0 $PID_AI2 ; then
	kill $PID_AI2
	 wait $PID_AI2
	 exit_code_ai2=$?
	 if [ $exit_code_ai2 -ge 42 ] ; then
	  exit 1
	 fi
       fi
      done
      sleep $s
    ((i++))
  done
  fi
fi


if kill -0 $PID_AI1 ; then
  kill $PID_AI1
  wait $PID_AI1
  exit_code_ai1=$?
else
  wait $PID_AI1
  exit_code_ai1=$?
fi

if kill -0 $PID_AI2 ; then
  kill $PID_AI2
  wait $PID_AI2
  exit_code_ai2=$?
else
  wait $PID_AI2
  exit_code_ai2=$?
fi

if kill -0 $PID_GUI ; then
  kill $PID_GUI
  wait $PID_GUI
  #exit $?
  exit 0
fi
if [ $exit_code_ai1 -ge 42 ] || [ $exit_code_ai2 -ge 42 ] ; then
  exit 1
else
  exit 0
fi
