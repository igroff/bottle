# vim: ft=sh
MY_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export INCLUDE_LIST=${INCLUDE_LIST}

export ERR_INVALID_MESSAGE=3

function is_json_valid(){
  JSON=${1?You didn\'t give me anything to test}
  PARSE_RESULT=$(echo ${JSON} | jq "." 2>&1)
  PARSE_RETURN_CODE=$?
  echo ${PARSE_RESULT}
  return $PARSE_RETURN_CODE
}

function get_message_body_from_stdin(){
  export FULL_MESSAGE=$(cat)
  FULL_MESSAGE=$(is_json_valid "${FULL_MESSAGE}")
  VALID_TEST=$?
  if [ ${VALID_TEST} -eq 0 ]; then
    BOTTLE_MESSAGE=$(echo "${FULL_MESSAGE}" | jq --raw-output --compact-output ".Body") 
    [ $? -eq 0 -a "${BOTTLE_MESSAGE}" != "null" ] || die "failed to pull bottle message from message ( ${FULL_MESSAGE} )"
    export BOTTLE_MESSAGE
  else
    echo "Error parsing message message ( ${FULL_MESSAGE} )"
  fi
  return $VALID_TEST
}

###############################################################################
# Checks to see if we have what appears to be a valid message on STDIN, this
# also parses out the required portions of a message specifically
# FULL_MESSAGE - the whole message
# MESSAGE_SOURCE - the 'source' value, which is required for a valid message
###############################################################################
function get_message_or_die(){
  get_message_body_from_stdin
  if [ $? -ne 0 ]; then
    die "failed getting message from stdin" ${ERR_INVALID_MESSAGE}
  fi
  MESSAGE_SOURCE=$(echo "${BOTTLE_MESSAGE}" | jq --raw-output ".source")
  [ -n "${MESSAGE_SOURCE}" -a "null" != "${MESSAGE_SOURCE}" ] || \
    die "message is invalid, contains no source value ( ${BOTTLE_MESSAGE} )"
  [ $? -eq 0 ] || die "message is not valid json" ${ERR_INVALID_MESSAGE}
}

function send_message_to_router() {
  MESSAGE=${1?You must provide a message to send to the router queue}
  if [ -n "${DO_NOT_QUEUE}" ]; then
    echo ${MESSAGE}
  else
    eval ${AWS_CLI} sqs send-message --queue-url ${ROUTER_QUEUE_URL} --message-body \'${FULL_MESSAGE}\'
  fi
}

function send_message_to_task_queue(){
  local TASK=${1?You must provide a message to send to the task queue}
  if [ -n "${DO_NOT_QUEUE}" ]; then
    echo "${TASK}"
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
