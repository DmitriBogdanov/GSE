# __________________________________ CONTENTS ___________________________________
#
#    Some common functions used by other scripts to make them not as verbose.
# _______________________________________________________________________________

source bash/colors.sh # defines ansi colors

# Seems to be the most robust way of checking if some command exists in $PATH,
# can be used inside 'if' blocks as a condition
command_exists() {
    builtin type -P "$1" &> /dev/null
}

# Some commands (such as 'cmake') are required in order to do even a minimal build,
# this is a shortcut for exiting if we can't find them 
require_command_exists() {
    if ! command_exists "$1" ;
    then
        printf "${ansi_red}Command [ $1 ] could not be found.${ansi_reset}"
        exit 1
    fi
}
