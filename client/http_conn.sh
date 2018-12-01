#!/bin/bash
#
# Simulate legal http connection
function print_help ()
{
  echo "Usage: \"http_conn.sh -d IP -p PORT\""
  echo -e "It is mandatory to include -d and -p option, script should be run with sudo."
  echo -e "-u \t --url \t\tUrl to be requested, should be like http://www.hackathon.com:80."
  echo -e "-c \t --count \t\tNumber of http get requests to be sent."
  echo -e "-i \t --interval \t\tInterval between iterations."
  echo -e "-H \t --HOST \t\tAdd host header to the curl command."
  exit
}

ip_destination=""
host=""


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
    "-H"|"--HOST")
      host="--header 'Host: www.Hackathon.com'"
      shift
      ;;
    *)
      echo "Try http_conn.sh -h or --help"
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
  curl $url $host
  echo $count
  iterator=$(($iterator+1))
done
