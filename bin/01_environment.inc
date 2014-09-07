# vim: ft=sh
[ -r ~/.bottle.env ] && source ~/.bottle.env

export SUBSCRIBER_REPO_DIR=${SUBSCRIBER_REPO_DIR?Missing subscriber repo dir, halting}
export TASK_QUEUE_URL=${TASK_QUEUE_URL?Missing task queue url, halting}
export ROUTER_QUEUE_URL=${ROUTER_QUEUE_URL?Missing router queue url, halting}
export OTE_LOCK_TABLE=${OTE_LOCK_TABLE-ote_locks}
export AWS_CLI=${AWS_CLI?Missing aws cli specification, halting}
export QUEUE_POLL_DURATION=${QUEUE_POLL_DURATION-5}

export EC_OK=0
export EC_INFO_NO_MESSAGES_IN_QUEUE=1
