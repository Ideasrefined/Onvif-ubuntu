#!/bin/bash

# debug message prefixes
errPrefix="\033[37;1;41m [${bs}] \033[0m"
succPrefix="\033[37;1;42m [${bs}] \033[0m"

# arg 1 - message text
function checkErr() {
	echo -en "${errPrefix} $1. Exiting..."
	echo ""
	exit 1
}

# arg 1 - message text
function succMessage() {
	echo -en "${succPrefix} $1"
	echo ""
}