.PHONY: build install start

build:
	sudo apt-get update || true # we can have some non fatal errors here
	sudo apt-get install jq awscli --assume-yes

install:
	# yup, starphleet is gonna run us as ubuntu
	sudo ./install ubuntu
	# we'll pick up our environment from our starphleet ship level configuration
	sudo bash -c 'cat /etc/starphleet.d/* > /etc/bottle'
	# the upstart scripts expect the app to live at ~/bottle
	ln -s /home/ubuntu/app ~/bottle
	sudo initctl reload-configuration

start:
	# we don't have our 'start' file created until, well, now.
	# and we can get anything we might need from our orders which should
	# be the 3rd space delimited field of the 'container_environment' line from
	# our /home/ubuntu/start file, yup super dependent on starphleet structure
	# 
	# just to keep things clean, we'll filter out the autodeploy directive so we 
	# don't get extraneous errors
	sudo bash -c 'cat $$(cat /home/ubuntu/start | grep container_environment | cut -d " " -f 3 ) | grep -v -e "^autodeploy.*" >> /etc/bottle'
	sudo start bottle
	# now for a couple extra workers, as this is what we run traditionally
	sudo start bottle-router name=router1
	sudo start bottle-router name=router2
	sudo start bottle-task-worker name=task_worker1
	sudo start bottle-task-worker name=task_worker2
	sudo start bottle-task-worker name=task_worker3
	sudo start bottle-task-worker name=task_worker4
	exec sleep 99999999
