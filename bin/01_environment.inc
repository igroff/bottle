# vim: ft=sh
[ -r ~/.bottle.env ] && source ~/.bottle.env

export SUBSCRIBER_REPO_DIR=${SUBSCRIBER_REPO_DIR?Missing subscriber repo dir, halting}
export TASK_QUEUE_URL=${TASK_QUEUE_URL?Missing task queue url, halting}
export ROUTER_QUEUE_URL=${ROUTER_QUEUE_URL?Missing router queue url, halting}
export OTE_LOCK_TABLE=${OTE_LOCK_TABLE-ote_locks}
export AWS_CLI=${AWS_CLI?Missing aws cli specification, halting}
export QUEUE_POLL_DURATION=${QUEUE_POLL_DURATION-5}
# this is the url that, if set, messages about failed message sends
# will be sent to.  They'll be sent just as messages would be sent to
# the bottle HTTP api (the intention is that you even use bottle here)
# so data will be posted to this URL as a JSON blob with a source 
# property equal to bottle/push/error
export PUSH_ERROR_NOTIFY_URL=${PUSH_ERROR_NOTIFY_URL}

export EC_OK=0
export EC_INFO_NO_MESSAGES_IN_QUEUE=1
