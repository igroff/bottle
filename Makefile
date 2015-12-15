.PHONY: build install start

build:
	# do nothing, because there is no build

install:
	# yup, starphleet is gonna run us as ubuntu
	./install ubuntu

start:
	# we'll pick up our environment from our starphleet ship level configuration
	cat /etc/starphleet.d/* > /etc/bottle
	# and we can get anything we might need from our orders which should
	# be the 3rd space delimited field of the 'container_environment' line from
	# our /home/ubuntu/start file, yup super dependent on starphleet structure
	cat $(cat /home/ubuntu/start | grep container_environment | cut -d ' ' -f 3) >> /etc/bottle
	# now when bottle runs it can get then environment info we've gathered together above
	start bottle
	# things are all managed by upstart, but we need something for
	# starphleet to bite onto so it thinks it's keeping something up and running
	sleep 999999
