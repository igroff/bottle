# vim: ft=sh
MY_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export INCLUDE_LIST=${INCLUDE_LIST}

export ERR_INVALID_MESSAGE=3

function is_json_valid(){
  JSON=${1?You didn\'t give me anything to test}
  echo ${JSON} | jq "." > /dev/null 2>&1
  return $?
}

function get_message_body_from_stdin(){
  export MESSAGE_BODY=$(cat | sed -E -e $'s/\'/\'\\\\\'\'/g')
  is_json_valid "${MESSAGE_BODY}"
  VALID_TEST=$?
  [ ${VALID_TEST} -eq 0 ] && export MESSAGE_BODY=$(echo "${MESSAGE_BODY}" | jq --compact-output ".") 
  return $VALID_TEST
}

###############################################################################
# Checks to see if we have what appears to be a valid message on STDIN, this
# also parses out the required portions of a message specifically
# MESSAGE_BODY - the whole message
# MESSAGE_SOURCE - the 'source' value, which is required for a valid message
###############################################################################
function get_message_or_die(){
  get_message_body_from_stdin
  MESSAGE_SOURCE=$(echo "${MESSAGE_BODY}" | jq --raw-output ".source")
  [ -n "${MESSAGE_SOURCE}" -a "null" != "${MESSAGE_SOURCE}" ] || \
    die "message is invalid, contains no source value ${MESSAGE_BODY}"
  [ $? -eq 0 ] || die "message is not valid json" ${ERR_INVALID_MESSAGE}
}

function send_message_to_router() {
  MESSAGE=${1?You must provide a message to send to the router queue}
  if [ -n "${DO_NOT_QUEUE}" ]; then
    echo ${MESSAGE}
  else
    eval ${AWS_CLI} sqs send-message --queue-url ${ROUTER_QUEUE_URL} --message-body \'${MESSAGE_BODY}\'
  fi
}

function send_message_to_task_queue(){
  local TASK=${1?You must provide a message to send to the task queue}
  if [ -n "${DO_NOT_QUEUE}" ]; then
    echo ${TASK}
  else
    ${AWS_CLI} sqs send-message \
       --queue-url ${TASK_QUEUE_URL} \
       --message-body "${TASK}"
  fi
}

export -f is_json_valid
export -f get_message_or_die
export -f get_message_body_from_stdin
export -f send_message_to_task_queue
