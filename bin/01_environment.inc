# vim: ft=sh
[ -r ~/.bottle.env ] && source ~/.bottle.env

SUBSCRIBER_REPO_DIR=${SUBSCRIBER_REPO_DIR?Missing subscriber repo dir, halting}
TASK_QUEUE_URL=${TASK_QUEUE_URL?Missing task queue url, halting}
ROUTER_QUEUE_URL=${ROUTER_QUEUE_URL?Missing router queue url, halting}
OTE_LOCK_TABLE=${OTE_LOCK_TABLE-ote_locks}
AWS_CLI=${AWS_CLI?Missing aws cli specification, halting}
