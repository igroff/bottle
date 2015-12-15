.PHONY: build install start

build:
	# do nothing, because there is no build

install:
	# yup, starphleet is gonna run us as ubuntu
	sudo ./install ubuntu
	# we'll pick up our environment from our starphleet ship level configuration
	sudo bash -c 'cat /etc/starphleet.d/* > /etc/bottle'
	# and we can get anything we might need from our orders which should
	# be the 3rd space delimited field of the 'container_environment' line from
	# our /home/ubuntu/start file, yup super dependent on starphleet structure
	sudo bash -c 'cat $(cat /home/ubuntu/start | grep container_environment | cut -d " " -f 3) >> /etc/bottle'
	# the upstart scripts expect the app to live at ~/bottle
	ln -s /home/ubuntu/app ~/bottle
	# now when bottle runs it can get then environment info we've gathered together above
	sudo start bottle

start:
	# unpublished, it doesn't start
