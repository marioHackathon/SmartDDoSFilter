#!/bin/bash
#
# Simulate legal http connection
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
    "-u"|"--url")
      url="$2"
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

if [ -z $count ]; then
  echo "Count not set, please try http_conn.sh -h or --help"
  exit 1;
fi

iterator=0

while [ $iterator -lt $count ]; do
  curl $url
  echo $count
  iterator=$(($iterator+1))
done
